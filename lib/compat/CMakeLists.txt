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

project(compat C)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "San")

include(CheckIncludeFile)
check_include_file(inttypes.h HAS_INTTYPES_H)
check_include_file(stdbool.h HAS_STDBOOL_H)
check_include_file(stdint.h HAS_STDINT_H)
check_include_file(string.h HAS_STRING_H)
check_include_file(strings.h HAS_STRINGS_H)
check_include_file(sys/types.h HAS_SYS_TYPES_H)

include(CheckFunctionExists)
check_function_exists(bzero HAS_BZERO)
check_function_exists(memcpy HAS_MEMCPY)
check_function_exists(memmove HAS_MEMMOVE)
check_function_exists(memset HAS_MEMSET)
check_function_exists(strlcpy HAS_STRLCPY)
check_function_exists(strncpy HAS_STRNCPY)
check_function_exists(strnlen HAS_STRNLEN)

set(compat_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(compat_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(compat_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)
set(compat_CONF_IN ${compat_INC_DIR}/compat/conf.h.in)
set(compat_CONF_OUT ${CMAKE_BINARY_DIR}/include/compat/conf.h)

configure_file(${compat_CONF_IN} ${compat_CONF_OUT} @ONLY)

set(compat_HDRS ${compat_CONF_OUT})
file(GLOB_RECURSE compat_HDRS ${compat_HDRS} ${compat_INC_DIR}/*.h)
file(GLOB_RECURSE compat_SRCS ${compat_SRCS} ${compat_SRC_DIR}/*.c)

add_library(compat ${compat_SRCS} ${compat_HDRS})

set_property(TARGET compat PROPERTY C_STANDARD 99)

target_include_directories(compat
  PUBLIC ${compat_INC_DIR} ${CMAKE_BINARY_DIR}/include)
target_compile_definitions(compat PRIVATE COMPILE)

if (MSVC)
  target_compile_options(compat
    PUBLIC /MT$<$<CONFIG:Debug>:d> /Oy /W3
    PUBLIC /D_CRT_SECURE_NO_WARNINGS)
  if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(compat PUBLIC /Ox)
  else ()
    target_compile_options(compat PUBLIC /Od)
  endif ()
  foreach (flag_var
    CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if (${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif ()
  endforeach ()
else ()
  target_compile_options(compat
    PUBLIC -Wall -Werror -Wextra
    PUBLIC -ansi -fomit-frame-pointer)
  if (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(compat PUBLIC -O3)
  else ()
    target_compile_options(compat PUBLIC -O0 -g3)
    if (CMAKE_BUILD_TYPE MATCHES San)
      target_link_libraries(compat
        PUBLIC "-fsanitize=address -fno-omit-frame-pointer")
      target_compile_options(compat PUBLIC "-fsanitize=address")
    endif ()
  endif ()
endif ()

option(COMPAT_CHECK "Enable compat testing" ON)

if (COMPAT_CHECK AND EXISTS ${compat_TEST_DIR})
  if (NOT TARGET check)
    enable_testing()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --timeout 2)
  endif ()
  add_subdirectory(${compat_TEST_DIR})
endif ()
