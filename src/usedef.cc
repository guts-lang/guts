#include "usedef.h"

namespace jayl {
  namespace ir {

// class VarDef
    std::ostream &operator<<(std::ostream &os, const VarDef &vd) {
      switch (vd.getKind()) {
        case VarDef::Argument:
          os << "arg";
          break;
        case VarDef::Result:
          os << "res";
          break;
        case VarDef::Assignment:
          os << "assign";
          break;
        case VarDef::Map:
          os << "map";
          break;
        case VarDef::Undefined:
          os << "undefined";
          break;
      }
      return os;
    }


// class UseDef
    std::ostream &operator<<(std::ostream &os, const UseDef &ud) {
      os << "UseDef: ";
      auto it = ud.begin();
      if (it != ud.end()) {
        os << *it << ":" << ud.getDef(*it);
        ++it;
      }
      while (it != ud.end()) {
        os << ", " << *it << ":" << ud.getDef(*it);
        ++it;
      }
      return os;
    }

  }
} // namespace jayl::ir
