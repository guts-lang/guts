cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
cmake_policy(SET CMP0054 OLD)
cmake_policy(SET CMP0045 OLD)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(LoadLLVM)
include(ProjectDependency)

project(jay C)
project_dependency(il GIT_REPOSITORY https://github.com/uael/il.git)
project_dependency(cute GIT_REPOSITORY https://github.com/uael/cute.git)

set(jay_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(jay_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(jay_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

file(GLOB_RECURSE jay_SRCS ${jay_SRCS} ${jay_SRC_DIR}/*.c)
file(GLOB_RECURSE jay_HDRS ${jay_HDRS} ${jay_INC_DIR}/jay/*.h)
set(jay_HDR ${jay_INC_DIR}/jay.h)

add_library(jay ${jay_SRCS} ${jay_HDRS} ${jay_HDR})
target_include_directories(jay PUBLIC ${jay_INC_DIR})
target_link_libraries(jay PUBLIC il)

get_directory_property(jay_PARENT PARENT_DIRECTORY)
if (NOT ${jay_PARENT})
  set(jay_DEVEL TRUE)
elseif (NOT ${jay_DEVEL})
  set(jay_DEVEL FALSE)
endif ()
if (${jay_DEVEL})
  if (EXISTS ${jay_TEST_DIR})
    if (NOT jay_PARENT)
      enable_testing()
    endif ()

    file(GLOB jay_TEST_SRCS ${jay_TEST_SRCS} ${jay_TEST_DIR}/*.c)
    foreach (jay_TEST_SRC ${jay_TEST_SRCS})
      get_filename_component(jay_TEST_NAME ${jay_TEST_SRC} NAME_WE)
      add_executable(test_${jay_TEST_NAME} ${jay_TEST_SRC})
      add_dependencies(test_${jay_TEST_NAME} jay cute)
      target_link_libraries(test_${jay_TEST_NAME} jay cute)
      add_test(${jay_TEST_NAME} test_${jay_TEST_NAME})
    endforeach ()
  endif ()
endif ()

install(FILES ${jay_HDRS}
  DESTINATION include/jay)
install(FILES ${jay_HDR}
  DESTINATION include)
