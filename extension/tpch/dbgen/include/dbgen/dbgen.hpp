//===----------------------------------------------------------------------===//
//
//                         DuckDB
//
// dbgen.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb.hpp"
#ifndef DUCKDB_AMALGAMATION
#include "duckdb/catalog/catalog.hpp"
#include "duckdb/runtime/default/common/types/data_chunk.hpp"
#endif

namespace duckdb {
class ClientContext;
}

namespace tpch {

struct DBGenWrapper {
	//! Create the TPC-H tables in the given catalog / schema with the given suffix
	static void CreateTPCHSchema(duckdb::ClientContext &context, std::string catalog, std::string schema,
	                             std::string suffix);
	//! Load the TPC-H data of the given scale factor
	static void LoadTPCHData(duckdb::ClientContext &context, double sf, std::string catalog, std::string schema,
	                         std::string suffix, int children, int step);

	//! Gets the specified TPC-H Query number as a string
	static std::string GetQuery(int query);
	//! Returns the CSV answer of a TPC-H query
	static std::string GetAnswer(double sf, int query);
};

} // namespace tpch
