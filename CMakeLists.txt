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

project(guts C)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "San")

set(gutc_LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/lib)
set(gutc_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(gutc_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

option(GUTC_CHECK "Enable gutc testing" ON)
option(COVERAGE "Enable coverage" OFF)

if (COVERAGE AND NOT MSVC)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")
endif ()

if (GUTC_CHECK AND EXISTS ${gutc_TEST_DIR})
  if (NOT TARGET check)
    enable_testing()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --timeout 2)
  endif ()
  add_subdirectory(${gutc_TEST_DIR})
endif ()

add_subdirectory(${gutc_LIB_DIR})
add_subdirectory(${gutc_SRC_DIR})
