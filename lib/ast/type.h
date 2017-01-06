/*
 *  MIT License
 *
 * Copyright (c) 2016 .DRY (Don't Repeat Yourself)
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* $Id$ */

#ifndef _AST_TYPE_H
#define _AST_TYPE_H

#include <vector>
#include "node.h"

namespace Jay {
  namespace Ast {
    struct TypeSpecifier : Node {};

    struct type_callable_t : TypeSpecifier {
      TypeSpecifier *type = nullptr;
      TypeSpecifier *args_types = nullptr;

      type_callable_t(TypeSpecifier *type, TypeSpecifier *args_types = nullptr);

      void accept(Node *scope) override;
    };

    struct TypePtr : TypeSpecifier {
      TypeSpecifier *type = nullptr;

      TypePtr(TypeSpecifier *type);

      void accept(Node *scope) override;
    };

    struct TypeArray : TypeSpecifier {
      TypeSpecifier *type = nullptr;
      Expr *fixed_size = nullptr;

      TypeArray(TypeSpecifier *type, Expr *fixed_size = nullptr);

      void accept(Node *scope) override;
    };

    struct Type : TypeSpecifier {};

    struct TypeInternal : Type {
      enum Kind {
        SELF, STATIC, VOID, BOOL, CHAR, INT, UINT, SINT, SHORT, USHORT,
        STRING, SSHORT, FLOAT, UFLOAT, SFLOAT, DOUBLE, UDOUBLE, SDOUBLE
      } kind;

      TypeInternal(Kind kind);
    };

    struct TypeUserdef : Type {
      Identifier *id = nullptr;

      TypeUserdef(Identifier *id);

      void accept(Node *scope) override;
    };

    struct TypeGeneric : TypeUserdef {
      TypeSpecifier *types = nullptr;

      TypeGeneric(Identifier *id, TypeSpecifier *types);

      void accept(Node *scope) override;
    };
  }
}

#endif /* _AST_TYPE_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */