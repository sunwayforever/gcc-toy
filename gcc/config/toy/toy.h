// 2023-06-20 11:35
#ifndef TOY_H
#define TOY_H

#define DWARF2_DEBUGGING_INFO 1

// 18 gpr + 18 fpr + 2
#define FIRST_PSEUDO_REGISTER 38

#define ARG_POINTER_REGNUM 36
#define FRAME_POINTER_REGNUM 37
#define STACK_POINTER_REGNUM 2
#define HARD_FRAME_POINTER_REGNUM 8

#define FUNCTION_ARG_REGNO_P(N) \
    (IN_RANGE((N), 10, 17) || IN_RANGE((N), 10 + 18, 17 + 18))

enum reg_class {
    NO_REGS,        /* no registers in set */
    GPR_REGS,       /* integer registers */
    FPR_REGS,       /* floating-point registers */
    ALL_REGS,       /* all registers */
    LIM_REG_CLASSES /* max value + 1 */
};

#define REG_CLASS_NAMES \
    { "NO_REGS", "GR_REGS", "FP_REGS", "ALL_REGS" }

// clang-format off
#define FIXED_REGISTERS \
    { /* General registers.  */                               \
      1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    \
      /* Floating-point registers.  */                        \
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0\
    }

#define CALL_USED_REGISTERS						\
{ /* General registers.  */						\
  1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, \
  /* Floating-point registers.  */					\
  1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 \
}

#define REG_CLASS_CONTENTS          \
{									\
  { 0x00000000, 0x00000000},	/* NO_REGS */                   \
  { 0xffffc000, 0x00000000},	/* GR_REGS */                   \
  { 0x00000000, 0xffffc000},	/* FP_REGS */                   \
  { 0xffffc000, 0xfffff000}, /* ALL_REGS */		\
}
#define REGISTER_NAMES						\
{ "zero","ra",  "sp",  "gp",  "tp",  "t0",  "t1",  "t2",	\
  "s0",  "s1",  "a0",  "a1",  "a2",  "a3",  "a4",  "a5",	\
  "a6",  "a7",                                              \
  "ft0", "ft1", "ft2", "ft3", "ft4", "ft5", "ft6", "ft7",	\
  "fs0", "fs1", "fa0", "fa1", "fa2", "fa3", "fa4", "fa5",	\
  "fa6", "fa7", "arg", "vfp"}
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

#define GP_REG_FIRST 0
#define GP_REG_LAST 17
#define GP_REG_NUM (GP_REG_LAST - GP_REG_FIRST + 1)

#define FP_REG_FIRST 18
#define FP_REG_LAST 35
#define FP_REG_NUM (FP_REG_LAST - FP_REG_FIRST + 1)

#define GP_REG_P(REGNO) ((unsigned int)((int)(REGNO)-GP_REG_FIRST) < GP_REG_NUM)

#define FP_REG_P(REGNO) ((unsigned int)((int)(REGNO)-FP_REG_FIRST) < FP_REG_NUM)

#define REGNO_MODE_OK_FOR_BASE_P(REGNO, MODE) GP_REG_P(REGNO)

typedef struct {
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

#define STACK_BOUNDARY BITS_PER_WORD
#define PARM_BOUNDARY BITS_PER_WORD
#define FUNCTION_BOUNDARY 32
#define STRICT_ALIGNMENT 1

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

#endif  // TOY_H
