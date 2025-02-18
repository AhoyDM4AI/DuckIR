//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/runtime/default/parallel/pipeline_finish_event.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/runtime/default/parallel/base_pipeline_event.hpp"

namespace duckdb {
class Executor;

class PipelineFinishEvent : public BasePipelineEvent {
public:
	explicit PipelineFinishEvent(shared_ptr<Pipeline> pipeline);

public:
	void Schedule() override;
	void FinishEvent() override;
};

} // namespace duckdb
