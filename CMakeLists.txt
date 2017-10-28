cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
cmake_policy(SET CMP0054 OLD)
cmake_policy(SET CMP0045 OLD)

include(CMakeParseArguments)

project(guts C)
get_directory_property(guts_PARENT PARENT_DIRECTORY)

set(BUILD_SHARED_LIBS OFF)

set(guts_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(guts_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(guts_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

set(CMAKE_C_FLAGS "")
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_RELEASE "")

option(COVERAGE "Turn on COVERAGE support" OFF)
if (COVERAGE AND NOT MSVC)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")
endif ()

if (NOT guts_PARENT AND EXISTS ${guts_TEST_DIR})
  enable_testing()
  add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
  set(guts_ENABLE_TEST ON)
else ()
  set(guts_ENABLE_TEST OFF)
endif ()

macro (add_module MOD TYPE)
  set(MOD_DIR ${MOD})
  set(EXT c)
  if (ARGN)
    message(STATUS
      "[guts] add module '${MOD}' depends on '${ARGN}'")
  else ()
    message(STATUS "[guts] add module '${MOD}'")
  endif ()

  set(BUILD_SHARED_LIBS OFF)

  if ("${TYPE}" STREQUAL "INTERFACE")
    file(GLOB_RECURSE ${MOD}_HDRS ${${MOD}_HDRS} ${guts_INC_DIR}/${MOD_DIR}/*.h)
    set(${MOD}_HDR ${guts_INC_DIR}/${MOD_DIR}.h)

    add_library(${MOD} INTERFACE)
    target_include_directories(${MOD} INTERFACE ${guts_INC_DIR})

    target_compile_definitions(${MOD}
      INTERFACE $<$<CONFIG:Debug>:DEBUG>$<$<CONFIG:Release>:NDEBUG>)
  elseif ("${TYPE}" STREQUAL "LIBRARY")
    file(GLOB_RECURSE ${MOD}_SRCS ${${MOD}_SRCS}
      ${guts_SRC_DIR}/${MOD_DIR}/*.${EXT})
    file(GLOB_RECURSE ${MOD}_HDRS ${${MOD}_HDRS}
      ${guts_INC_DIR}/${MOD_DIR}/*.h)
    set(${MOD}_HDR ${guts_INC_DIR}/${MOD_DIR}.h)

    add_library(${MOD} ${${MOD}_SRCS} ${${MOD}_HDRS} ${${MOD}_HDR})
    set_property(TARGET ${MOD} PROPERTY C_STANDARD 90)
    target_include_directories(${MOD} PUBLIC ${guts_INC_DIR})
    target_compile_definitions(${MOD} PRIVATE U_COMPILE)

    target_compile_definitions(${MOD}
      PRIVATE $<$<CONFIG:Debug>:DEBUG>$<$<CONFIG:Release>:NDEBUG>
      PRIVATE API_COMPILE)
    if (MSVC)
      target_compile_options(${MOD}
        PRIVATE /MT$<$<CONFIG:Debug>:d> /Oy /W3
        PRIVATE /D_CRT_SECURE_NO_WARNINGS
        PRIVATE /O$<$<CONFIG:Debug>:d>$<$<CONFIG:Release>:x>)
    else ()
      target_compile_options(${MOD}
        PRIVATE -Wall -Werror -Wextra -fomit-frame-pointer -ansi
        PRIVATE -Wno-missing-field-initializers -Wno-missing-braces
        PRIVATE -O$<$<CONFIG:Debug>:0 -g3>$<$<CONFIG:Release>:3>)
    endif ()
  elseif ("${TYPE}" STREQUAL "EXECUTABLE")
    file(GLOB_RECURSE ${MOD}_SRCS ${${MOD}_SRCS}
      ${guts_SRC_DIR}/${MOD_DIR}/*.${EXT})

    add_executable(${MOD} ${${MOD}_SRCS})
    set_property(TARGET ${MOD} PROPERTY C_STANDARD 90)
    target_include_directories(${MOD} PUBLIC ${guts_INC_DIR})
    target_compile_definitions(${MOD} PRIVATE U_COMPILE)

    target_compile_definitions(${MOD}
      PRIVATE $<$<CONFIG:Debug>:DEBUG>$<$<CONFIG:Release>:NDEBUG>
      PRIVATE API_COMPILE)
    if (MSVC)
      target_compile_options(${MOD}
        PRIVATE /MT$<$<CONFIG:Debug>:d> /Oy /W3
        PRIVATE /D_CRT_SECURE_NO_WARNINGS
        PRIVATE /O$<$<CONFIG:Debug>:d>$<$<CONFIG:Release>:x>)
    else ()
      target_compile_options(${MOD}
        PRIVATE -Wall -Werror -Wextra -fomit-frame-pointer -ansi
        PRIVATE -Wno-missing-field-initializers -Wno-missing-braces
        PRIVATE -O$<$<CONFIG:Debug>:0 -g3>$<$<CONFIG:Release>:3>)
    endif ()
  else ()
    message(FATAL_ERROR "Unrecognized module type : ${TYPE}")
  endif ()

  foreach(dep ${ARGN})
    if ("${TYPE}" STREQUAL "INTERFACE")
      target_link_libraries(${MOD} INTERFACE ${dep})
    else ()
      target_link_libraries(${MOD} ${dep})
    endif ()
    add_dependencies(${MOD} ${dep})
  endforeach()

  if (guts_ENABLE_TEST)
    file(GLOB ${MOD}_TEST_SRCS ${${MOD}_TEST_SRCS}
      ${guts_TEST_DIR}/${MOD_DIR}/*.${EXT})
    foreach (${MOD}_TEST_SRC ${${MOD}_TEST_SRCS})
      get_filename_component(${MOD}_TEST_NAME ${${MOD}_TEST_SRC} NAME_WE)
      set(test test_${MOD}_${${MOD}_TEST_NAME})
      add_executable(${test} ${${MOD}_TEST_SRC})
      set_property(TARGET ${test}
        PROPERTY C_STANDARD 90)
      add_dependencies(${test} ${MOD})
      add_dependencies(check ${test})
      target_include_directories(${test} PUBLIC
        ${guts_TEST_DIR})
      target_link_libraries(${test} ${MOD})
      target_compile_definitions(${test}
        PRIVATE $<$<CONFIG:Debug>:DEBUG>$<$<CONFIG:Release>:NDEBUG>)
      add_test(${MOD}_${${MOD}_TEST_NAME} ${test}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
      if (MSVC)
        target_compile_options(${test}
          PRIVATE /MTd /Oy /W3
          PRIVATE /D_CRT_SECURE_NO_WARNINGS
          PRIVATE /Od)
      else ()
        target_compile_options(${test}
          PRIVATE -fomit-frame-pointer -ansi
          PRIVATE -O0 -g3)
      endif ()
    endforeach ()
  endif ()
endmacro ()

add_module(nt LIBRARY)
add_module(mem LIBRARY nt)
add_module(ds LIBRARY nt mem)
add_module(ev LIBRARY nt mem ds)
add_module(fs LIBRARY nt mem ds)
add_module(lex LIBRARY nt mem ds fs)
add_module(fe LIBRARY nt mem ds fs lex)
add_module(app LIBRARY nt mem ds)
