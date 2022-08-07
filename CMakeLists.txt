cmake_minimum_required(VERSION 3.10)
project(rv64emu VERSION 0.0.1)

set(CMAKE_CXX_STANDARD            20)
set(CMAKE_CXX_STANDARD_REQUIRED   ON)
set(CMAKE_CXX_EXTENSIONS          OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4141")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4146")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4291")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4227")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4099")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4267")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4477")
    set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" )
elseif(UNIX AND NOT APPLE)
    set(CC   "/usr/bin/gcc")
    set(CXX  "/usr/bin/g++")
elseif(APPLE)
    set(CC   "/usr/bin/clang")
    set(CXX  "/usr/bin/clang++")
endif()

# Integrate with google test
add_subdirectory(thirdparty/googletest)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

#get_filename_component(ProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
#string(REPLACE " " "_" ProjectName ${ProjectName})
set(SOURCE_FILES
    # Headers
    ${CMAKE_SOURCE_DIR}/source/include/common.h
    ${CMAKE_SOURCE_DIR}/source/include/global.h
    ${CMAKE_SOURCE_DIR}/source/include/cmd.h
    ${CMAKE_SOURCE_DIR}/source/include/elf.h
    ${CMAKE_SOURCE_DIR}/source/include/io.h
    ${CMAKE_SOURCE_DIR}/source/include/riscv.h
    ${CMAKE_SOURCE_DIR}/source/include/state.h
    ${CMAKE_SOURCE_DIR}/source/include/syscall.h
    ${CMAKE_SOURCE_DIR}/source/include/opcode.h
    # Sources
    ${CMAKE_SOURCE_DIR}/source/main.cpp
    ${CMAKE_SOURCE_DIR}/source/global.cpp
    ${CMAKE_SOURCE_DIR}/source/cmd.cpp
    ${CMAKE_SOURCE_DIR}/source/elf.cpp
    ${CMAKE_SOURCE_DIR}/source/io.cpp
    ${CMAKE_SOURCE_DIR}/source/riscv.cpp
    ${CMAKE_SOURCE_DIR}/source/state.cpp
    ${CMAKE_SOURCE_DIR}/source/syscall.cpp
    ${CMAKE_SOURCE_DIR}/source/opcode.cpp
    )


# Integrate with clang-format
include(source/cmake/clang-format.cmake)
clangformat_setup(
    ${SOURCE_FILES}
    )


add_executable(
    ${CMAKE_PROJECT_NAME}
    ${SOURCE_FILES}
    )

target_include_directories(
    ${CMAKE_PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/source/include
    ${CMAKE_SOURCE_DIR}/thirdparty/googletest/googletest/include
    )

target_link_libraries(
    ${CMAKE_PROJECT_NAME} PUBLIC
    gtest)

