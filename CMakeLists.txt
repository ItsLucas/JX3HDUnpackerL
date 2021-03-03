# CMakeList.txt: JX3HDUnpackerL 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
  
endif()

project ("JX3HDUnpackerL")

include_directories(AFTER SYSTEM "$ENV{VCPKG_ROOT}/installed/x86-windows/include/")
find_package(ZSTD CONFIG REQUIRED)
find_package(CURL CONFIG REQUIRED)

find_path(ZSTD_INCLUDE_DIR zstd.h)
include_directories(${ZSTD_INCLUDE_DIR})
# 将源代码添加到此项目的可执行文件。
add_executable (JX3HDUnpackerL "JX3HDUnpackerL.cpp" "JX3HDUnpackerL.h" "ini.c" "ini.h" "INIReader.cpp" "INIReader.h" "miniz.h" "zdict.h" "zip.c" "zip.h" "zstd.h" "zstd_errors.h")
set_target_properties(JX3HDUnpackerL PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
target_link_libraries(JX3HDUnpackerL PRIVATE CURL::libcurl PRIVATE zstd::libzstd_shared)
# TODO: 如有需要，请添加测试并安装目标。
