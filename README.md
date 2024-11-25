# Maths

C++20 Math header-only library for game engines, extending the standard library and providing functions for animation and rendering. 
Supports Clang 14 & CL

## Project Health


[![CMake on multiple platforms](https://github.com/paulbaudy/cpp-maths/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/paulbaudy/cpp-maths/actions/workflows/cmake-multi-platform.yml)


## Build and Install

* Clone the repository
```shell
git clone https://github.com/paulbaudy/cpp-maths.git --recursive
```
Do include the `--recursive` option if you need the glm submodule

* Build using CMake

```shell
cd /path/to/repository
cmake \
    -B build .

cmake --build build -- all
cmake --build build -- install
```
* Or add this repository as a subdirectory in your CMakeLists file :
  
```cmake
add_subdirectory("/path/to/repository")
```

## Release notes

### cpp-maths 0.0.1 - Work in progress

### Features : 
- First release of the library
