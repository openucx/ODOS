# Setup

1. Setup environment variables for building and installation of LLVM
```
export PREFIX=/path/to/install/dir/
export BUILDDIR=/path/to/build/dir/
```
2. Compile LLVM
```
cmake -S llvm-project/llvm                          \
      -B llvm_objdir                                \
      -DCMAKE_INSTALL_PREFIX="$PREFIX"              \
      -DCMAKE_BUILD_TYPE=Release                    \
      -DCMAKE_C_COMPILER="`which clang`"            \
      -DCMAKE_CXX_COMPILER="`which clang++`"        \
      -DCMAKE_EXE_LINKER_FLAGS="$LDFLAGS"           \
      -DLLVM_BUILD_UTILS=OFF                        \
      -DLLVM_ENABLE_PROJECTS="clang"                \
      -DGCC_INSTALL_PREFIX="/usr"                   \
      -DCLANG_ENABLE_ARCMT=OFF                      \
      -DCLANG_ENABLE_STATIC_ANALYZER=OF

cd $BUILDDIR && make -j install
```
3. Setup environment variables for building and installation of OpenMP
   preferably use the same installation directory and different build
   directory this time
```
export PREFIX=/path/to/install/dir/
export BUILDDIR=/path/to/build/dir/
```
4. Compile OpenMP
```
cmake -S llvm-project/openmp                        \
      -B $BUILDDIR                                  \
      -DLLVM_ROOT="$PREFIX"                         \
      -DCMAKE_INSTALL_PREFIX="$PREFIX"              \
      -DCMAKE_BUILD_TYPE=Release                    \
      -DCMAKE_C_COMPILER="$PREFIX/bin/clang"        \
      -DCMAKE_CXX_COMPILER="$PREFIX/bin/clang++"    \
      -DCMAKE_EXE_LINKER_FLAGS="$LDFLAGS"           \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON            \
      -DOPENMP_ENABLE_LIBOMPTARGET_PROFILING=OFF    \
      -DLIBOMP_HAVE_OMPT_SUPPORT=OFF                \
      -DLIBOMP_INSTALL_ALIASES=OFF                  \
      -DLIBOMPTARGET_ENABLE_DEBUG=OFF               \
      -DLLVM_BUILD_TOOLS=ON                         \
      -DLLVM_ENABLE_RUNTIMES=openmp                 \
      -DDOCA_PATH=/path/to/tools/doca/2.0.2/opt/mellanox/doca

cd $BUILDDIR && make -j install
```
5. Compile the DOCA OpenMP Service within DPU:
   `llvm-project/openmp/libomptarget/toos/docaservice/`

# Run
1. Run DOCA OpenMP Service on BlueField DPU
2. Export environment variables on host
```
export BLUEFIELD_SYSROOT=/path/to/tools/gcc-arm/8.3/aarch64-linux-gnu/libc
export BLUEFIELD_GCC_TOOLCHAIN=/path/to/tools/gcc-arm/8.3
```
3. Compile application on host
```
clang -fopenmp -fopenmp-targets=aarch64-unknown-linux ./app.c -o app
```
4. Run
```
./app
```

Note: please make sure that DOCA libraries are included in LD\_LIBRARY\_PATH
