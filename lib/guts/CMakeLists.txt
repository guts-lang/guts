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

include(CheckIncludeFile)
include(CheckFunctionExists)

## ************************************************************************** ##
# Options
##
option(GUTS_CHECK    "Enable libguts testing" ON)
option(GUTS_COVERAGE "Enable libguts coverage" OFF)
option(GUTS_ASAN     "Enable libguts address sanitizer" OFF)

## ************************************************************************** ##
# Dependencies
##
if (NOT TARGET Wrap::libwrap)
  find_package(Wrap 0.0.1 REQUIRED COMPONENT libwrap)
endif()
if (NOT TARGET Termcolor::libtermcolor)
  find_package(Termcolor 0.0.1 REQUIRED COMPONENT libtermcolor)
endif()
if (NOT TARGET DS::libds)
  find_package(DS 0.0.1 REQUIRED COMPONENT libds)
endif()
if (NOT TARGET IL::libil)
  find_package(IL 0.0.1 REQUIRED COMPONENT libil)
endif()

## ************************************************************************** ##
# Project directories definition
##
set(__INC_DIR  ${CMAKE_CURRENT_LIST_DIR}/include)
set(__SRC_DIR  ${CMAKE_CURRENT_LIST_DIR}/src)
set(__TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

## ************************************************************************** ##
# Targets
##
set(__TARGET_NAME guts)
set(__TARGET lib${__TARGET_NAME})
list(APPEND TARS ${__TARGET})

set(CONF_H ${CMAKE_BINARY_DIR}/include/${__TARGET_NAME}/conf.h)
configure_file(${__INC_DIR}/${__TARGET_NAME}/conf.h.in ${CONF_H} @ONLY)

file(GLOB_RECURSE __HDRS ${CONF_H} ${__INC_DIR}/*.h)

add_library(${__TARGET} ${__HDRS}
  ${__SRC_DIR}/hir/expr.c
  ${__SRC_DIR}/hir/lexer.c
  ${__SRC_DIR}/hir/literal.c
  ${__SRC_DIR}/hir/parser.c
  ${__SRC_DIR}/hir/token.c
  ${__SRC_DIR}/hir/type.c
  ${__SRC_DIR}/hir.c)
add_library(Guts::${__TARGET} ALIAS ${__TARGET})
set_target_properties(${__TARGET} PROPERTIES
  C_STANDARD 90 OUTPUT_NAME "${__TARGET_NAME}")

target_link_libraries(${__TARGET}
  PUBLIC
    Wrap::libwrap
    Termcolor::libtermcolor
    DS::libds
    IL::libil)
target_compile_definitions(${__TARGET} PRIVATE API_COMPILE)
target_include_directories(${__TARGET}
  PRIVATE ${__SRC_DIR}
  PUBLIC
    $<BUILD_INTERFACE:${__INC_DIR}>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>)
if (MSVC)
  target_compile_definitions(${__TARGET} PRIVATE _CRT_SECURE_NO_WARNINGS)
  target_compile_options(${__TARGET} PRIVATE /Oy /W3)
else ()
  target_compile_options(${__TARGET}
    PRIVATE
      -Wall -Werror -Wextra
      -ansi -fomit-frame-pointer
      -Wno-missing-field-initializers
      $<$<BOOL:${GUTS_COVERAGE}>:--coverage>
      $<$<BOOL:${GUTS_ASAN}>:-fsanitize=address>)
  target_link_libraries(${__TARGET}
    PRIVATE
      $<$<BOOL:${GUTS_ASAN}>:-fsanitize=address>
      $<$<BOOL:${GUTS_ASAN}>:-fno-omit-frame-pointer>)
endif ()

## ************************************************************************** ##
# Testing
##
if (GUTS_CHECK AND EXISTS ${__TEST_DIR})
  if (NOT TARGET check)
    enable_testing()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --timeout 2)
  endif ()
  add_subdirectory(${__TEST_DIR})
endif ()
