#include "duckdb/runtime/default/execution/operator/scan/physical_dummy_scan.hpp"
#include "duckdb/runtime/default/execution/physical_plan_generator.hpp"
#include "duckdb/planner/operator/logical_prepare.hpp"
#include "duckdb/runtime/default/execution/operator/helper/physical_prepare.hpp"

namespace duckdb {

unique_ptr<PhysicalOperator> PhysicalPlanGenerator::CreatePlan(LogicalPrepare &op) {
	D_ASSERT(op.children.size() <= 1);

	// generate default plan
	if (!op.children.empty()) {
		auto plan = CreatePlan(*op.children[0]);
		op.prepared->types = plan->types;
		op.prepared->plan = std::move(plan);
	}

	return make_uniq<PhysicalPrepare>(op.name, std::move(op.prepared), op.estimated_cardinality);
}

} // namespace duckdb
