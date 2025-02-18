#include "duckdb/runtime/default/execution/physical_plan_generator.hpp"
#include "duckdb/planner/operator/logical_pragma.hpp"

#include "duckdb/runtime/default/execution/operator/helper/physical_pragma.hpp"
namespace duckdb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalPragma &op) {
	return make_uniq<PhysicalPragma>(std::move(op.info), op.estimated_cardinality);
}

} // namespace duckdb
