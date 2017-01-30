#ifndef _JAYL_BACKEND_H
#define _JAYL_BACKEND_H

#include <vector>
#include <string>
#include "interfaces/uncopyable.h"

namespace jayl {
  namespace ir {
    class Func;

    class Environment;

    class Stmt;

    class Var;

    class Storage;
  }
  namespace backend {
    class Function;

    class BackendImpl;

/// Code generators are used to turn Jayl IR into some other representation.
/// Examples include LLVM IR, compiled machine code and Graphviz .dot files.
    class Backend : jayl::interfaces::Uncopyable {
      public:
      Backend(const std::string &type);

      ~Backend();

      /// Compiles an IR function to a runable function.
      backend::Function *compile(const ir::Func &func);

      /// Compiles an IR function to a runable function with a storage descriptor.
      /// The storage descriptor describes the storage layout of tensors.
      backend::Function *compile(const ir::Func &func, const ir::Storage &storage);

      /// Compiles an IR statement to a runable function. Any undefined variable
      /// becomes part of the runable function's environment and must be bound
      /// before the function is run.
      backend::Function *compile(const ir::Stmt &stmt, const ir::Environment &env);

      /// Compiles an IR statement to a runable function with a storage descriptor.
      /// The storage descriptor describes the storage layout of tensors. Any
      /// undefined variable becomes part of the runable function's environment and
      /// must be bound before the function is run.
      backend::Function *compile(const ir::Stmt &stmt, const ir::Environment &env,
        const ir::Storage &storage);

      /// Compiles an IR statement to a runable function. The output variables and
      /// any undefined variable becomes part of the runable function's environment
      /// and must be bound before the function is run.
      /// TODO DEPRECATED: This function is deprecated and should be removed.
      ///                  Remember to also delete Var forward decl.
      backend::Function *compile(const ir::Stmt &stmt, std::vector<ir::Var> output);

      protected:
      BackendImpl *pimpl;
    };

  }
}
#endif
