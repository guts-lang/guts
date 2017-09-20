if (NOT COMMAND load_llvm)
  function(load_llvm)
    set(LLVM_KNOWN_VERSIONS
      6.0.0 6.0 5.0.0 5.0 4.0.1 4.0.0 4.0 4 3.9.1 3.9.0 3.9 3.8.1 3.8.0 3.8
      3.7.1 3.7.0 3.7 3.6.2 3.6.1 3.6.0 3.6 3.5.2 3.5.1 3.5.0 3.5 3.4.2 3.4.1
      3.4 3.3 3.2 3.1
    )
    list(APPEND LLVM_ROOT_DIRS
      "C:/Program Files (x86)/LLVM"
      "C:/Program Files/LLVM"
      "C:/LLVM"
    )
    foreach (version ${LLVM_KNOWN_VERSIONS})
      string(REPLACE "." "" undotted_version "${version}")
      list(APPEND LLVM_ROOT_DIRS
        "C:/Libraries/llvm-${version}"
        "/usr/lib/llvm-${version}"
        "/opt/local/libexec/llvm-${version}"
        "/usr/local/Cellar/llvm/${version}"
        "/usr/local/lib/llvm-${version}"
        "/usr/local/llvm${undotted_version}"
        "/usr/lib/llvm/${version}"
      )
    endforeach()
    find_package(LLVM REQUIRED PATHS ${LLVM_ROOT_DIRS})
    find_package(CLANG REQUIRED PATHS ${LLVM_ROOT_DIRS})
    set(LLVM_LINK_COMPONENTS
      Demangle Support TableGen Core IRReader CodeGen SelectionDAG AsmPrinter
      MIRParser GlobalISel BinaryFormat BitReader BitWriter TransformUtils
      Instrumentation InstCombine ScalarOpts ipo Vectorize ObjCARCOpts
      Coroutines Linker Analysis LTO MC MCParser MCDisassembler Object
      ObjectYAML Option DebugInfoDWARF DebugInfoMSF DebugInfoCodeView
      DebugInfoPDB Symbolize ExecutionEngine Interpreter MCJIT OrcJIT
      RuntimeDyld Target AArch64CodeGen AArch64Info AArch64AsmParser
      AArch64Disassembler AArch64AsmPrinter AArch64Desc AArch64Utils
      AMDGPUCodeGen AMDGPUAsmParser AMDGPUAsmPrinter AMDGPUDisassembler
      AMDGPUInfo AMDGPUDesc AMDGPUUtils ARMCodeGen ARMInfo ARMAsmParser
      ARMDisassembler ARMAsmPrinter ARMDesc ARMUtils BPFCodeGen BPFDisassembler
      BPFAsmPrinter BPFInfo BPFDesc HexagonCodeGen HexagonAsmParser HexagonInfo
      HexagonDesc HexagonDisassembler LanaiCodeGen LanaiAsmParser LanaiInfo
      LanaiDesc LanaiAsmPrinter LanaiDisassembler MipsCodeGen MipsAsmPrinter
      MipsDisassembler MipsInfo MipsDesc MipsAsmParser MSP430CodeGen
      MSP430AsmPrinter MSP430Info MSP430Desc NVPTXCodeGen NVPTXInfo
      NVPTXAsmPrinter NVPTXDesc PowerPCCodeGen PowerPCAsmParser
      PowerPCDisassembler PowerPCAsmPrinter PowerPCInfo PowerPCDesc SparcCodeGen
      SparcInfo SparcDesc SparcAsmPrinter SparcAsmParser SparcDisassembler
      SystemZCodeGen SystemZAsmParser SystemZDisassembler SystemZAsmPrinter
      SystemZInfo SystemZDesc X86CodeGen X86AsmParser X86Disassembler
      X86AsmPrinter X86Desc X86Info X86Utils XCoreCodeGen XCoreDisassembler
      XCoreAsmPrinter XCoreInfo XCoreDesc AsmParser LineEditor ProfileData
      Coverage Passes
    )
    llvm_map_components_to_libnames(LLVM_LIBRARIES ${LLVM_LINK_COMPONENTS})
    set(CLANG_LIBRARIES
      clangBasic clangLex clangParse clangAST clangASTMatchers
      clangDynamicASTMatchers clangSema clangCodeGen clangAnalysis clangEdit
      clangRewrite clangARCMigrate clangDriver clangSerialization clangFrontend
      clangRewriteFrontend clangFrontendTool clangToolingCore
      clangToolingRefactor clangToolingASTDiff clangTooling clangIndex
      clangStaticAnalyzerCore clangStaticAnalyzerCheckers
      clangStaticAnalyzerFrontend clangFormat libclang
    )
  endfunction ()
endif ()
