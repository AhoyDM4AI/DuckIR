#include "duckdb/runtime/default/parallel/thread_context.hpp"
#include "duckdb/runtime/default/execution/execution_context.hpp"
#include "duckdb/main/client_context.hpp"

namespace duckdb {

ThreadContext::ThreadContext(ClientContext &context) : profiler(QueryProfiler::Get(context).IsEnabled()) {
}

} // namespace duckdb
