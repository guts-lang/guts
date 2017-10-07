set(LLVM_KNOWN_VERSIONS
  6.0.0 6.0 5.0.0 5.0 4.0.1 4.0.0 4.0 4 3.9.1 3.9.0 3.9 3.8.1 3.8.0 3.8
  3.7.1 3.7.0 3.7 3.6.2 3.6.1 3.6.0 3.6 3.5.2 3.5.1 3.5.0 3.5 3.4.2 3.4.1
  3.4 3.3 3.2 3.1)
list(APPEND LLVM_ROOT_DIRS
  "C:/Program Files (x86)/LLVM"
  "C:/Program Files/LLVM"
  "C:/LLVM")
foreach (version ${LLVM_KNOWN_VERSIONS})
  string(REPLACE "." "" undotted_version "${version}")
  list(APPEND LLVM_ROOT_DIRS
    "C:/Libraries/llvm-${version}"
    "/usr/lib/llvm-${version}"
    "/opt/local/libexec/llvm-${version}"
    "/usr/local/Cellar/llvm/${version}"
    "/usr/local/lib/llvm-${version}"
    "/usr/local/llvm${undotted_version}"
    "/usr/lib/llvm/${version}")
endforeach()
find_package(CLANG REQUIRED CONFIG PATHS ${LLVM_ROOT_DIRS})
set(CLANG_LIBRARIES
  clangFrontend
  clangDriver
  clangSerialization
  clangSema
  clangAnalysis
  clangAST
  clangParse
  clangSema
  clangBasic
  clangEdit
  clangLex)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CLANG
  DEFAULT_MSG CLANG_DIR
  CLANG_INCLUDE_DIRS CLANG_LIBRARIES)
mark_as_advanced(
  CLANG_DIR
  CLANG_INCLUDE_DIRS CLANG_LIBRARIES)
