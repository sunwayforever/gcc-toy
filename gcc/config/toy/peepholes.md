(define_peephole2
[(set (match_operand 0 "nonimmediate_operand")
  (match_operand 1 "nonimmediate_operand"))
 (set (match_dup 1)
  (match_dup 0))]
""
[(set (match_dup 0)
  (match_dup 1))]
)
