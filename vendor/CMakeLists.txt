add_subdirectory(firm)

set(libfirm_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/firm)
set(libfirm_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/firm)

set(jayl_DEPS ${jayl_DEPS} firm)
if(WIN32)
    set(jayl_LDADD ${jayl_LDADD} firm winmm)
elseif(UNIX)
    set(jayl_LDADD ${jayl_LDADD} firm m)
endif()
set(jayl_INCLUDES ${jayl_INCLUDES} ${libfirm_SOURCE_DIR}/include)
set(jayl_INCLUDES ${jayl_INCLUDES} ${libfirm_BINARY_DIR}/gen/include)
set(jayl_INCLUDES ${jayl_INCLUDES} ${libfirm_BINARY_DIR}/gen/include/libfirm)
set(jayl_DEFS ${jayl_DEFS} -DWITH_LIBFIRM)

option(LTALLOC "With ltalloc" OFF)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(LTALLOC 0)
endif()

if(LTALLOC)
    add_subdirectory(ltalloc)

    set(jayl_DEPS ${jayl_DEPS} ltalloc)
    set(jayl_LDADD ${jayl_LDADD} ltalloc)
    set(jayl_INCLUDES ${jayl_INCLUDES} ${CMAKE_CURRENT_LIST_DIR}/ltalloc)
    set(jayl_DEFS ${jayl_DEFS} -DWITH_LTALLOC)
endif()

add_subdirectory(mu)

set(jayl_DEPS ${jayl_DEPS} u)
set(jayl_LDADD ${jayl_LDADD} u)
set(jayl_INCLUDES ${jayl_INCLUDES} ${CMAKE_CURRENT_LIST_DIR}/mu/include)

set(jayl_DEPS ${jayl_DEPS} PARENT_SCOPE)
set(jayl_LDADD ${jayl_LDADD} PARENT_SCOPE)
set(jayl_INCLUDES ${jayl_INCLUDES} PARENT_SCOPE)
set(jayl_DEFS ${jayl_DEFS} PARENT_SCOPE)
