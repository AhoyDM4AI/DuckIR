## DuckIR
Melt a Traditional Database (DuckB) to support ML-based prediction queries.
We call this project as Duck Intermediate Representation (DuckIR) temporarily.

## Key Modules Plan
* A unified logical plan IR combined with DuckDB Logical plan and onnx Graph
* A unified metadata/catalog for ML related info storage
* A default query execution runtime (DuckDB with ONNX Runtime for now)

## Installation
If you want to install and use DuckDB, please see [our website](https://www.duckdb.org) for installation and usage instructions.

## SQL Reference
The [website](https://duckdb.org/docs/sql/introduction) contains a reference of functions and SQL constructs available in DuckDB.

## Development 
For development, DuckDB requires [CMake](https://cmake.org), Python3 and a `C++11` compliant compiler. Run `make` in the root directory to compile the sources. For development, use `make debug` to build a non-optimized debug version. You should run `make unit` and `make allunit` to verify that your version works properly after making changes. To test performance, you can run `BUILD_BENCHMARK=1 BUILD_TPCH=1 make` and then perform several standard benchmarks from the root directory by executing `./build/release/benchmark/benchmark_runner`. The detail of benchmarks is in our [Benchmark Guide](benchmark/README.md).

Please also refer to our [Contribution Guide](CONTRIBUTING.md).

## Build Notes
### About Protobuf
If we install protobuf using system package manager, i.e.,
```bash
apt-get install libprotobuf-dev protobuf-compiler
```
for debian/ubuntu, we need to set `-DONNX_USE_PROTOBUF_SHARED_LIBS=ON` since this dist version compiled protobuf without '-fPIC' option.

This setting is the default for now.
