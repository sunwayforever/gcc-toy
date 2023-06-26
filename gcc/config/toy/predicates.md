(define_predicate "arith_operand"
  (ior (match_code "reg")
       (match_code "const_int")))

(define_predicate "symbolic_operand"
  (match_code "symbol_ref,label_ref"))

(define_predicate "order_operator"
  (match_code "eq,ne,lt,ltu,le,leu,ge,geu,gt,gtu"))

(define_predicate "fp_order_operator"
  (match_code "ne,eq,lt,le,gt,ge"))
