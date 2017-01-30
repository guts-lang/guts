#ifndef _JAYL_INDEXVAR_H
#define _JAYL_INDEXVAR_H

#include "intrusive_ptr.h"
#include "domain.h"
#include "reduction.h"

namespace jayl {
  namespace ir {

// namespace { // hiding disabled for now - GCC hates this, TODO: migrate inside *.cc?
    struct IndexVarContent {
      std::string name;
      IndexDomain domain;
      ReductionOperator rop;
      Expr *fixedExpr;
      int kind;

      ~IndexVarContent();

      mutable long ref = 0;

      friend inline void aquire(IndexVarContent *c) { ++c->ref; }

      friend inline void release(IndexVarContent *c) { if (--c->ref == 0) delete c; }
    };
// }

/// An index variable describes iteration over an index set.  There are two
/// types of index variables, free index variables and reduction index
/// variables and both types are represented by the IndexVar class.
///
/// Free index variables simply describe iteration across an index set and do
/// not have a reduction operation (op=FREE).
///
/// Reduction variables have an associated reduction operation that is
/// performed for each index in the index set.  Examples are SUM, which not
/// surprisingly sums over the index variable (\sum_{i} in latex speak) and
/// product which takes the product over the index variable (\prod_{i}).
    class IndexVar : public util::IntrusivePtr<IndexVarContent> {
      public:
      /// The kind of an index variable.
      enum Kind {
        Free, Reduction, Fixed
      };

      /// Construct an undefined index variable.
      IndexVar() : IntrusivePtr() {}

      /// Construct a free index variable.
      IndexVar(std::string name, IndexDomain domain)
        : IntrusivePtr(new IndexVarContent) {
        ptr->name = name;
        ptr->domain = domain;
        ptr->rop = ReductionOperator::Undefined;
        ptr->kind = Free;
        ptr->fixedExpr = nullptr;
      }

      // Construct a reduction index variable.
      IndexVar(std::string name, IndexDomain domain, ReductionOperator rop)
        : IntrusivePtr(new IndexVarContent) {
        ptr->name = name;
        ptr->domain = domain;
        ptr->rop = rop;
        ptr->kind = Reduction;
        ptr->fixedExpr = nullptr;
      }

      // Construct a fixed index variable
      IndexVar(std::string name, IndexDomain domain, Expr *expr)
        : IntrusivePtr(new IndexVarContent) {
        ptr->name = name;
        ptr->kind = Fixed;
        ptr->fixedExpr = expr;
        ptr->domain = domain;
      }


      std::string getName() const {
        iassert(defined()) << "Undefined IndexVar";
        return ptr->name;
      }

      const IndexDomain &getDomain() const {
        iassert(defined()) << "Undefined IndexVar";
        return ptr->domain;
      }

      Expr *getFixedExpr() const {
        iassert(defined()) << "Undefined IndexVar";
        return ptr->fixedExpr;
      }

      size_t getNumBlockLevels() const {
        iassert(defined()) << "Undefined IndexVar";
        return ptr->domain.getNumIndexSets();
      }

      bool isFreeVar() const {
        iassert(defined()) << "Undefined IndexVar";
        return (ptr->kind == Free);
      }

      bool isReductionVar() const {
        iassert(defined()) << "Undefined IndexVar";
        return (ptr->kind == Reduction);
      }

      bool isFixed() const {
        iassert(defined()) << "Undefined IndexVar";
        return (ptr->kind == Fixed);
      }

      ReductionOperator getOperator() const {
        iassert(defined()) << "Undefined IndexVar";
        return ptr->rop;
      }
    };

    std::ostream &operator<<(std::ostream &os, const IndexVar &);

  }
} // namespace jayl::ir

#endif
