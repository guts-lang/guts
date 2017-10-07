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
find_package(LLVM REQUIRED CONFIG PATHS ${LLVM_ROOT_DIRS})
set(LLVM_LINK_COMPONENTS
  Demangle Support TableGen Core IRReader CodeGen SelectionDAG AsmPrinter
  MIRParser GlobalISel BitReader BitWriter TransformUtils
  Instrumentation InstCombine ScalarOpts ipo Vectorize ObjCARCOpts
  Coroutines Linker Analysis MC MCParser MCDisassembler Object
  ObjectYAML Option DebugInfoDWARF DebugInfoMSF DebugInfoCodeView
  DebugInfoPDB Symbolize ExecutionEngine Interpreter MCJIT OrcJIT
  RuntimeDyld Target AsmParser LineEditor ProfileData
  Coverage Passes)
llvm_map_components_to_libnames(LLVM_LIBRARIES ${LLVM_LINK_COMPONENTS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LLVM
  DEFAULT_MSG LLVM_DIR
  LLVM_INCLUDE_DIRS LLVM_LIBRARIES LLVM_DEFINITIONS)
mark_as_advanced(
  LLVM_DIR
  LLVM_INCLUDE_DIRS LLVM_LIBRARIES LLVM_DEFINITIONS)
