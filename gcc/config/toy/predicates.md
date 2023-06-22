(define_predicate "arith_operand"
  (ior (match_code "reg")
       (match_code "const_int")))

(define_predicate "symbolic_operand"
  (match_code "const,symbol_ref,label_ref"))
