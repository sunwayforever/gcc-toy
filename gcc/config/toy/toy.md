(include "predicates.md")

(define_insn "addsi3"
    [(set (match_operand:SI          0 "register_operand" "=r,r" )
	      (plus:SI (match_operand:SI 1 "register_operand" "r,r")
		           (match_operand:SI 2 "arith_operand" "r,i")))]
  "true"
  "@
   add\t%0,%1,%2
   addi\t%0,%1,%2"
  [])

(define_insn "mulsi3"
    [(set (match_operand:SI          0 "register_operand")
	      (mult:SI (match_operand:SI 1 "register_operand")
		           (match_operand:SI 2 "register_operand")))]
  ""
  "mul\t%0,%1,%2"
  [])

(define_insn "divsi3"
    [(set (match_operand:SI          0 "register_operand")
	      (div:SI (match_operand:SI 1 "register_operand")
		           (match_operand:SI 2 "register_operand")))]
  ""
  "div\t%0,%1,%2"
  [])

(define_insn "andsi3"
    [(set (match_operand:SI          0 "register_operand" "=r,r")
	      (and:SI (match_operand:SI 1 "register_operand" "r,r")
		           (match_operand:SI 2 "arith_operand" "r,i")))]
  ""
  "@
   and\t%0,%1,%2
   andi\t%0,%1,%2"
  [])

(define_insn "iorsi3"
    [(set (match_operand:SI          0 "register_operand" "=r,r")
	      (ior:SI (match_operand:SI 1 "register_operand" "r,r")
		           (match_operand:SI 2 "arith_operand" "r,i")))]
  ""
  "@
   or\t%0,%1,%2
   ori\t%0,%1,%2"
  [])

(define_insn "xorsi3"
    [(set (match_operand:SI          0 "register_operand" "=r,r")
	      (xor:SI (match_operand:SI 1 "register_operand" "r,r")
		           (match_operand:SI 2 "arith_operand" "r,i")))]
  ""
  "@
   xor\t%0,%1,%2
   xori\t%0,%1,%2"
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
    [(set (match_operand:SI 0 "register_operand" "=r,r")
	      (match_operand:SI 1 "arith_operand" "r,i"))]
  ""
  "@
   mov %0, %1
   li %0, %1"
  )

(define_insn "*store"
  [(set (match_operand:SI 0 "memory_operand")
	(match_operand:SI 1 "register_operand"))]
  ""
  "sw %1, %0")

(define_insn "*load"
  [(set (match_operand:SI 0 "register_operand")
	(match_operand:SI 1 "memory_operand"))]
  ""
  "lw %0, %1")
