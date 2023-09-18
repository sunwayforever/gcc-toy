#define IN_TARGET_CODE 1

// clang-format off
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "regs.h"
#include "insn-config.h"
#include "insn-attr.h"
#include "recog.h"
#include "output.h"
#include "alias.h"
#include "tree.h"
#include "stringpool.h"
#include "attribs.h"
#include "varasm.h"
#include "stor-layout.h"
#include "calls.h"
#include "function.h"
#include "explow.h"
#include "memmodel.h"
#include "emit-rtl.h"
#include "reload.h"
#include "tm_p.h"
#include "target.h"
#include "target-def.h"
#include "basic-block.h"
#include "expr.h"
#include "optabs.h"
#include "bitmap.h"
#include "df.h"
#include "diagnostic.h"
#include "builtins.h"
#include "predict.h"
#include "tree-pass.h"
#include "opts.h"
#include "langhooks.h"
// clang-format on

const enum reg_class toy_regno_to_class[FIRST_PSEUDO_REGISTER] = {
    GPR_REGS,    GPR_REGS,    GPR_REGS,    GPR_REGS,    GPR_REGS,
    GPR_REGS,    GPR_REGS,    GPR_REGS,    GPR_REGS,    GPR_REGS,
    GPR_REGS,    GPR_REGS,    GPR_REGS,    GPR_REGS,    GPR_REGS,
    GPR_REGS,    GPR_REGS,    GPR_REGS,

    FPR_REGS,    FPR_REGS,    FPR_REGS,    FPR_REGS,    FPR_REGS,
    FPR_REGS,    FPR_REGS,    FPR_REGS,    FPR_REGS,    FPR_REGS,
    FPR_REGS,    FPR_REGS,    FPR_REGS,    FPR_REGS,    FPR_REGS,
    FPR_REGS,    FPR_REGS,    FPR_REGS,    FRAME_REGS,  FRAME_REGS,

    VECTOR_REGS, VECTOR_REGS, VECTOR_REGS, VECTOR_REGS, VECTOR_REGS,
    VECTOR_REGS, VECTOR_REGS, VECTOR_REGS, VECTOR_REGS, VECTOR_REGS,
    VECTOR_REGS, VECTOR_REGS, VECTOR_REGS, VECTOR_REGS,
};

void toy_asm_out_constructor(rtx symbol, int priority) {}
void toy_asm_out_destructor(rtx symbol, int priority) {}

static bool toy_legitimate_address_p(machine_mode mode, rtx x, bool strict_p) {
    return true;
}

#undef TARGET_LEGITIMATE_ADDRESS_P
#define TARGET_LEGITIMATE_ADDRESS_P toy_legitimate_address_p

void toy_print_operand(FILE *file, rtx op, int letter) {
    enum rtx_code code = GET_CODE(op);

    switch (letter) {
        case 'C':
            fputs(GET_RTX_NAME(code), file);
            break;
    }
    switch (code) {
        case REG:
            fprintf(file, "%s", reg_names[REGNO(op)]);
            return;

        case MEM:
            output_address(VOIDmode, XEXP(op, 0));
            return;

        case CODE_LABEL:
            output_address(VOIDmode, op);
            return;

        default:
            if (CONSTANT_P(op)) {
                output_addr_const(file, op);
                return;
            }
    }
}

static void toy_print_operand_address(
    FILE *file, machine_mode mode ATTRIBUTE_UNUSED, rtx x) {
    switch (GET_CODE(x)) {
        case REG:
            fprintf(file, "0(%s)", reg_names[REGNO(x)]);
            break;
        case PLUS:
            toy_print_operand(file, XEXP(x, 1), 0);
            fprintf(file, "(%s)", reg_names[REGNO(XEXP(x, 0))]);
            break;
        case SYMBOL_REF:
        case CODE_LABEL:
            output_addr_const(file, x);
            break;
        case CONST:
            toy_print_operand_address(file, mode, XEXP(x, 0));
            break;
        default:
            abort();
    }
}

#define TARGET_PRINT_OPERAND toy_print_operand
#define TARGET_PRINT_OPERAND_ADDRESS toy_print_operand_address

static bool toy_can_eliminate(const int from ATTRIBUTE_UNUSED, const int to) {
    return (to == HARD_FRAME_POINTER_REGNUM);
}

#define TARGET_CAN_ELIMINATE toy_can_eliminate

static rtx legitimize_memory_address(machine_mode mode, rtx x) {
    if (!can_create_pseudo_p()) {
        return x;
    }
    switch (GET_CODE(x)) {
        case PLUS: {
            rtx tmp = gen_reg_rtx(GET_MODE(x));
            rtx a = legitimize_memory_address(GET_MODE(x), XEXP(x, 0));
            rtx b = XEXP(x, 1);
            if (GET_CODE(XEXP(x, 1)) != CONST_INT) {
                b = legitimize_memory_address(GET_MODE(x), XEXP(x, 1));
            }
            if (a == XEXP(x, 0) && b == XEXP(x, 1) &&
                GET_MODE_CLASS(mode) != MODE_VECTOR_INT) {
                return x;
            }
            emit_insn(gen_add3_insn(tmp, a, b));
            return tmp;
        }
        case MULT: {
            rtx tmp = gen_reg_rtx(GET_MODE(x));
            rtx a = force_reg(GET_MODE(x), XEXP(x, 0));
            rtx b = force_reg(GET_MODE(x), XEXP(x, 1));
            emit_insn(
                gen_rtx_SET(tmp, gen_rtx_fmt_ee(MULT, GET_MODE(tmp), a, b)));
            return tmp;
        }
        case SYMBOL_REF: {
            rtx tmp = gen_reg_rtx(GET_MODE(x));
            emit_insn(gen_rtx_SET(tmp, x));
            return tmp;
        }
        case CONST: {
            return legitimize_memory_address(GET_MODE(x), XEXP(x, 0));
        }
        case CONST_INT: {
            rtx tmp = gen_reg_rtx(SImode);
            emit_insn(gen_rtx_SET(tmp, x));
            return tmp;
        }
        case MEM: {
            rtx tmp = gen_reg_rtx(GET_MODE(x));
            emit_move_insn(tmp, x);
            return tmp;
        }
    }
    return x;
}

bool toy_legitimize_move(rtx dst, rtx src) {
    bool legitimize = false;
    machine_mode mode = GET_MODE(dst);
    if (GET_CODE(dst) == MEM) {
        rtx tmp = legitimize_memory_address(mode, XEXP(dst, 0));
        if (tmp != XEXP(dst, 0)) {
            XEXP(dst, 0) = tmp;
            legitimize = true;
        }
        if (GET_CODE(src) != MEM) {
            rtx tmp = legitimize_memory_address(mode, src);
            if (tmp != src) {
                src = tmp;
                legitimize = true;
            }
        }
    }
    if (GET_CODE(src) == MEM) {
        rtx tmp = legitimize_memory_address(mode, XEXP(src, 0));
        if (tmp != XEXP(src, 0)) {
            XEXP(src, 0) = tmp;
            legitimize = true;
        }
        if (GET_CODE(dst) == MEM) {
            rtx tmp = gen_reg_rtx(GET_MODE(dst));
            emit_move_insn(tmp, src);
            src = tmp;
            legitimize = true;
        }
    }
    if (legitimize) {
        emit_insn(gen_rtx_SET(dst, src));
        return true;
    }
    return false;
}

void toy_expand_int_scc(rtx target, int code, rtx op0, rtx op1) {
    switch (code) {
        case LT:
            emit_insn(gen_rtx_SET(
                target, gen_rtx_fmt_ee(LT, GET_MODE(target), op0, op1)));
            break;
        case LTU:
            emit_insn(gen_rtx_SET(
                target, gen_rtx_fmt_ee(LTU, GET_MODE(target), op0, op1)));
            break;
        case GT:
            emit_insn(gen_rtx_SET(
                target, gen_rtx_fmt_ee(LT, GET_MODE(target), op1, op0)));
            break;
        case GE:
            emit_insn(gen_rtx_SET(
                target, gen_rtx_fmt_ee(LT, GET_MODE(target), op0, op1)));
            emit_insn(gen_rtx_SET(
                target,
                gen_rtx_fmt_ee(XOR, GET_MODE(target), target, const1_rtx)));
            break;
        case LE:
            emit_insn(gen_rtx_SET(
                target, gen_rtx_fmt_ee(LT, GET_MODE(target), op1, op0)));
            emit_insn(gen_rtx_SET(
                target,
                gen_rtx_fmt_ee(XOR, GET_MODE(target), target, const1_rtx)));
            break;
        case EQ:
            emit_insn(gen_rtx_SET(
                target, gen_rtx_fmt_ee(XOR, GET_MODE(target), op1, op0)));
            emit_insn(gen_rtx_SET(
                target,
                gen_rtx_fmt_ee(LT, GET_MODE(target), target, const1_rtx)));
            break;
        case NE:
            emit_insn(gen_rtx_SET(
                target, gen_rtx_fmt_ee(XOR, GET_MODE(target), op1, op0)));
            emit_insn(gen_rtx_SET(
                target, gen_rtx_fmt_ee(
                            LT, GET_MODE(target),
                            gen_rtx_REG(GET_MODE(target), 0), target)));
            break;
        default:
            abort();
    }
}

void toy_expand_fp_scc(rtx target, int code, rtx op0, rtx op1) {
    switch (code) {
        case LT:
            emit_insn(gen_rtx_SET(
                target, gen_rtx_fmt_ee(LT, GET_MODE(target), op0, op1)));
            break;
        case GT:
            emit_insn(gen_rtx_SET(
                target, gen_rtx_fmt_ee(LT, GET_MODE(target), op1, op0)));
            break;
        case GE:
            emit_insn(gen_rtx_SET(
                target, gen_rtx_fmt_ee(LE, GET_MODE(target), op1, op0)));
            break;
        case LE:
            emit_insn(gen_rtx_SET(
                target, gen_rtx_fmt_ee(LE, GET_MODE(target), op0, op1)));
            break;
        case EQ:
            emit_insn(gen_rtx_SET(
                target, gen_rtx_fmt_ee(EQ, GET_MODE(target), op0, op1)));
            break;
        case NE:
            emit_insn(gen_rtx_SET(
                target, gen_rtx_fmt_ee(EQ, GET_MODE(target), op0, op1)));
            emit_insn(gen_rtx_SET(
                target,
                gen_rtx_fmt_ee(XOR, GET_MODE(target), target, const1_rtx)));
            break;
        default:
            abort();
    }
}

void toy_expand_fp_brcc(rtx *operands) {
    rtx compare = operands[0];
    rtx op0 = operands[1];
    rtx op1 = operands[2];
    rtx reg = gen_reg_rtx(SImode);
    toy_expand_fp_scc(reg, GET_CODE(compare), op0, op1);
    operands[0] = gen_rtx_fmt_ee(NE, SImode, op0, op1);
    operands[1] = reg;
    operands[2] = gen_rtx_REG(SImode, 0);
}

static int toy_stack_size;
static int toy_csr_size;

static bool toy_save_reg_p(unsigned int regno) {
    bool call_saved = !global_regs[regno] && !call_used_or_fixed_reg_p(regno);
    bool might_clobber =
        crtl->saves_all_registers || df_regs_ever_live_p(regno);
    if (call_saved && might_clobber) return true;
    if (regno == HARD_FRAME_POINTER_REGNUM) return true;
    if (regno == RETURN_ADDR_REGNUM) return true;

    return false;
}

static void toy_compute_frame(void) {
    toy_stack_size = 0;
    toy_stack_size += TOY_STACK_ALIGN(crtl->outgoing_args_size);
    toy_stack_size += TOY_STACK_ALIGN(get_frame_size());
    toy_csr_size = 0;
    for (int regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++) {
        if (toy_save_reg_p(regno)) {
            if (REGNO_REG_CLASS(regno) == GPR_REGS) {
                toy_csr_size += 4;
            } else if (REGNO_REG_CLASS(regno) == FPR_REGS) {
                toy_csr_size += 8;
            } else {
                gcc_unreachable();
            }
        }
    }
    toy_csr_size = TOY_STACK_ALIGN(toy_csr_size);
    toy_stack_size += toy_csr_size;
}

HOST_WIDE_INT
toy_initial_elimination_offset(int from, int ARG_UNUSED(to)) {
    toy_compute_frame();
    if (from == FRAME_POINTER_REGNUM) {
        return -toy_csr_size;
    } else if (from == ARG_POINTER_REGNUM) {
        return 0;
    }
    gcc_unreachable();
}

void toy_set_cfa_offset(int regno, int offset, rtx insn) {
    rtx reg = gen_rtx_REG(SImode, regno);
    rtx mem =
        gen_frame_mem(SImode, plus_constant(Pmode, stack_pointer_rtx, offset));
    rtx tmp = gen_rtx_SET(mem, reg);
    rtx dwarf = alloc_reg_note(REG_CFA_OFFSET, tmp, NULL_RTX);
    REG_NOTES(insn) = dwarf;
    RTX_FRAME_RELATED_P(insn) = 1;
}

void toy_set_def_cfa_offset(int offset, rtx insn, rtx reg) {
    rtx cfa_adjust_rtx = gen_rtx_PLUS(Pmode, reg, GEN_INT(offset));
    rtx dwarf = alloc_reg_note(REG_CFA_DEF_CFA, cfa_adjust_rtx, NULL_RTX);
    REG_NOTES(insn) = dwarf;
    RTX_FRAME_RELATED_P(insn) = 1;
}

void toy_set_cfa_restore(rtx reg, rtx insn) {
    rtx dwarf = alloc_reg_note(REG_CFA_RESTORE, reg, NULL_RTX);
    REG_NOTES(insn) = dwarf;
    RTX_FRAME_RELATED_P(insn) = 1;
}

void toy_expand_prologue() {
    toy_compute_frame();
    // addi    sp,sp,-32
    // sw      ra,28(sp)
    // sw      s0,24(sp)
    // addi    s0,sp,32
    // NOTE: adjust sp
    rtx insn = emit_move_insn(
        stack_pointer_rtx,
        gen_rtx_fmt_ee(
            PLUS, SImode, stack_pointer_rtx, GEN_INT(-toy_stack_size)));
    toy_set_def_cfa_offset(toy_stack_size, insn, stack_pointer_rtx);

    // NOTE: save CSRs
    int offset = toy_stack_size;
    for (int regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++) {
        if (toy_save_reg_p(regno)) {
            if (REGNO_REG_CLASS(regno) == GPR_REGS) {
                offset -= 4;
                rtx dst = gen_rtx_MEM(
                    SImode,
                    gen_rtx_fmt_ee(
                        PLUS, SImode, stack_pointer_rtx, GEN_INT(offset)));
                rtx insn = emit_move_insn(dst, gen_rtx_REG(SImode, regno));
                toy_set_cfa_offset(regno, offset, insn);
            } else if (REGNO_REG_CLASS(regno) == FPR_REGS) {
                offset -= 8;
                rtx dst = gen_rtx_MEM(
                    DFmode,
                    gen_rtx_fmt_ee(
                        PLUS, SImode, stack_pointer_rtx, GEN_INT(offset)));
                rtx insn = emit_move_insn(dst, gen_rtx_REG(DFmode, regno));
                toy_set_cfa_offset(regno, offset, insn);
            } else {
                gcc_unreachable();
            }
        }
    }
    // NOTE: set fp
    insn = emit_insn(gen_add3_insn(
        hard_frame_pointer_rtx, stack_pointer_rtx, GEN_INT(toy_stack_size)));
    if (cfun->calls_alloca) {
        toy_set_def_cfa_offset(0, insn, hard_frame_pointer_rtx);
    }
}

void toy_expand_epilogue() {
    int offset = toy_stack_size;
    for (int regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++) {
        if (toy_save_reg_p(regno)) {
            rtx insn = NULL_RTX;
            rtx reg = NULL_RTX;
            if (REGNO_REG_CLASS(regno) == GPR_REGS) {
                offset -= 4;
                rtx src = gen_rtx_MEM(
                    SImode,
                    gen_rtx_fmt_ee(
                        PLUS, SImode, stack_pointer_rtx, GEN_INT(offset)));
                reg = gen_rtx_REG(SImode, regno);
                insn = emit_move_insn(reg, src);
            } else if (REGNO_REG_CLASS(regno) == FPR_REGS) {
                offset -= 8;
                rtx src = gen_rtx_MEM(
                    DFmode,
                    gen_rtx_fmt_ee(
                        PLUS, SImode, stack_pointer_rtx, GEN_INT(offset)));
                reg = gen_rtx_REG(DFmode, regno);
                insn = emit_move_insn(reg, src);
            } else {
                gcc_unreachable();
            }
            toy_set_cfa_restore(reg, insn);
        }
    }
    rtx insn = emit_move_insn(
        stack_pointer_rtx,
        gen_rtx_fmt_ee(
            PLUS, SImode, stack_pointer_rtx, GEN_INT(toy_stack_size)));
    toy_set_def_cfa_offset(0, insn, stack_pointer_rtx);
    emit_jump_insn(gen_return());
}

static void toy_function_arg_advance(
    cumulative_args_t cum_v, const function_arg_info &arg) {}

#define TARGET_FUNCTION_ARG_ADVANCE toy_function_arg_advance

static rtx toy_function_arg(
    cumulative_args_t cum_v, const function_arg_info &arg) {
    CUMULATIVE_ARGS *cum = get_cumulative_args(cum_v);
    rtx ret = NULL_RTX;
    if (FLOAT_MODE_P(arg.mode) && TARGET_HARD_FLOAT) {
        if (FUNCTION_ARG_REGNO_P(FP_ARG_FIRST + cum->num_fprs)) {
            ret = gen_rtx_REG(arg.mode, FP_ARG_FIRST + cum->num_fprs);
            cum->num_fprs++;
        }
    } else {
        if (FUNCTION_ARG_REGNO_P(GP_ARG_FIRST + cum->num_gprs)) {
            int num_words =
                (GET_MODE_SIZE(arg.mode) + UNITS_PER_WORD - 1) / UNITS_PER_WORD;
            if (num_words != 1) {
                cum->num_gprs += cum->num_gprs & 1;
            }
            ret = gen_rtx_REG(arg.mode, GP_ARG_FIRST + cum->num_gprs);
            cum->num_gprs += num_words;
        }
    }
    return ret;
}

#define TARGET_FUNCTION_ARG toy_function_arg

rtx toy_function_value(
    const_tree valtype, const_tree fntype_or_decl,
    bool outgoing ATTRIBUTE_UNUSED) {
    if (FLOAT_MODE_P(TYPE_MODE(valtype)) && TARGET_HARD_FLOAT) {
        return gen_rtx_REG(TYPE_MODE(valtype), FP_ARG_FIRST);
    } else {
        return gen_rtx_REG(TYPE_MODE(valtype), GP_ARG_FIRST);
    }
}

#define TARGET_FUNCTION_VALUE toy_function_value

static rtx toy_libcall_value(
    machine_mode mode, const_rtx func ATTRIBUTE_UNUSED) {
    if (FLOAT_MODE_P(mode) && TARGET_HARD_FLOAT) {
        return gen_rtx_REG(mode, FP_ARG_FIRST);
    } else {
        return gen_rtx_REG(mode, GP_ARG_FIRST);
    }
}

#define TARGET_LIBCALL_VALUE toy_libcall_value

static bool toy_legitimate_constant_p(
    machine_mode mode ATTRIBUTE_UNUSED, rtx x) {
    switch (GET_CODE(x)) {
        case CONST_INT:
            return SMALL_OPERAND(INTVAL(x));
        case CONST_DOUBLE:
            return false;
    }
    return false;
}

#define TARGET_LEGITIMATE_CONSTANT_P toy_legitimate_constant_p

void toy_init_builtins(void) {
    tree BT_FN_VOID_PTR = build_function_type_list(ptr_type_node, NULL_TREE);
    add_builtin_function(
        "__builtin_getsp", BT_FN_VOID_PTR, 0, BUILT_IN_MD, NULL, NULL);
}

#define TARGET_INIT_BUILTINS toy_init_builtins

rtx toy_expand_builtin(
    tree exp, rtx target, rtx subtarget ATTRIBUTE_UNUSED,
    machine_mode mode ATTRIBUTE_UNUSED, int ignore ATTRIBUTE_UNUSED) {
    tree fndecl = TREE_OPERAND(CALL_EXPR_FN(exp), 0);
    unsigned int fcode = DECL_MD_FUNCTION_CODE(fndecl);

    switch (fcode) {
        case 0: {
            rtx tmp = gen_reg_rtx(SImode);
            emit_insn(GEN_FCN(CODE_FOR_getsp)(tmp));
            return tmp;
        }
        default:
            gcc_unreachable();
    }
}

#define TARGET_EXPAND_BUILTIN toy_expand_builtin

static bool toy_hard_regno_mode_ok(unsigned int regno, machine_mode mode) {
    if (FP_REG_P(regno) && (GET_MODE_CLASS(mode) != MODE_FLOAT)) {
        return false;
    }
    if (GP_REG_P(regno) && (GET_MODE_CLASS(mode) != MODE_INT)) {
        return false;
    }
    if (VECTOR_REG_P(regno) && (GET_MODE_CLASS(mode) != MODE_VECTOR_INT)) {
        return false;
    }
    return true;
}

#define TARGET_HARD_REGNO_MODE_OK toy_hard_regno_mode_ok

static bool toy_vector_mode_supported_p(machine_mode mode) {
    return mode == V4SImode || V8HImode || mode == V16QImode;
}

#define TARGET_VECTOR_MODE_SUPPORTED_P toy_vector_mode_supported_p

static void toy_conditional_register_usage(void) {
    if (!TARGET_HARD_FLOAT) {
        for (int regno = FP_REG_FIRST; regno <= FP_REG_LAST; regno++)
            fixed_regs[regno] = call_used_regs[regno] = 1;
    }
}

#define TARGET_CONDITIONAL_REGISTER_USAGE toy_conditional_register_usage

static machine_mode toy_preferred_simd_mode(scalar_mode mode) {
    if (mode == SImode) {
        return V4SImode;
    }
    if (mode == HImode) {
        return V8HImode;
    }
    if (mode == QImode) {
        return V16QImode;
    }
    return mode;
}
#define TARGET_VECTORIZE_PREFERRED_SIMD_MODE toy_preferred_simd_mode

struct gcc_target targetm = TARGET_INITIALIZER;
