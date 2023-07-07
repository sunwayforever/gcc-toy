(define_register_constraint "f" "FPR_REGS" "A floating-point register")
(define_register_constraint "v" "VECTOR_REGS" "A vector register")

(define_constraint "I"
  "An I-type 12-bit signed immediate."
  (and (match_code "const_int")
       (match_test "SMALL_OPERAND (ival)")))

(define_constraint "J"
  "Integer zero."
  (and (match_code "const_int")
       (match_test "ival == 0")))
