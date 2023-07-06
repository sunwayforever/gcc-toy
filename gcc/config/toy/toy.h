// 2023-06-20 11:35
#ifndef TOY_H
#define TOY_H

#define DWARF2_DEBUGGING_INFO 1

// 18 gpr + 18 fpr + 2 + 14 vector
#define FIRST_PSEUDO_REGISTER 52

#define RETURN_ADDR_REGNUM 1
#define ARG_POINTER_REGNUM 36
#define FRAME_POINTER_REGNUM 37
#define STACK_POINTER_REGNUM 2
#define HARD_FRAME_POINTER_REGNUM 8

#define GP_REG_FIRST 0
#define GP_ARG_FIRST 10
#define GP_REG_LAST 17
#define GP_REG_NUM (GP_REG_LAST - GP_REG_FIRST + 1)

#define FP_REG_FIRST 18
#define FP_ARG_FIRST 28
#define FP_REG_LAST 35
#define FP_REG_NUM (FP_REG_LAST - FP_REG_FIRST + 1)

#define VECTOR_REG_FIRST 38
#define VECTOR_REG_LAST 51
#define VECTOR_REG_NUM (VECTOR_REG_LAST - VECTOR_REG_FIRST + 1)

#define FUNCTION_ARG_REGNO_P(N)                       \
    (IN_RANGE((N), GP_ARG_FIRST, GP_ARG_FIRST + 7) || \
     IN_RANGE((N), FP_ARG_FIRST, FP_ARG_FIRST + 7))

enum reg_class {
    NO_REGS,  /* no registers in set */
    GPR_REGS, /* integer registers */
    FPR_REGS, /* floating-point registers */
    FRAME_REGS,
    VECTOR_REGS,
    ALL_REGS,       /* all registers */
    LIM_REG_CLASSES /* max value + 1 */
};

#define REG_CLASS_NAMES \
    { "NO_REGS", "GR_REGS", "FP_REGS", "FRAME_REGS", "VECTOR_REGS", "ALL_REGS" }

// clang-format off
#define FIXED_REGISTERS \
    { /* General registers.  */                               \
      1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    \
      /* Floating-point registers.  */                        \
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, \
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
    }

#define CALL_USED_REGISTERS						\
{ /* General registers.  */						\
  1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, \
  /* Floating-point registers.  */					\
  1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   \
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
}

#define REG_CLASS_CONTENTS          \
{									\
  { 0x00000000, 0x00000000},	/* NO_REGS */                   \
  { 0x0003ffff, 0x00000000},	/* GR_REGS */                   \
  { 0xfffc0000, 0x0000000f},	/* FP_REGS */                   \
  { 0x00000000, 0x00000030},	/* FRAME_REGS */                   \
  { 0x00000000, 0x000fffc0},	/* VECTOR_REGS */                   \
  { 0xffffffff, 0x000fffff}, /* ALL_REGS */		\
}
#define REGISTER_NAMES						\
{ "zero","ra",  "sp",  "gp",  "tp",  "t0",  "t1",  "t2",	\
  "s0",  "s1",  "a0",  "a1",  "a2",  "a3",  "a4",  "a5",	\
  "a6",  "a7",                                              \
  "ft0", "ft1", "ft2", "ft3", "ft4", "ft5", "ft6", "ft7",	\
  "fs0", "fs1", "fa0", "fa1", "fa2", "fa3", "fa4", "fa5",	\
  "fa6", "fa7", "varg", "vfp",                              \
  "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13"}
// clang-format off

extern const enum reg_class toy_regno_to_class[];
#define REGNO_REG_CLASS(REGNO) toy_regno_to_class[(REGNO)]

// clang-format off
#define ELIMINABLE_REGS                                   \
    {                                                     \
        {ARG_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM},       \
        {FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM},     \
    }
// clang-format on

#define GP_REG_P(REGNO) ((unsigned int)((int)(REGNO)-GP_REG_FIRST) < GP_REG_NUM)

#define FP_REG_P(REGNO) ((unsigned int)((int)(REGNO)-FP_REG_FIRST) < FP_REG_NUM)

#define VECTOR_REG_P(REGNO) ((unsigned int)((int)(REGNO)-VECTOR_REG_FIRST) < VECTOR_REG_NUM)

#define REGNO_MODE_OK_FOR_BASE_P(REGNO, MODE) GP_REG_P(REGNO)

typedef struct {
    int num_gprs;
    int num_fprs;
} CUMULATIVE_ARGS;

#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, INDIRECT, N_NAMED_ARGS) \
    memset(&(CUM), 0, sizeof(CUM))

#define LOCAL_LABEL_PREFIX "."
#define ASM_GENERATE_INTERNAL_LABEL(LABEL, PREFIX, NUM) \
    sprintf((LABEL), "*%s%s%ld", (LOCAL_LABEL_PREFIX), (PREFIX), (long)(NUM))

#define N_REG_CLASSES (int)LIM_REG_CLASSES
#define UNITS_PER_WORD 4
#define MOVE_MAX UNITS_PER_WORD
#define Pmode word_mode
#define TRAMPOLINE_SIZE 16

#define STACK_BOUNDARY 2 * BITS_PER_WORD
#define PREFERRED_STACK_BOUNDARY 128
#define PARM_BOUNDARY BITS_PER_WORD
#define FUNCTION_BOUNDARY 32
#define STRICT_ALIGNMENT 1
#define ACCUMULATE_OUTGOING_ARGS 1

#define STACK_GROWS_DOWNWARD 1
#define FRAME_GROWS_DOWNWARD 1

#define TOY_STACK_ALIGN(LOC)                          \
    (((LOC) + ((PREFERRED_STACK_BOUNDARY / 8) - 1)) & \
     -(PREFERRED_STACK_BOUNDARY / 8))

#define BIGGEST_ALIGNMENT 128
#define FUNCTION_MODE SImode
#define MAX_REGS_PER_ADDRESS 1
#define GENERAL_REGS GPR_REGS
#define BASE_REG_CLASS GPR_REGS
#define INDEX_REG_CLASS NO_REGS
#define REGNO_OK_FOR_INDEX_P(REGNO) 0

#define BITS_BIG_ENDIAN 0
#define BYTES_BIG_ENDIAN 0
#define WORDS_BIG_ENDIAN 0

#define TARGET_CPU_CPP_BUILTINS()

#define SLOW_BYTE_ACCESS 1
#define CASE_VECTOR_MODE SImode

#define FIRST_PARM_OFFSET(FNDECL) 0

#define ASM_OUTPUT_ALIGN(STREAM, LOG) fprintf(STREAM, "\t.align\t%d\n", (LOG))
#define ASM_APP_ON " "
#define ASM_APP_OFF " "

#define FUNCTION_PROFILER(STREAM, LABELNO) \
    do {                                   \
    } while (0)

#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET) \
    (OFFSET) = toy_initial_elimination_offset(FROM, TO)

#define DEFAULT_SIGNED_CHAR 0
#define GLOBAL_ASM_OP "\t.globl\t"

#define TARGET_ASM_CONSTRUCTOR toy_asm_out_constructor
#define TARGET_ASM_DESTRUCTOR toy_asm_out_destructor

#define TEXT_SECTION_ASM_OP "\t.text"
#define DATA_SECTION_ASM_OP "\t.data"
#define READONLY_DATA_SECTION_ASM_OP "\t.section\t.rodata"
#define BSS_SECTION_ASM_OP "\t.bss"

#define IMM_BITS 12
#define IMM_REACH (1LL << IMM_BITS)
#define SMALL_OPERAND(VALUE) \
    ((unsigned HOST_WIDE_INT)(VALUE) + IMM_REACH / 2 < IMM_REACH)

#define INCOMING_RETURN_ADDR_RTX gen_rtx_REG(VOIDmode, RETURN_ADDR_REGNUM)
#define DWARF_FRAME_REGNUM(REGNO) \
    (GP_REG_P(REGNO) || FP_REG_P(REGNO) ? REGNO : INVALID_REGNUM)

#endif  // TOY_H
