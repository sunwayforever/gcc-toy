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
    if (toy_legitimize_move(SImode, operands[0], operands[1]))
        DONE;
  }
  )

(define_expand "movqi"
    [(set (match_operand:QI 0 "general_operand" "" )
	      (match_operand:QI 1 "general_operand" ""))]
  ""
  {
    if (toy_legitimize_move(QImode, operands[0], operands[1]))
        DONE;
  }
  )

(define_insn "*movqi"
    [(set (match_operand:QI 0 "register_operand")
	      (subreg:QI (match_operand:SI 1 "register_operand") 0))]
  ""
  "mv\t%0, %1"
  )

(define_insn "*movsi"
    [(set (match_operand:SI 0 "register_operand" "=r,r")
	      (match_operand:SI 1 "arith_operand" "r,i"))]
  ""
  "@
   mv\t%0, %1
   li\t%0, %1"
  )

(define_insn "*movsi"
    [(set (match_operand:SI 0 "register_operand")
	      (match_operand:SI 1 "symbolic_operand" ))]
  ""
  "la\t%0, %1"
  )

(define_insn "*store"
   [(set (match_operand:SI 0 "memory_operand")
	(match_operand:SI 1 "register_operand"))]
  ""
  "sw\t%1, %0")

(define_insn "*load"
  [(set (match_operand:SI 0 "register_operand")
	(match_operand:SI 1 "memory_operand"))]
  ""
  "lw\t%0, %1")

(define_expand "cstoresi4"
  [(set (match_operand:SI 0 "register_operand")
	(match_operator:SI 1 "order_operator"
	    [(match_operand:SI 2 "register_operand")
	     (match_operand:SI 3 "register_operand")]))]
  ""
  {
    toy_expand_int_scc (operands[0], GET_CODE (operands[1]), operands[2],
			operands[3]);
    DONE;
  })

(define_insn "*slt"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(lt:SI
	    (match_operand:SI 1 "register_operand" "r,r")
        (match_operand:SI 2 "arith_operand" "r,i")))]
  ""
  "@
   slt\t%0,%1,%2
   slti\t%0,%1,%2"
  [])

(define_insn "zero_extendqisi2"
  [(set (match_operand:SI 0 "register_operand"    "")
	(zero_extend:SI
	    (match_operand:QI 1 "register_operand" "")))]
  ""
  "zext.b\t%0,%1"
  [])

(define_insn "cbranchsi4"
  [(set (pc)
	(if_then_else (match_operator 0 "comparison_operator"
		      [(match_operand:SI 1 "register_operand")
		       (match_operand:SI 2 "register_operand")])
		      (label_ref (match_operand 3 ""))
		      (pc)))]
  ""
  "b%C0\t%1,%2,%3"
  )

(define_insn "jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  ""
  "j"
  [])
