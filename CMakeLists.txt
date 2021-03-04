# CMakeList.txt: JX3HDUnpackerL 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

if(UNIX OR APPLE)
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.295.tar.gz"
    SHA1 "8a3447594fa5948cc7c6888dd86a9e823259c69f"
)
endif()

 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
project ("JX3HDUnpackerL")

if(UNIX OR APPLE)
hunter_add_package(CURL)
hunter_add_package(zstd)
find_package(Threads REQUIRED)
endif()
if(WIN32)
find_path(ZSTD_INCLUDE_PATH NAMES zdict.h)
include_directories(${ZSTD_INCLUDE_PATH})
endif()
find_package(CURL CONFIG REQUIRED)
find_package(zstd CONFIG REQUIRED)

# 将源代码添加到此项目的可执行文件。
add_executable (JX3HDUnpackerL "JX3HDUnpackerL.cpp" "JX3HDUnpackerL.h" "ini.c" "ini.h" "INIReader.cpp" "INIReader.h" "miniz.h" "zip.c" "zip.h" "config.h")

target_link_libraries(JX3HDUnpackerL CURL::libcurl zstd::libzstd_shared)
# TODO: 如有需要，请添加测试并安装目标。
