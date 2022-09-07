cmake_minimum_required(VERSION 3.10)
project(rvemu VERSION 0.0.1)

set(CMAKE_CXX_STANDARD            20)
set(CMAKE_CXX_STANDARD_REQUIRED   ON)
set(CMAKE_CXX_EXTENSIONS          OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


#==============================================================================
# Compiler options
#==============================================================================
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


#==============================================================================
# Source files - OS dependent
#==============================================================================
if(CMAKE_HOST_WIN32)
  set(SOURCE_OSDEPS
      ${CMAKE_SOURCE_DIR}/source/osdep/syscall_win.cpp
      )
elseif(CMAKE_HOST_APPLE)
  set(SOURCE_OSDEPS
      ${CMAKE_SOURCE_DIR}/source/osdep/syscall_macos.cpp
      )
elseif(CMAKE_HOST_UNIX)
  set(SOURCE_OSDEPS
    ${CMAKE_SOURCE_DIR}/source/osdep/syscall_linux.cpp
      )
endif()


#==============================================================================
# Source files - Common
#==============================================================================
set(SOURCE_FILES
    # Headers
    ${CMAKE_SOURCE_DIR}/source/include/config.h
    ${CMAKE_SOURCE_DIR}/source/include/cmd.h
    ${CMAKE_SOURCE_DIR}/source/include/elf.h
    ${CMAKE_SOURCE_DIR}/source/include/mem.h
    ${CMAKE_SOURCE_DIR}/source/include/regfile.h
    ${CMAKE_SOURCE_DIR}/source/include/riscv.h
    ${CMAKE_SOURCE_DIR}/source/include/riscv_define.h
    ${CMAKE_SOURCE_DIR}/source/include/decode.h
    ${CMAKE_SOURCE_DIR}/source/include/state.h
    ${CMAKE_SOURCE_DIR}/source/include/syscall.h
    ${CMAKE_SOURCE_DIR}/source/include/record.h

    # Sources
    ${CMAKE_SOURCE_DIR}/source/main.cpp
    ${CMAKE_SOURCE_DIR}/source/config.cpp
    ${CMAKE_SOURCE_DIR}/source/cmd.cpp
    ${CMAKE_SOURCE_DIR}/source/elf.cpp
    ${CMAKE_SOURCE_DIR}/source/elf32.cpp
    ${CMAKE_SOURCE_DIR}/source/mem.cpp
    ${CMAKE_SOURCE_DIR}/source/riscv.cpp
    ${CMAKE_SOURCE_DIR}/source/riscv_rv32c.cpp
    ${CMAKE_SOURCE_DIR}/source/riscv_rv32i.cpp
    ${CMAKE_SOURCE_DIR}/source/riscv_rv32f.cpp
    ${CMAKE_SOURCE_DIR}/source/state.cpp
    ${CMAKE_SOURCE_DIR}/source/regfile.cpp
    ${CMAKE_SOURCE_DIR}/source/syscall.cpp
    )


#==============================================================================
# Source files - TestSuite
#==============================================================================
set(SOURCE_TESTS
    # Headers
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_quicktest.h

    # Sources
    #
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_regfile.cpp
    #
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_execute_inst_rv32c_arithmetic.cpp
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_execute_inst_rv32c_branch.cpp
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_execute_inst_rv32c_jump.cpp
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_execute_inst_rv32c_load.cpp
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_execute_inst_rv32c_store.cpp
    #
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_execute_inst_rv32i_arithmetic.cpp
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_execute_inst_rv32i_branch.cpp
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_execute_inst_rv32i_jump.cpp
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_execute_inst_rv32i_load.cpp
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_execute_inst_rv32i_store.cpp
    #
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_decode_inst.cpp
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_decode_inst_rv32i.cpp
    ${CMAKE_SOURCE_DIR}/source/testsuite/test_decode_inst_rv32c.cpp
    )

# Integrate with google test
add_subdirectory(thirdparty/googletest)


#==============================================================================
# Integrate with clang-format
#==============================================================================
include(source/cmake/clang-format.cmake)
clangformat_setup(
    ${SOURCE_FILES}
    ${SOURCE_OSDEPS}
    ${SOURCE_TESTS}
    )


#==============================================================================
#
#==============================================================================
add_executable(
    ${CMAKE_PROJECT_NAME}
    ${SOURCE_FILES}
    ${SOURCE_OSDEPS}
    ${SOURCE_TESTS}
    )

target_include_directories(
    ${CMAKE_PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/source/include
    ${CMAKE_SOURCE_DIR}/thirdparty/googletest/googletest/include
    )

 target_link_libraries(
     ${CMAKE_PROJECT_NAME} PUBLIC
     gtest
     )

# Setup output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

