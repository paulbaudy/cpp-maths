# Maths

## Project Health

| Service | Status |
| ------- | ------ |
| [GitHub actions](https://github.com/g-truc/glm/actions)| [![.github/workflows/ci.yml](https://github.com/g-truc/glm/actions/workflows/ci.yml/badge.svg)](https://github.com/g-truc/glm/actions/workflows/ci.yml)

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

Todo
