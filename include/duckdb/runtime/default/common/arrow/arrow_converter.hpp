//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/runtime/default/common/arrow/arrow_converter.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/runtime/default/common/types/data_chunk.hpp"
#include "duckdb/runtime/default/common/arrow/arrow.hpp"
#include "duckdb/main/client_properties.hpp"

namespace duckdb {

struct ArrowConverter {
	DUCKDB_API static void ToArrowSchema(ArrowSchema *out_schema, const vector<LogicalType> &types,
	                                     const vector<string> &names, const ClientProperties &options);
	DUCKDB_API static void ToArrowArray(DataChunk &input, ArrowArray *out_array, ClientProperties options);
};

} // namespace duckdb
