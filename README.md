# tsor

MPEG Transport Stream Inspector

[![Build with CMake](https://github.com/snesb/tsor/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/snesb/tsor/actions/workflows/build.yml)

## Building
```bash
# Install deps and get tsor sources
sudo apt install libglfw3-dev libopengl-dev
git clone --recurse-submodules https://github.com/snesb/tsor
```

```bash
# Setup build environment
cd tsor
mkdir build
cd build
cmake ..
```

```bash
# Make the entire tsor project
make -j$(nproc)
./tsor --help
```

```bash
# Build only libts as a static library
make ts
file build/src/ts/libts.a
```

```bash
# Build only libts as a shared library
cmake .. -DBUILD_SHARED_LIBS=ON
make ts
file build/src/ts/libts.so
```
