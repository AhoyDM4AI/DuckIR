//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/runtime/default/execution/base_aggregate_hashtable.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/common.hpp"
#include "duckdb/runtime/default/common/types/row/tuple_data_layout.hpp"
#include "duckdb/runtime/default/common/types/vector/vector.hpp"
#include "duckdb/runtime/default/execution/operator/aggregate/aggregate_object.hpp"

namespace duckdb {
class BufferManager;

class BaseAggregateHashTable {
public:
	BaseAggregateHashTable(ClientContext &context, Allocator &allocator, const vector<AggregateObject> &aggregates,
	                       vector<LogicalType> payload_types);
	virtual ~BaseAggregateHashTable() {
	}

protected:
	Allocator &allocator;
	BufferManager &buffer_manager;
	//! A helper for managing offsets into the data buffers
	TupleDataLayout layout;
	//! The types of the payload columns stored in the hashtable
	vector<LogicalType> payload_types;
	//! Intermediate structures and data for aggregate filters
	AggregateFilterDataSet filter_set;
};

} // namespace duckdb
