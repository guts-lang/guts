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

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(Termcolor VERSION 0.0.1 LANGUAGES C)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
include(CheckIncludeFile)
include(CheckFunctionExists)
include(CMakePackageConfigHelpers)

## ************************************************************************** ##
# Options
##
option(${PROJECT_NAME}_CHECK    "Enable libwrap testing" ON)
option(${PROJECT_NAME}_COVERAGE "Enable libwrap coverage" OFF)
option(${PROJECT_NAME}_ASAN     "Enable libwrap address sanitizer" OFF)

## ************************************************************************** ##
# Dependencies
##
if (NOT TARGET Wrap::libwrap)
  find_package(Wrap 0.0.1 REQUIRED)
endif()

## ************************************************************************** ##
# Project directories definition
##
set(__INC_DIR     ${CMAKE_CURRENT_LIST_DIR}/include)
set(__SRC_DIR     ${CMAKE_CURRENT_LIST_DIR}/src)
set(__TEST_DIR    ${CMAKE_CURRENT_LIST_DIR}/test)
set(__INC_INSDIR  ${CMAKE_INSTALL_INCLUDEDIR})
set(__LIB_INSDIR  ${CMAKE_INSTALL_LIBDIR})
set(__CONF_INSDIR ${__LIB_INSDIR}/cmake/${PROJECT_NAME})

## ************************************************************************** ##
# Targets
##
set(__TARGET_NAME termcolor)
set(__TARGET lib${__TARGET_NAME})
list(APPEND TARS ${__TARGET})

check_include_file(unistd.h  HAS_UNISTD_H)

set(CONF_H ${CMAKE_BINARY_DIR}/include/${__TARGET_NAME}/conf.h)
configure_file(${__INC_DIR}/${__TARGET_NAME}/conf.h.in ${CONF_H} @ONLY)

file(GLOB_RECURSE __HDRS ${CONF_H} ${__INC_DIR}/*.h)

add_library(${__TARGET} ${__HDRS}
  ${__SRC_DIR}/color.c
  ${__SRC_DIR}/term.c)
add_library(${PROJECT_NAME}::${__TARGET} ALIAS ${__TARGET})
set_target_properties(${__TARGET} PROPERTIES
  C_STANDARD 90 OUTPUT_NAME "${__TARGET_NAME}")

target_link_libraries(${__TARGET} PUBLIC Wrap::libwrap)
target_compile_definitions(${__TARGET} PRIVATE API_COMPILE)
target_include_directories(${__TARGET}
  PRIVATE ${__SRC_DIR}
  PUBLIC
  $<INSTALL_INTERFACE:${__INC_INSDIR}>
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
    $<$<CONFIG:Debug>:-g3 -O0>
    $<$<BOOL:${${PROJECT_NAME}_COVERAGE}>:--coverage>
    $<$<BOOL:${${PROJECT_NAME}_ASAN}>:-fsanitize=address>)
  target_link_libraries(${__TARGET}
    PRIVATE
    $<$<BOOL:${${PROJECT_NAME}_ASAN}>:-fsanitize=address>
    $<$<BOOL:${${PROJECT_NAME}_ASAN}>:-fno-omit-frame-pointer>)
endif ()

install(FILES ${CONF_H} DESTINATION ${__INC_INSDIR}/${__TARGET_NAME})
install(DIRECTORY ${__INC_DIR}/${__TARGET_NAME} DESTINATION ${__INC_INSDIR}
  FILES_MATCHING PATTERN "*.h")
install(FILES ${__INC_DIR}/${__TARGET_NAME}.h DESTINATION ${__INC_INSDIR})

## ************************************************************************** ##
# Testing
##
if (${PROJECT_NAME}_CHECK AND EXISTS ${__TEST_DIR})
  if (NOT TARGET check)
    enable_testing()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --timeout 2)
  endif ()
  add_subdirectory(${__TEST_DIR})
endif ()

## ************************************************************************** ##
# Installation
##
install(TARGETS ${TARS} EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${__LIB_INSDIR}
  ARCHIVE DESTINATION ${__LIB_INSDIR})
install(EXPORT ${PROJECT_NAME}Targets FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${__CONF_INSDIR})
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion)
configure_package_config_file(
  ${CMAKE_CURRENT_LIST_DIR}/cmake/Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${__CONF_INSDIR})
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${__CONF_INSDIR})
export(EXPORT ${PROJECT_NAME}Targets
  FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::)
export(PACKAGE ${PROJECT_NAME})
