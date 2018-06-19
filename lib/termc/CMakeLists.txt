#
# MIT License
#
# Copyright (c) 2016-2018 Abel Lucas <www.github.com/uael>
# Copyright (c) 2016-2018 Guts <www.github.com/guts-lang>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

project(termc C)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "San")

include(CheckIncludeFile)
check_include_file(unistd.h HAS_UNISTD_H)

set(termc_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(termc_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(termc_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)
set(termc_CONF_IN ${termc_INC_DIR}/termc/conf.h.in)
set(termc_CONF_OUT ${CMAKE_BINARY_DIR}/include/termc/conf.h)

configure_file(${termc_CONF_IN} ${termc_CONF_OUT} @ONLY)

set(termc_HDRS ${termc_CONF_OUT})
file(GLOB_RECURSE termc_HDRS ${termc_HDRS} ${termc_INC_DIR}/*.h)
file(GLOB_RECURSE termc_SRCS ${termc_SRCS} ${termc_SRC_DIR}/*.c)

add_library(termc ${termc_HDRS} ${termc_SRCS})

set_property(TARGET termc PROPERTY C_STANDARD 99)

target_include_directories(termc
  PUBLIC ${termc_INC_DIR} ${CMAKE_BINARY_DIR}/include
  PRIVATE ${termc_SRC_DIR})
target_link_libraries(termc PUBLIC Wrap::libwrap)
target_compile_definitions(termc PRIVATE API_COMPILE)

if (MSVC)
  foreach (flag_var
    CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if (${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif ()
  endforeach ()
endif ()

option(termc_CHECK "Enable termc testing" ON)

if (termc_CHECK AND EXISTS ${termc_TEST_DIR})
  if (NOT TARGET check)
    enable_testing()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --timeout 2)
  endif ()
  add_subdirectory(${termc_TEST_DIR})
endif ()
