# Maths



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
