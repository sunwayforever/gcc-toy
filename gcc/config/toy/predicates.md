(define_predicate "arith_operand"
    (ior (match_code "const_int")
         (match_operand 0 "register_operand")))

(define_predicate "symbolic_operand"
  (match_code "symbol_ref,label_ref"))

(define_predicate "order_operator"
  (match_code "eq,ne,lt,ltu,le,leu,ge,geu,gt,gtu"))

(define_predicate "fp_order_operator"
  (match_code "ne,eq,lt,le,gt,ge"))

(define_predicate "const_0_operand"
  (and (match_code "const_int")
       (match_test "op == CONST0_RTX (GET_MODE (op))")))

(define_predicate "register_or_zero_operand"
  (ior (match_operand 0 "const_0_operand")
       (match_operand 0 "register_operand")))
