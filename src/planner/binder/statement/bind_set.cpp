#include "duckdb/parser/statement/set_statement.hpp"
#include "duckdb/planner/binder.hpp"
#include "duckdb/planner/operator/logical_set.hpp"
#include "duckdb/planner/operator/logical_reset.hpp"
#include "duckdb/runtime/default/execution/expression_executor.hpp"
#include "duckdb/planner/expression_binder/constant_binder.hpp"

namespace duckdb {

BoundStatement Binder::Bind(SetVariableStatement &stmt) {
	BoundStatement result;
	result.types = {LogicalType::BOOLEAN};
	result.names = {"Success"};

	// evaluate the scalar value
	ConstantBinder default_binder(*this, context, "SET value");
	auto bound_value = default_binder.Bind(stmt.value);
	auto value = ExpressionExecutor::EvaluateScalar(context, *bound_value, true);

	result.plan = make_uniq<LogicalSet>(stmt.name, std::move(value), stmt.scope);
	properties.return_type = StatementReturnType::NOTHING;
	return result;
}

BoundStatement Binder::Bind(ResetVariableStatement &stmt) {
	BoundStatement result;
	result.types = {LogicalType::BOOLEAN};
	result.names = {"Success"};

	result.plan = make_uniq<LogicalReset>(stmt.name, stmt.scope);
	properties.return_type = StatementReturnType::NOTHING;
	return result;
}

BoundStatement Binder::Bind(SetStatement &stmt) {
	switch (stmt.set_type) {
	case SetType::SET: {
		auto &set_stmt = stmt.Cast<SetVariableStatement>();
		return Bind(set_stmt);
	}
	case SetType::RESET: {
		auto &set_stmt = stmt.Cast<ResetVariableStatement>();
		return Bind(set_stmt);
	}
	default:
		throw NotImplementedException("Type not implemented for SetType");
	}
}

} // namespace duckdb
