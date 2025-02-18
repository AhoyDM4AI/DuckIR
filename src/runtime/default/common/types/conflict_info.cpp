#include "duckdb/runtime/default/common/types/constraint_conflict_info.hpp"
#include "duckdb/runtime/default/storage/index.hpp"

namespace duckdb {

bool ConflictInfo::ConflictTargetMatches(Index &index) const {
	if (only_check_unique && !index.IsUnique()) {
		// We only support checking ON CONFLICT for Unique/Primary key constraints
		return false;
	}
	if (column_ids.empty()) {
		return true;
	}
	// Check whether the column ids match
	return column_ids == index.column_id_set;
}

} // namespace duckdb
