#include "catch.hpp"
#include "duckdb/common/types/date.hpp"
#include "duckdb/common/types/time.hpp"
#include "duckdb/common/types/timestamp.hpp"
#include "duckdb/runtime/default/execution/operator/scan/csv/csv_reader_options.hpp"
#include "duckdb/main/appender.hpp"
#include "test_helpers.hpp"
#include "duckdb/main/client_data.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace duckdb;
using namespace std;

//! CSV Files
const string csv = "*.csv";
const string tsv = "*.tsv";
const string csv_gz = "csv.gz";
const string csv_zst = "csv.zst";
const string tbl_zst = "tbl.zst";

const string csv_extensions[5] = {csv, tsv, csv_gz, csv_zst, tbl_zst};

const char *run = std::getenv("DUCKDB_RUN_PARALLEL_CSV_TESTS");

bool RunParallel(const string &path, idx_t thread_count, idx_t buffer_size, bool set_temp_dir,
                 ColumnDataCollection *ground_truth = nullptr, const string &add_parameters = "") {
	DuckDB db(nullptr);
	Connection multi_conn(db);

	multi_conn.Query("PRAGMA verify_parallelism");
	if (set_temp_dir) {
		multi_conn.Query("PRAGMA temp_directory='offload.tmp'");
	}
	multi_conn.Query("SET preserve_insertion_order=false;");
	multi_conn.Query("PRAGMA threads=" + to_string(thread_count));
	duckdb::unique_ptr<MaterializedQueryResult> multi_threaded_result =
	    multi_conn.Query("SELECT * FROM read_csv_auto('" + path + "'" + add_parameters +
	                     ", buffer_size = " + to_string(buffer_size) + ") ORDER BY ALL");
	bool multi_threaded_passed;
	ColumnDataCollection *result = nullptr;
	if (multi_threaded_result->HasError()) {
		multi_threaded_passed = false;
	} else {
		multi_threaded_passed = true;
		result = &multi_threaded_result->Collection();
	}
	if (!ground_truth && !multi_threaded_passed) {
		// Two wrongs can make a right
		return true;
	}
	if (!ground_truth) {
		//! oh oh, this should not pass
		std::cout << path << " Failed on single threaded but succeeded on parallel reading" << '\n';
		return false;
	}
	if (!multi_threaded_passed) {
		std::cout << path << " Multithreaded failed" << '\n';
		std::cout << multi_threaded_result->GetError() << '\n';
		return false;
	}
	// Results do not match
	string error_message;
	if (!ColumnDataCollection::ResultEquals(*ground_truth, *result, error_message, false)) {
		std::cout << path << " Thread count: " << to_string(thread_count) << " Buffer Size: " << to_string(buffer_size)
		          << '\n';
		std::cout << error_message << '\n';
		return false;
	}
	return true;
}

bool RunFull(std::string &path, std::set<std::string> *skip = nullptr, const string &add_parameters = "",
             bool set_temp_dir = false) {
	DuckDB db(nullptr);
	Connection conn(db);
	if (!run) {
		return true;
	}
	// Here we run the csv file first with the single thread reader.
	// Then the parallel csv reader with a combination of multiple threads and buffer sizes.
	if (skip) {
		if (skip->find(path) != skip->end()) {
			// Gotta skip this
			return true;
		}
	}
	// Set max line length to 0 when starting a ST CSV Read
	conn.context->client_data->debug_set_max_line_length = true;
	conn.context->client_data->debug_max_line_length = 0;
	duckdb::unique_ptr<MaterializedQueryResult> single_threaded_res;
	ColumnDataCollection *ground_truth = nullptr;
	single_threaded_res =
	    conn.Query("SELECT * FROM read_csv_auto('" + path + "'" + add_parameters + ", parallel = 0) ORDER BY ALL");
	if (!single_threaded_res->HasError()) {
		ground_truth = &single_threaded_res->Collection();
	}
	// For parallel CSV Reading the buffer must be at least the size of the biggest line in the File.
	idx_t min_buffer_size = conn.context->client_data->debug_max_line_length + 2;
	// So our tests don't take infinite time, we will go till a max buffer size of 5 positions higher than the minimum.
	idx_t max_buffer_size = min_buffer_size + 5;
	// Let's go from 1 to 8 threads.
	bool all_tests_passed = true;
	for (auto thread_count = 1; thread_count <= 8; thread_count++) {
		for (auto buffer_size = min_buffer_size; buffer_size < max_buffer_size; buffer_size++) {
			all_tests_passed = all_tests_passed &&
			                   RunParallel(path, thread_count, buffer_size, set_temp_dir, ground_truth, add_parameters);
		}
	}

	return all_tests_passed;
}

// Collects All CSV-Like files from folder and execute Parallel Scans on it
void RunTestOnFolder(const string &path, std::set<std::string> *skip = nullptr, const string &add_parameters = "") {
	DuckDB db(nullptr);
	Connection con(db);
	bool all_tests_passed = true;
	auto &fs = duckdb::FileSystem::GetFileSystem(*con.context);
	for (auto &ext : csv_extensions) {
		auto csv_files = fs.Glob(path + "*" + ext);
		for (auto &csv_file : csv_files) {
			all_tests_passed = all_tests_passed && RunFull(csv_file, skip, add_parameters);
		}
	}
	REQUIRE(all_tests_passed);
}

TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data", "[parallel-csv][.]") {
	std::set<std::string> skip;
	// This file requires additional parameters, we test it on the following test.
	skip.insert("test/sql/copy/csv/data/no_quote.csv");
	RunTestOnFolder("test/sql/copy/csv/data/", &skip);
}

//! Test case with specific parameters that allow us to run the no_quote.tsv we were skipping
TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/no_quote.csv", "[parallel-csv][.]") {
	string add_parameters = ",  header=1, quote=''";
	string file = "test/sql/copy/csv/data/no_quote.csv";
	REQUIRE(RunFull(file, nullptr, add_parameters));
}

TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/auto", "[parallel-csv][.]") {
	std::set<std::string> skip;
	// This file requires additional parameters, we test it on the following test.
	skip.insert("test/sql/copy/csv/data/auto/titlebasicsdebug.tsv");
	RunTestOnFolder("test/sql/copy/csv/data/auto/", &skip);
}

//! Test case with specific parameters that allow us to run the titlebasicsdebug.tsv we were skipping
TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/auto/titlebasicsdebug.tsv", "[parallel-csv][.]") {
	string add_parameters = ", nullstr=\'\\N\', sample_size = -1";
	string file = "test/sql/copy/csv/data/auto/titlebasicsdebug.tsv";
	REQUIRE(RunFull(file, nullptr, add_parameters));
}

TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/auto/glob", "[parallel-csv][.]") {
	RunTestOnFolder("test/sql/copy/csv/data/auto/glob/");
}

TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/error/date_multiple_file", "[parallel-csv][.]") {
	RunTestOnFolder("test/sql/copy/csv/data/error/date_multiple_file/");
}

TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/glob/a1", "[parallel-csv][.]") {
	RunTestOnFolder("test/sql/copy/csv/data/glob/a1/");
}

TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/glob/a2", "[parallel-csv][.]") {
	RunTestOnFolder("test/sql/copy/csv/data/glob/a2/");
}

TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/glob/a3", "[parallel-csv][.]") {
	RunTestOnFolder("test/sql/copy/csv/data/glob/a3/");
}

TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/glob/empty", "[parallel-csv][.]") {
	RunTestOnFolder("test/sql/copy/csv/data/glob/empty/");
}

TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/glob/i1", "[parallel-csv][.]") {
	RunTestOnFolder("test/sql/copy/csv/data/glob/i1/");
}

TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/real", "[parallel-csv][.]") {
	std::set<std::string> skip;
	// This file requires a temp_dir for offloading
	skip.insert("test/sql/copy/csv/data/real/tmp2013-06-15.csv.gz");
	RunTestOnFolder("test/sql/copy/csv/data/real/", &skip);
}

TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/test", "[parallel-csv][.]") {
	std::set<std::string> skip;
	// This file requires additional parameters, we test it on the following test.
	skip.insert("test/sql/copy/csv/data/test/5438.csv");
	RunTestOnFolder("test/sql/copy/csv/data/test/", &skip);
}

//! Test case with specific parameters that allow us to run the titlebasicsdebug.tsv we were skipping
TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/test/5438.csv", "[parallel-csv][.]") {
	string add_parameters = ", delim=\'\', columns={\'j\': \'JSON\'}";
	string file = "test/sql/copy/csv/data/test/5438.csv";
	REQUIRE(RunFull(file, nullptr, add_parameters));
}

TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/zstd", "[parallel-csv][.]") {
	RunTestOnFolder("test/sql/copy/csv/data/zstd/");
}

TEST_CASE("Test Parallel CSV All Files - data/csv", "[parallel-csv][.]") {
	std::set<std::string> skip;
	// This file is too big, executing on it is slow and unreliable
	skip.insert("data/csv/sequences.csv.gz");
	// This file requires specific parameters
	skip.insert("data/csv/bug_7578.csv");
	// This file requires a temp_dir for offloading
	skip.insert("data/csv/hebere.csv.gz");
	RunTestOnFolder("data/csv/", &skip);
}

//! Test case with specific parameters that allow us to run the bug_7578.csv we were skipping
TEST_CASE("Test Parallel CSV All Files - data/csv/bug_7578.csv", "[parallel-csv][.]") {
	string add_parameters = ", delim=\'\\t\', header=true, quote = \'`\', columns={ \'transaction_id\': \'VARCHAR\', "
	                        "\'team_id\': \'INT\', \'direction\': \'INT\', \'amount\':\'DOUBLE\', "
	                        "\'account_id\':\'INT\', \'transaction_date\':\'DATE\', \'recorded_date\':\'DATE\', "
	                        "\'tags.transaction_id\':\'VARCHAR\', \'tags.team_id\':\'INT\', \'tags\':\'varchar\'}";
	string file = "data/csv/bug_7578.csv";
	REQUIRE(RunFull(file, nullptr, add_parameters));
}

TEST_CASE("Test Parallel CSV All Files - data/csv/decimal_separators", "[parallel-csv][.]") {
	RunTestOnFolder("data/csv/decimal_separators/");
}

TEST_CASE("Test Parallel CSV All Files - data/csv/中文", "[parallel-csv][.]") {
	RunTestOnFolder("data/csv/中文/");
}

TEST_CASE("Test Parallel CSV All Files - test/sql/copy/csv/data/abac", "[parallel-csv][.]") {
	RunTestOnFolder("test/sql/copy/csv/data/abac/");
}

TEST_CASE("Test Parallel CSV All Files - test/sqlserver/data", "[parallel-csv][.]") {
	std::set<std::string> skip;
	// This file is too big, executing on it is slow and unreliable
	skip.insert("test/sqlserver/data/Person.csv.gz");
	RunTestOnFolder("test/sqlserver/data/", &skip);
}

//! Test case with specific parameters that allow us to run the Person.tsv we were skipping
TEST_CASE("Test Parallel CSV All Files - test/sqlserver/data/Person.csv.gz", "[parallel-csv][.]") {
	string add_parameters = ", delim=\'|\', quote=\'*\'";
	string file = "test/sqlserver/data/Person.csv.gz";
	REQUIRE(RunFull(file, nullptr, add_parameters));
}

//! Test case with specific files that require a temp_dir for offloading
TEST_CASE("Test Parallel CSV All Files - Temp Dir for Offloading", "[parallel-csv][.]") {
	string file = "test/sql/copy/csv/data/real/tmp2013-06-15.csv.gz";
	REQUIRE(RunFull(file, nullptr, "", true));

	file = "data/csv/hebere.csv.gz";
	REQUIRE(RunFull(file, nullptr, "", true));
}
