#ifndef _JAYL_FUNC_H
#define _JAYL_FUNC_H

#include "storage.h"
#include "var.h"
#include "environment.h"

namespace jayl {
  namespace ir {
    class Stmt;

    class IRVisitorStrict;

    struct FuncContent {
      int kind;
      std::string name;
      std::vector<Var> arguments;
      std::vector<Var> results;
      Environment env;

      Stmt *body = nullptr; // Stored as a pointer so it can be forward declared.

      // TODO: Remove storage from Func
      Storage storage;

      ~FuncContent();

      mutable long ref = 0;

      friend inline void aquire(FuncContent *c) { ++c->ref; }

      friend inline void release(FuncContent *c) { if (--c->ref == 0) delete c; }
    };

/// A Jayl Func, which can be passed to the backend to get a runnable Function.
    class Func : public util::IntrusivePtr<FuncContent> {
      public:
      enum Kind {
        Internal, External, Intrinsic
      };

      /// Create an undefined Function
      Func() : IntrusivePtr() {}

      /// Create a function declaration.
      Func(const std::string &name, const std::vector<Var> &arguments,
        const std::vector<Var> &results, Kind kind);

      /// Create a function definition with an environment.
      Func(const std::string &name, const std::vector<Var> &arguments,
        const std::vector<Var> &results, const Stmt &body,
        const Environment &environment, Kind kind = Internal);

      /// Create a function definition.
      Func(const std::string &name, const std::vector<Var> &arguments,
        const std::vector<Var> &results, const Stmt &body, Kind kind = Internal);

      /// Creates a new func with the same prototype as the given func, but with
      /// a new body
      Func(const Func &func, Stmt body);

      /// Creates a new func identical to an old func, except for a different
      /// environment. That is, create a closure around func.
      Func(const Func &func, const Environment &environment);

      std::string getName() const { return ptr->name; }

      const std::vector<Var> &getArguments() const { return ptr->arguments; }

      const std::vector<Var> &getResults() const { return ptr->results; }

      Stmt getBody() const;

      /// Get the function kind, which can be Internal, Intrinsic or External.
      Func::Kind getKind() const { return static_cast<Kind>(ptr->kind); }

      /// Retrieve the function's environment
      const Environment &getEnvironment() const { return ptr->env; }

      Environment &getEnvironment() { return ptr->env; }

      /// Set the storage descriptor for the function's local variables.
      void setStorage(const Storage &storage) { ptr->storage = storage; }

      /// Retrieve a storage descriptor for the function's local variables
      Storage &getStorage() { return ptr->storage; }

      /// Retrieve a storage descriptor for the function's local variables
      const Storage &getStorage() const { return ptr->storage; }

      void accept(IRVisitorStrict *visitor) const;
    };

    std::ostream &operator<<(std::ostream &, const Func &);

  }
}
#endif
