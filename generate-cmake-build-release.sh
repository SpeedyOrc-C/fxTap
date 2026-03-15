cmake \
  -DCMAKE_BUILD_TYPE=MinSizeRel \
  -DCMAKE_C_COMPILER=~/.local/bin/sh-elf-gcc \
  -DCMAKE_C_FLAGS=-nostdlib \
  -DCMAKE_CXX_COMPILER=~/.local/bin/sh-elf-g++ \
  -DCMAKE_CXX_FLAGS=-nostdlib \
  -DCMAKE_MODULE_PATH=~/.local/lib/cmake/fxsdk \
  -DFXSDK_CMAKE_MODULE_PATH=~/.local/lib/cmake/fxsdk \
  -DCMAKE_TOOLCHAIN_FILE=~/.local/lib/cmake/fxsdk/FX9860G.cmake \
  -S . \
  -B ./cmake-build-fx-9860-release

cp ./cmake-build-fx-9860-release/compile_commands.json .
