add_library_unity(
  duckdb_planner_expression
  OBJECT
  bound_expression.cpp
  bound_aggregate_expression.cpp
  bound_between_expression.cpp
  bound_case_expression.cpp
  bound_cast_expression.cpp
  bound_columnref_expression.cpp
  bound_comparison_expression.cpp
  bound_conjunction_expression.cpp
  bound_constant_expression.cpp
  bound_function_expression.cpp
  bound_lambda_expression.cpp
  bound_lambdaref_expression.cpp
  bound_operator_expression.cpp
  bound_parameter_expression.cpp
  bound_reference_expression.cpp
  bound_subquery_expression.cpp
  bound_unnest_expression.cpp
  bound_window_expression.cpp
  bound_onnx_expression.cpp
  )

target_link_libraries(duckdb_planner_expression onnx_optimizer onnx_optimizer_c_api onnx onnx_proto)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:duckdb_planner_expression>
    PARENT_SCOPE)
