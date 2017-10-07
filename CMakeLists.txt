cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
cmake_policy(SET CMP0054 OLD)
cmake_policy(SET CMP0045 OLD)

project(guts C CXX)
get_directory_property(guts_PARENT PARENT_DIRECTORY)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(CMakeParseArguments)

find_package(llvm)
include_directories(${LLVM_INCLUDE_DIRS})

find_package(clang)
include_directories(${CLANG_INCLUDE_DIRS})

set(guts_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(guts_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(guts_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_RELEASE "")

macro (add_module MOD TYPE)
  cmake_parse_arguments(PA "CXX" "IN" "DEPS" ${ARGN})
  if (PA_IN)
    set(MOD_DIR ${PA_IN}/${MOD})
  else ()
    set(MOD_DIR ${MOD})
  endif ()
  if (PA_CXX)
    project(${MOD} CXX)
    set(CMAKE_CXX_STANDARD 11)
    set(EXT cc)
  else ()
    project(${MOD} C)
    set(CMAKE_C_STANDARD 99)
    set(EXT c)
  endif ()
  if (PA_DEPS)
    message(STATUS "[guts] add module '${MOD}' depends on '${PA_DEPS}'")
  else ()
    message(STATUS "[guts] add module '${MOD}'")
  endif ()

  set(BUILD_SHARED_LIBS OFF)

  if ("${TYPE}" STREQUAL "INTERFACE")
    file(GLOB_RECURSE ${MOD}_HDRS ${${MOD}_HDRS} ${guts_INC_DIR}/${MOD_DIR}/*.h)
    set(${MOD}_HDR ${guts_INC_DIR}/${MOD_DIR}.h)

    add_library(${MOD} INTERFACE)
    target_include_directories(${MOD} INTERFACE ${guts_INC_DIR})

    if (MSVC)
      target_compile_options(${MOD}
        INTERFACE /MT$<$<CONFIG:Debug>:d> /Oy /Za /W3
        INTERFACE /D_CRT_SECURE_NO_WARNINGS
        INTERFACE /O$<$<CONFIG:Debug>:d>$<$<CONFIG:Release>:x>)
    else ()
      target_compile_options(${MOD}
        INTERFACE -Wall -Werror -Wextra -fomit-frame-pointer -ansi -pedantic
        INTERFACE -Wno-missing-field-initializers
        INTERFACE -O$<$<CONFIG:Debug>:0 -g3>$<$<CONFIG:Release>:3>)
    endif ()
  elseif ("${TYPE}" STREQUAL "LIBRARY")
    file(GLOB_RECURSE ${MOD}_SRCS ${${MOD}_SRCS}
      ${guts_SRC_DIR}/${MOD_DIR}/*.${EXT})
    file(GLOB_RECURSE ${MOD}_HDRS ${${MOD}_HDRS}
      ${guts_INC_DIR}/${MOD_DIR}/*.h)
    set(${MOD}_HDR ${guts_INC_DIR}/${MOD_DIR}.h)

    add_library(${MOD} ${${MOD}_SRCS} ${${MOD}_HDRS} ${${MOD}_HDR})
    target_include_directories(${MOD} PUBLIC ${guts_INC_DIR})
    target_compile_definitions(${MOD} PRIVATE U_COMPILE)

    if (MSVC)
      target_compile_options(${MOD}
        PUBLIC /MT$<$<CONFIG:Debug>:d> /Oy /Za /W3
        PUBLIC /D_CRT_SECURE_NO_WARNINGS
        PUBLIC /O$<$<CONFIG:Debug>:d>$<$<CONFIG:Release>:x>)
    else ()
      target_compile_options(${MOD}
        PUBLIC -Wall -Werror -Wextra -fomit-frame-pointer -ansi -pedantic
        PUBLIC -Wno-missing-field-initializers
        PUBLIC -O$<$<CONFIG:Debug>:0 -g3>$<$<CONFIG:Release>:3>)
    endif ()
  elseif ("${TYPE}" STREQUAL "EXECUTABLE")
    file(GLOB_RECURSE ${MOD}_SRCS ${${MOD}_SRCS}
      ${guts_SRC_DIR}/${MOD_DIR}/*.${EXT})

    add_executable(${MOD} ${${MOD}_SRCS})
    target_include_directories(${MOD} PUBLIC ${guts_INC_DIR})
    target_compile_definitions(${MOD} PRIVATE U_COMPILE)

    if (MSVC)
      target_compile_options(${MOD}
        PRIVATE /MT$<$<CONFIG:Debug>:d> /Oy /Za /W3
        PRIVATE /D_CRT_SECURE_NO_WARNINGS
        PRIVATE /O$<$<CONFIG:Debug>:d>$<$<CONFIG:Release>:x>)
    else ()
      target_compile_options(${MOD}
        PRIVATE -Wall -Werror -Wextra -fomit-frame-pointer -ansi -pedantic
        PRIVATE -Wno-missing-field-initializers
        PRIVATE -O$<$<CONFIG:Debug>:0 -g3>$<$<CONFIG:Release>:3>)
    endif ()
  else ()
    message(FATAL_ERROR "Unrecognized module type : ${TYPE}")
  endif ()

  foreach(dep IN LISTS ${PA_DEPS})
    if ("${TYPE}" STREQUAL "INTERFACE")
      target_link_libraries(${MOD} INTERFACE ${dep})
    else ()
      target_link_libraries(${MOD} ${dep})
    endif ()
    add_dependencies(${MOD} ${dep})
  endforeach()

  if (NOT guts_PARENT)
    enable_testing()

    file(GLOB ${MOD}_TEST_SRCS ${${MOD}_TEST_SRCS}
      ${guts_TEST_DIR}/${MOD_DIR}/*.${EXT})
    foreach (${MOD}_TEST_SRC ${${MOD}_TEST_SRCS})
      get_filename_component(${MOD}_TEST_NAME ${${MOD}_TEST_SRC} NAME_WE)
      add_executable(test_${MOD}_${${MOD}_TEST_NAME} ${${MOD}_TEST_SRC})
      add_dependencies(test_${MOD}_${${MOD}_TEST_NAME} ${MOD})
      target_include_directories(test_${MOD}_${${MOD}_TEST_NAME} PUBLIC
        ${guts_TEST_DIR})
      target_link_libraries(test_${MOD}_${${MOD}_TEST_NAME} ${MOD})
      add_test(${${MOD}_TEST_NAME} test_${${MOD}_TEST_NAME})
    endforeach ()
  endif ()
endmacro ()

add_module(ucc INTERFACE)
add_module(uty INTERFACE DEPS ucc)
add_module(uerr INTERFACE DEPS ucc uty)
add_module(uds INTERFACE DEPS ucc uty uerr)
add_module(uev INTERFACE DEPS ucc uty uerr uds)
add_module(ufs LIBRARY DEPS ucc uty uerr uds)
add_module(ulex LIBRARY DEPS ucc uty uerr uds ufs uev)
add_module(uopt LIBRARY DEPS ucc uty uerr uds)
add_module(cor LIBRARY CXX IN guts)
add_module(m LIBRARY CXX IN guts DEPS cor)
add_module(ds LIBRARY CXX IN guts DEPS cor m)
