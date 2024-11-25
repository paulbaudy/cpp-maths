# Maths

C++20 Math library for game engines, extending the standard library and providing functions for animation and rendering

## Project Health


[![CMake on multiple platforms](https://github.com/paulbaudy/cpp-maths/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/paulbaudy/cpp-maths/actions/workflows/cmake-multi-platform.yml)


## Build and Install

Build the glm library before the math library

```shell
cd /path/to/glm
cmake \
    -DGLM_BUILD_TESTS=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -B build .
cmake --build build -- all
cmake --build build -- install
```
## Release notes

### cpp-maths 0.0.1 - Work in progress

### Features : 
- First release of the library
