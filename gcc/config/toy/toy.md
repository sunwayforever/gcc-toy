(include "predicates.md")
(include "constraints.md")

(define_mode_iterator ANYI [QI HI SI])
(define_mode_iterator SHORT [QI HI])
(define_mode_iterator ANYF [SF DF])

(define_mode_attr size [(QI "b") (HI "h")])
(define_mode_attr fmt [(SF "s") (DF "d")])
(define_mode_attr load [(QI "lb") (HI "lh") (SI "lw") (SF "flw") (DF "fld")])
(define_mode_attr store [(QI "sb") (HI "sh") (SI "sw") (SF "fsw") (DF "fsd")])

(define_code_iterator arithi [plus and ior xor ashift ashiftrt lshiftrt])
(define_code_iterator arith [minus mult div udiv mod umod])
(define_code_iterator arithf [plus minus mult div])
(define_code_iterator cmpf [lt le eq])

(define_code_attr optab [
  (plus "add")
  (minus "sub")
  (and "and")
  (ior "ior")
  (xor "xor")
  (mult "mul")
  (div "div")
  (udiv "udiv")
  (ashift "ashl")
  (ashiftrt "ashr")
  (lshiftrt "lshr")
  (mod "mod")
  (umod "umod")
  (lt "lt")
  (le "le")
  (eq "eq")
  ])

(define_code_attr insn [
  (plus "add")
  (minus "sub")
  (and "and")
  (ior "or")
  (xor "xor")
  (mult "mul")
  (div "div")
  (udiv "divu")
  (ashift "sll")
  (ashiftrt "sra")
  (lshiftrt "srl")
  (mod "rem")
  (umod "remu")
  ])

(define_insn "<optab>si3"
    [(set (match_operand:SI          0 "register_operand" "=r,r" )
	      (arithi:SI (match_operand:SI 1 "register_operand" "r,r")
		           (match_operand:SI 2 "arith_operand" "r,I")))]
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

(define_insn "<optab><mode>3"
    [(set (match_operand:ANYF          0 "register_operand" "=f")
	      (arithf:ANYF (match_operand:ANYF 1 "register_operand" "f")
		           (match_operand:ANYF 2 "register_operand" "f")))]
  ""
  "f<insn>.<fmt>\t%0,%1,%2"
  [])

(define_insn "nop"
  [(const_int 0)]
  ""
  "nop"
  [])

(define_expand "mov<ANYI:mode>"
    [(set (match_operand:ANYI 0 "general_operand")
	      (match_operand:ANYI 1 "general_operand"))]
  ""
  {
    if (toy_legitimize_move(operands[0], operands[1]))
        DONE;
  }
  )

(define_expand "mov<ANYF:mode>"
    [(set (match_operand:ANYF 0 "general_operand" "=f" )
	      (match_operand:ANYF 1 "general_operand" "f"))]
  ""
  {
    if (toy_legitimize_move(operands[0], operands[1]))
        DONE;
  }
  )

(define_insn "*mov<mode>"
    [(set (match_operand:ANYF 0 "nonimmediate_operand" "=f,f,m,r,f")
	      (match_operand:ANYF 1 "general_operand" "f,m,f,f,r"))]
  ""
  "@
   fmv.<fmt>\t%0, %1
   <load>\t\%0, %1
   <store>\t\%1, %0
   fmv.x.w\t%0, %1
   fmv.w.x\t%0, %1"
  )

(define_insn "*mov<mode>"
    [(set (match_operand:ANYI 0 "nonimmediate_operand" "=r,r,m,r")
	      (match_operand:ANYI 1 "general_operand" "r,m,r,i"))]
  ""
  "@
   mv\t%0, %1
   <load>\t\%0, %1
   <store>\t\%1, %0
   li\t%0,%1"
  )

(define_insn "*storei"
   [(set (match_operand:ANYI 0 "memory_operand")
	(match_operand 1 "register_operand"))]
  ""
  "<store>\t%1, %0")

(define_insn "*la"
    [(set (match_operand:SI 0 "register_operand" "=r")
	      (match_operand:SI 1 "symbolic_operand" ))]
  ""
  "la\t%0, %1"
  )

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

(define_expand "cstore<mode>4"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(match_operator:SI 1 "fp_order_operator"
	    [(match_operand:ANYF 2 "register_operand" "f")
	     (match_operand:ANYF 3 "register_operand" "f")]))]
  ""
  {
    toy_expand_fp_scc (operands[0], GET_CODE (operands[1]), operands[2],
			operands[3]);
    DONE;
  })

(define_insn "*f<optab>"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(cmpf:SI
	 (match_operand:ANYF 1 "register_operand" "f")
     (match_operand:ANYF 2 "register_operand" "f")))]
  ""
  "f<optab>.<fmt>\t%0,%1,%2"
  [])

(define_insn "*slt"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(lt:SI
	    (match_operand:SI 1 "register_operand" "r,r")
        (match_operand:SI 2 "arith_operand" "r,I")))]
  ""
  "@
   slt\t%0,%1,%2
   slti\t%0,%1,%2"
  [])

(define_insn "*sltu"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(ltu:SI
	    (match_operand:SI 1 "register_operand" "r,r")
        (match_operand:SI 2 "arith_operand" "r,I")))]
  ""
  "@
   sltu\t%0,%1,%2
   sltiu\t%0,%1,%2"
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

(define_expand "cbranch<mode>4"
  [(set (pc)
	(if_then_else (match_operator 0 "comparison_operator"
		      [(match_operand:ANYF 1 "register_operand")
		       (match_operand:ANYF 2 "register_operand")])
		      (label_ref (match_operand 3 ""))
		      (pc)))]
  ""
  {
      toy_expand_fp_brcc (operands);
  }
  )

(define_insn "jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  ""
  "j %0"
  [])

(define_expand "prologue"
  [(clobber (const_int 0))]
  ""
  {
  toy_expand_prologue ();
  DONE;
  }
)

(define_expand "epilogue"
  [(clobber (const_int 0))]
  ""
  {
  toy_expand_epilogue ();
  DONE;
  }
)

(define_insn "return"
  [(return)]
  ""
  "ret"
  []
)

(define_insn "call"
  [(call (match_operand:SI 0 "memory_operand" "")
		(match_operand 1 "general_operand" ""))]
  ""
  "call %0"
  )

(define_insn "call_value"
    [(set (match_operand 0 "register_operand")
	      (call (match_operand:SI 1 "memory_operand")
	            (match_operand 2 "general_operand")))]
  ""
  "call %1")

(define_insn "extendsfdf2"
  [(set (match_operand:DF     0 "register_operand" "=f")
	(float_extend:DF
	    (match_operand:SF 1 "register_operand" "f")))]
  ""
  "fcvt.d.s\t%0,%1"
  [])

(define_insn "truncdfsf2"
  [(set (match_operand:SF     0 "register_operand" "=f")
	(float_truncate:SF
	    (match_operand:DF 1 "register_operand" " f")))]
  ""
  "fcvt.s.d\t%0,%1"
  [])

(define_insn "abs<mode>2"
  [(set (match_operand:ANYF           0 "register_operand" "=f")
	(abs:ANYF (match_operand:ANYF 1 "register_operand" " f")))]
    ""
  "fabs.<fmt>\t%0,%1"
  [])

(define_insn "fix_trunc<mode>si2"
  [(set (match_operand:SI      0 "register_operand" "=r")
	(fix:SI
	    (match_operand:ANYF 1 "register_operand" " f")))]
  ""
  "fcvt.w.<fmt> %0,%1,rtz"
  [])

(define_insn "floatsi<mode>2"
  [(set (match_operand:ANYF    0 "register_operand" "=f")
	(float:ANYF
	    (match_operand:SI 1 "register_operand" "r")))]
  ""
  "fcvt.<fmt>.w\t%0,%1"
  [])

(define_insn "neg<mode>2"
  [(set (match_operand:ANYF 0 "register_operand" "=f")
	(neg:ANYF (match_operand:ANYF 1 "register_operand" " f")))]
  ""
  "fneg.<fmt>\t%0,%1"
  [])

(define_insn "getsp"
    [(unspec [(match_operand:SI 0 "register_operand" "r")] 0)]
  ""
  "mv \t%0,sp"
  [])
