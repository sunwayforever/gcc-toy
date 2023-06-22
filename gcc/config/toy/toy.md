(include "predicates.md")

(define_code_iterator arithi [plus and ior xor ashift ashiftrt lshiftrt])
(define_code_iterator arith [mult div])

(define_code_attr optab [
  (plus "add")
  (and "and")
  (ior "ior")
  (xor "xor")
  (mult "mul")
  (div "div")
  (ashift "ashl")
  (ashiftrt "ashr")
  (lshiftrt "lshr")
  ])

(define_code_attr insn [
  (plus "add")
  (and "and")
  (ior "or")
  (xor "xor")
  (mult "mul")
  (div "div")
  (ashift "sll")
  (ashiftrt "sra")
  (lshiftrt "srl")
  ])

(define_insn "<optab>si3"
    [(set (match_operand:SI          0 "register_operand" "=r,r" )
	      (arithi:SI (match_operand:SI 1 "register_operand" "r,r")
		           (match_operand:SI 2 "arith_operand" "r,i")))]
  "true"
  "@
   <insn>\t%0,%1,%2
   <insn>i\t%0,%1,%2"
  [])


(define_insn "<optab>si3"
    [(set (match_operand:SI          0 "register_operand")
	      (arith:SI (match_operand:SI 1 "register_operand")
		           (match_operand:SI 2 "register_operand")))]
  ""
  "<insn>\t%0,%1,%2"
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
    if (toy_legitimize_move(operands[0], operands[1]))
        DONE;
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

(define_insn "*movsi"
    [(set (match_operand:SI 0 "register_operand")
	      (match_operand:SI 1 "symbolic_operand" ))]
  ""
  "la %0, %1"
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
