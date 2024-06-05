//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/runtime/default/storage/table/column_segment_tree.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/runtime/default/storage/table/segment_tree.hpp"
#include "duckdb/runtime/default/storage/table/column_segment.hpp"

namespace duckdb {

class ColumnSegmentTree : public SegmentTree<ColumnSegment> {};

} // namespace duckdb
