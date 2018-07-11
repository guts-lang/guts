tuple
  : type
  | tuple ',' type
  ;

tuple_or_empty
  : /* empty */
  | tuple
  ;

path
  : IDENT
  | IDENT '<' tuple '>'
  | path '::' IDENT
  | path '::' IDENT '<' tuple '>'
  ;

type
  : 'i' BYTES # HIR_TY_INT
  | 'u' BYTES # HIR_TY_INT
  | 'f' BYTES # HIR_TY_FLOAT
  | 'usz' # HIR_TY_SIZE
  | 'isz' # HIR_TY_SIZE
  | 'bool' # HIR_TY_BOOL
  | 'char' # HIR_TY_CHAR
  | path # HIR_TY_SYMBOL
  | '<' tuple '>' # HIR_TY_TUPLE
  | '(' tuple_or_empty ')' # HIR_TY_LAMBDA
  | '(' tuple_or_empty ')' ':' type # HIR_TY_LAMBDA
  | '?' type # HIR_TY_NULLABLE
  | '*' type # HIR_TY_PTR
  | '*' CONST type # HIR_TY_PTR
  | '[' type ']' # HIR_TY_SLICE
  | '[' type ';' expr ']' # HIR_TY_ARRAY
  ;

type_clause
  : type
  | type_clause '|' type
  | type_clause '+' type
  ;

ident_list
  : IDENT
  | ident_list ',' IDENT
  ;

generic
  : IDENT
  | IDENT ':' type_clause
  ;
  
generic_list
  : generic
  | generic_list ',' generic
  ;  

symbol
  : ident_list ':' type # HIR_SYMBOL_VARIABLE
  | ident_list ':' type '=' expr # HIR_SYMBOL_VARIABLE
  | entity # HIR_SYMBOL_ENTITY
  ;

include stdio as io;

namespace ds {
  use std;
}

struct Foo<T: Iterable<int>>: Bar<T> {

  toto: T;

  Foo() => io::printf("");
  ~Foo() {
    io::printf("");
  }

  call(tata: int, troll(a: int): int => a): T {
    return troll(toto.peek);
  }

  data<R>(oops: struct { item: R; oops: int; })
}

enum TOTO {
  SFSDF = 1,
  SDASDAD(int),
  SDFSDDF,
  SDFDSFSDF(struct {
    a: int;
  });
}

enum TOTO_KIND {
  TOTO_sfsdf,
  TOTO_sdasdad,
  TOTO_sdfsddf,
  TOTO_sdfdsfsdf
}

struct TOTO {
 uint8_t kind;

 union {
    int sdasdad;
    struct {
      int a;
    } sdfdsfsdf;
 };
}

foo: Foo = Foo(sdf);

foo.call(0, (a) => {
  return a + 2;
});
  
symbol_list
  : symbol
  | symbol_list ',' symbol
  ;

symbol_list_or_empty
  : /* empty */
  | symbol_list
  ;

field:
  : symbol
  | MODIFIER symbol
  | USE path
  | USE path AS IDENT
  ;

field_list:
  : field
  | field_list ';' field
  ;

entity
  : ident_list ':' type '=' expr # HIR_ENTITY
  | ident_list '(' symbol_list_or_empty ')' '=>' expr # HIR_ENTITY_CALLABLE
  | ident_list '(' symbol_list_or_empty ')' block_stmt # HIR_ENTITY_CALLABLE
  | ident_list '(' symbol_list_or_empty ')' ':' type '=>' expr # HIR_ENTITY_CALLABLE
  | ident_list '(' symbol_list_or_empty ')' ':' type block_stmt # HIR_ENTITY_CALLABLE
  ;

item
  : entity # HIR_ITEM_ENTITY
  | namespace # HIR_ITEM_NAMESPACE
  ;

item_list
  : item
  | item_list ';' item
  ;
  
item_list_or_empty
  : /* empty */
  | item_list
  | namespace
  ;  

namespace:
  : NAMESPACE path '{' item_list_or_empty '}'
