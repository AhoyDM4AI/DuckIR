//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/runtime/default/common/file_system/filename_pattern.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/runtime/default/common/file_system/file_system.hpp"
#include "duckdb/common/types/uuid.hpp"

namespace duckdb {

class Serializer;
class Deserializer;

class FilenamePattern {
	friend Deserializer;

public:
	FilenamePattern() : _base("data_"), _pos(_base.length()), _uuid(false) {
	}
	~FilenamePattern() {
	}

public:
	void SetFilenamePattern(const string &pattern);
	string CreateFilename(FileSystem &fs, const string &path, const string &extension, idx_t offset) const;

	void Serialize(Serializer &serializer) const;
	static FilenamePattern Deserialize(Deserializer &deserializer);

private:
	string _base;
	idx_t _pos;
	bool _uuid;
};

} // namespace duckdb
