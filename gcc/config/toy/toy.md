(define_insn "addsi3"
    [(set (match_operand:SI          0 "register_operand" )
	      (plus:SI (match_operand:SI 1 "register_operand" )
		           (match_operand:SI 2 "register_operand" )))]
  "true"
  "add\t%0,%1,%2"
  [])

(define_insn "nop"
  [(const_int 0)]
  ""
  "nop"
  [])

(define_expand "movsi"
    [(set (match_operand:SI 0 "general_operand" "" )
	      (match_operand:SI 1 "general_operand" ""))]
  ""
  {
  if (GET_CODE (operands[0]) == MEM)
      operands[1]=force_reg(SImode, operands[1]);
  }
  )

(define_insn "*movsi"
    [(set (match_operand:SI 0 "register_operand")
	      (match_operand:SI 1 "register_operand"))]
  ""
  "mov %0, %1"
  )

(define_insn "*movsi1"
    [(set (match_operand:SI 0 "register_operand")
	    (match_operand:SI 1 "const_int_operand"))]
  ""
  "li %0, %1"
  )

(define_insn "*movsi2"
  [(set (match_operand:SI 0 "memory_operand")
	(match_operand:SI 1 "register_operand"))]
  ""
  "sw %1, %0")

(define_insn "*movsi3"
  [(set (match_operand:SI 0 "register_operand")
	(match_operand:SI 1 "memory_operand"))]
  ""
  "lw %0, %1")
