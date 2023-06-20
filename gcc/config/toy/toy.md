(define_insn "addsi3"
  [(set (match_operand:SI          0 "register_operand" "=r,r")
	(plus:SI (match_operand:SI 1 "register_operand" " r,r")
		 (match_operand:SI 2 "register_operand"    " r,r")))]
  "true"
  {return "add\t%0,%1,%2";}
  [])


(define_insn "nop"
  [(const_int 0)]
  ""
  "nop"
  [])

(define_insn "movsi"
  [(set (match_operand:SI 0 "register_operand")
	(match_operand:SI 1 "register_operand"))]
  ""
  "addi\t$0, %1, 0"
  )
