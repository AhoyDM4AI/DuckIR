//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/runtime/default/common/types/arrow_aux_data.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/runtime/default/common/types/vector/vector_buffer.hpp"
#include "duckdb/runtime/default/common/arrow/arrow_wrapper.hpp"

namespace duckdb {

struct ArrowAuxiliaryData : public VectorAuxiliaryData {
	static constexpr const VectorAuxiliaryDataType TYPE = VectorAuxiliaryDataType::ARROW_AUXILIARY;
	explicit ArrowAuxiliaryData(shared_ptr<ArrowArrayWrapper> arrow_array_p)
	    : VectorAuxiliaryData(VectorAuxiliaryDataType::ARROW_AUXILIARY), arrow_array(std::move(arrow_array_p)) {
	}
	~ArrowAuxiliaryData() override {
	}

	shared_ptr<ArrowArrayWrapper> arrow_array;
};

} // namespace duckdb
