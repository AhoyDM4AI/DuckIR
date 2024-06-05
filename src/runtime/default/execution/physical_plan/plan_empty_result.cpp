#include "duckdb/runtime/default/execution/operator/scan/physical_empty_result.hpp"
#include "duckdb/runtime/default/execution/physical_plan_generator.hpp"
#include "duckdb/planner/operator/logical_empty_result.hpp"

namespace duckdb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalEmptyResult &op) {
	D_ASSERT(op.children.size() == 0);
	return make_uniq<PhysicalEmptyResult>(op.types, op.estimated_cardinality);
}

} // namespace duckdb
