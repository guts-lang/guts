tuple
  : type
  | tuple ',' type
  ;

tuple_or_empty
  : /* empty */
  | tuple
  ;

type
  : 'i' BYTES # HIR_TY_INTEGER
  | 'u' BYTES # HIR_TY_INTEGER
  | 'f' BYTES # HIR_TY_FLOAT
  | 'usz' # HIR_TY_SIZE
  | 'isz' # HIR_TY_SIZE
  | 'bool' # HIR_TY_BOOL
  | 'char' # HIR_TY_CHAR
  | IDENT # HIR_TY_SYMBOL
  | IDENT '<' tuple '>' # HIR_TY_SYMBOL
  | IDENT '<' tuple ',' '>' # HIR_TY_SYMBOL
  | '<' tuple '>' # HIR_TY_TUPLE
  | '<' '(' tuple_or_empty ')' '>' # HIR_TY_LAMBDA
  | '<' '(' tuple_or_empty ')' ':' type '>' # HIR_TY_LAMBDA
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
  : ident_list ':' type # HIR_SYMBOL
  | ident_list '(' symbol_list_or_empty ')' # HIR_SYMBOL_CALLABLE
  | ident_list '(' symbol_list_or_empty ')' ':' type # HIR_SYMBOL_FN
  | STRUCT ident_list '{' field_list '}' # HIR_SYMBOL_STRUCT
  | STRUCT ident_list '<' generic_list '>' '{' field_list '}' # HIR_SYMBOL_STRUCT
  | STRUCT ident_list ':' tuple {' field_list '}' # HIR_SYMBOL_STRUCT
  | STRUCT ident_list '<' generic_list '>' ':' tuple  '{' field_list '}' # HIR_SYMBOL_STRUCT
  | entity # HIR_SYMBOL_ENTITY
  ;
  
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
