set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3")
else()
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3 -DDEBUG")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Werror -fomit-frame-pointer -Wno-missing-braces")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-missing-field-initializers -Wno-unused-parameter -Wno-unused-function")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-variable -Wno-unused-value -Wno-unused-result")
endif()

set(jayl_SOURCE_DIRS . internal util)

foreach(dir ${jayl_SOURCE_DIRS})
    file(GLOB jayl_HEADERS ${jayl_HEADERS} ${dir}/*.h)
    file(GLOB jayl_SOURCES ${jayl_SOURCES} ${dir}/*.c)
endforeach()

add_subdirectory(adt)
add_subdirectory(backend)
add_subdirectory(frontend)

set(jayl_INCLUDES ${jayl_INCLUDES} backend frontend)

set(jayl_HEADERS ${jayl_HEADERS})
set(jayl_SOURCES ${jayl_SOURCES})
set(jayl_DEPS ${jayl_DEPS})
set(jayl_LDADD ${jayl_LDADD})
set(jayl_DEFS ${jayl_DEFS})

if(WIN32)
    set(jayl_LDADD ${jayl_LDADD} Shlwapi)
endif()

set(jayl_INCLUDES ${jayl_INCLUDES} .)

include_directories(${jayl_INCLUDES})

add_definitions(${jayl_DEFS})

add_library(jl ${jayl_SOURCES} ${jayl_HEADERS})
add_dependencies(jl ${jayl_DEPS})
target_link_libraries(jl ${jayl_LDADD})

add_executable(${PROJECT_NAME} cli/jay.c)
target_link_libraries(${PROJECT_NAME} jl)
