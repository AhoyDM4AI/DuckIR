//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/storage/statistics/segment_statistics.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "base_statistics.hpp"
#include "duckdb/common/common.hpp"
#include "duckdb/common/types.hpp"

namespace duckdb {

class SegmentStatistics {
public:
	SegmentStatistics(LogicalType type);
	SegmentStatistics(BaseStatistics statistics);

	//! Type-specific statistics of the segment
	BaseStatistics statistics;
};

} // namespace duckdb
