#include "ir_transforms.h"

#include "ir_rewriter.h"

using namespace std;

namespace jayl {
  namespace ir {

    Func insertVarDecls(Func func) {
      class InsertVarDeclsRewriter : public IRRewriter {
        /// The set of variables that have already been declared. We do not need a
        /// scoped map here because Vars are compared by identity and cannot shadow.
        set<Var> declared;

        void visit(const Func *f) {
          declared.clear();

          for (auto &argument : f->getArguments()) {
            declared.insert(argument);
          }

          for (auto &result : f->getResults()) {
            declared.insert(result);
          }

          for (auto &constant : f->getEnvironment().getConstants()) {
            declared.insert(constant.first);
          }

          for (auto &temp : f->getEnvironment().getTemporaries()) {
            declared.insert(temp);
          }

          IRRewriter::visit(f);
        }

        void visit(const VarDecl *op) {
          declared.insert(op->var);
          stmt = op;
        }

        void visit(const AssignStmt *op) {
          if (!util::contains(declared, op->var)) {
            stmt = Block::make(VarDecl::make(op->var), op);
            declared.insert(op->var);
          } else {
            stmt = op;
          }
        }

        void visit(const Map *op) {
          stmt = op;
          for (auto &var : op->vars) {
            if (!util::contains(declared, var)) {
              stmt = Block::make(VarDecl::make(var), stmt);
              declared.insert(var);
            }
          }
        }

        void visit(const CallStmt *op) {
          stmt = op;
          for (auto &var : op->results) {
            if (!util::contains(declared, var)) {
              stmt = Block::make(VarDecl::make(var), stmt);
              declared.insert(var);
            }
          }
        }
      };
      return InsertVarDeclsRewriter().rewrite(func);
    }

    std::pair<Stmt, std::vector<Stmt>> removeVarDecls(Stmt stmt) {
      class RemoveVarDeclsRewriter : public IRRewriter {
        public:
        std::vector<Stmt> varDecls;

        void visit(const VarDecl *op) {
          varDecls.push_back(op);
          stmt = Stmt();
        }
      };
      RemoveVarDeclsRewriter rewriter;

      Stmt result = rewriter.rewrite(stmt);
      return std::pair<Stmt, vector<Stmt>>(result, rewriter.varDecls);
    }

    Stmt moveVarDeclsToFront(Stmt stmt) {
      std::pair<Stmt, vector<Stmt>> varDecls = removeVarDecls(stmt);
      return (varDecls.second.size() > 0)
        ? Block::make(Block::make(varDecls.second), varDecls.first)
        : varDecls.first;
    }

    Func makeSystemTensorsGlobal(Func func) {
      class MakeSystemTensorsGlobalRewriter : public IRRewriter {
        Environment environment;

        void visit(const Func *f) {
          environment = f->getEnvironment();

          Stmt body = rewrite(f->getBody());
          if (body != f->getBody()) {
            func = Func(f->getName(), f->getArguments(), f->getResults(), body,
              environment);
            func.setStorage(f->getStorage());
          } else {
            func = *f;
          }
        }

        void visit(const VarDecl *op) {
          if (isSystemTensorType(op->var.getType())) {
            environment.addTemporary(op->var);
          } else {
            stmt = op;
          }
        }
      };
      return MakeSystemTensorsGlobalRewriter().rewrite(func);
    }

  }
}
