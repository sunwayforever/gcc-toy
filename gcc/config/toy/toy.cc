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
// clang-format on

const enum reg_class toy_regno_to_class[FIRST_PSEUDO_REGISTER] = {
    GPR_REGS, GPR_REGS, GPR_REGS, GPR_REGS, GPR_REGS, GPR_REGS,
    GPR_REGS, GPR_REGS, GPR_REGS, GPR_REGS, GPR_REGS, GPR_REGS,
    GPR_REGS, GPR_REGS, GPR_REGS, GPR_REGS, GPR_REGS, GPR_REGS,

    FPR_REGS, FPR_REGS, FPR_REGS, FPR_REGS, FPR_REGS, FPR_REGS,
    FPR_REGS, FPR_REGS, FPR_REGS, FPR_REGS, FPR_REGS, FPR_REGS,
    FPR_REGS, FPR_REGS, FPR_REGS, FPR_REGS, FPR_REGS, FPR_REGS,
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

bool toy_legitimize_move(machine_mode mode, rtx dst, rtx src) {
    // (mov mem (const 1))
    bool legitimize = false;
    if (GET_CODE(dst) == MEM) {
        rtx symbol = XEXP(dst, 0);
        if (GET_CODE(symbol) == SYMBOL_REF) {
            dst = gen_reg_rtx(mode);
            emit_move_insn(dst, symbol);
            dst = gen_rtx_MEM(mode, dst);
            legitimize = true;
        }
        if (GET_CODE(symbol) == CONST) {
            rtx plus = XEXP(symbol, 0);
            if (GET_CODE(XEXP(plus, 0)) == SYMBOL_REF) {
                rtx tmp = gen_reg_rtx(mode);
                emit_move_insn(tmp, XEXP(plus, 0));
                XEXP(plus, 0) = tmp;
                XEXP(dst, 0) = plus;
                legitimize = true;
            }
        }
        if (GET_CODE(src) != REG) {
            src = force_reg(mode, src);
            legitimize = true;
        }
        if (legitimize) {
            emit_move_insn(dst, src);
            return true;
        }
    }

    if (GET_CODE(src) == MEM) {
        rtx symbol = XEXP(src, 0);
        if (GET_CODE(symbol) == SYMBOL_REF) {
            src = gen_reg_rtx(mode);
            emit_move_insn(src, symbol);
            src = gen_rtx_MEM(mode, src);
            legitimize = true;
        }
        if (GET_CODE(symbol) == CONST) {
            rtx plus = XEXP(symbol, 0);
            if (GET_CODE(XEXP(plus, 0)) == SYMBOL_REF) {
                rtx tmp = gen_reg_rtx(mode);
                emit_move_insn(tmp, XEXP(plus, 0));
                XEXP(plus, 0) = tmp;
                XEXP(src, 0) = plus;
                legitimize = true;
            }
        }
        if (legitimize) {
            emit_move_insn(dst, src);
            return true;
        }
    }

    return false;
}

void toy_expand_int_scc(rtx target, int code, rtx op0, rtx op1) {
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

static int toy_callee_saved_reg_size;
static int toy_local_vars_size;
static int toy_stack_size;

static bool toy_save_reg_p(unsigned int regno) {
    bool call_saved = !global_regs[regno] && !call_used_or_fixed_reg_p(regno);
    bool might_clobber =
        crtl->saves_all_registers || df_regs_ever_live_p(regno);
    if (call_saved && might_clobber) return true;
    if (regno == HARD_FRAME_POINTER_REGNUM) return true;
    if (regno == RETURN_ADDR_REGNUM && crtl->calls_eh_return) return true;

    return false;
}

static void toy_compute_frame(void) {
    int stack_alignment = STACK_BOUNDARY / BITS_PER_UNIT;
    toy_local_vars_size = (int)get_frame_size();
    int padding_locals = toy_local_vars_size % stack_alignment;
    if (padding_locals) padding_locals = stack_alignment - padding_locals;
    toy_local_vars_size += padding_locals;
    toy_callee_saved_reg_size = 0;
    for (int regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++) {
        if (toy_save_reg_p(regno)) toy_callee_saved_reg_size += 4;
    }
    toy_stack_size = toy_local_vars_size + toy_callee_saved_reg_size;
}

HOST_WIDE_INT
toy_initial_elimination_offset(int ARG_UNUSED(from), int ARG_UNUSED(to)) {
    toy_compute_frame();
    return -toy_stack_size;
}

void toy_expand_prologue() {
    toy_compute_frame();
    // addi    sp,sp,-32
    // sw      ra,28(sp)
    // sw      s0,24(sp)
    // addi    s0,sp,32
    // NOTE: adjust sp
    emit_move_insn(
        stack_pointer_rtx,
        gen_rtx_fmt_ee(
            PLUS, SImode, stack_pointer_rtx, GEN_INT(-toy_stack_size)));

    // NOTE: save CSRs
    int offset = toy_stack_size;
    for (int regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++) {
        if (toy_save_reg_p(regno)) {
            offset -= 4;
            rtx dst = gen_rtx_MEM(
                SImode, gen_rtx_fmt_ee(
                            PLUS, SImode, stack_pointer_rtx, GEN_INT(offset)));
            emit_move_insn(dst, gen_rtx_REG(SImode, regno));
        }
    }
    // NOTE: set fp
    emit_insn(gen_add3_insn(
        hard_frame_pointer_rtx, stack_pointer_rtx, GEN_INT(toy_stack_size)));
}

void toy_expand_epilogue() {
    int offset = toy_stack_size;
    for (int regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++) {
        if (toy_save_reg_p(regno)) {
            offset -= 4;
            rtx src = gen_rtx_MEM(
                SImode, gen_rtx_fmt_ee(
                            PLUS, SImode, stack_pointer_rtx, GEN_INT(offset)));
            emit_move_insn(gen_rtx_REG(SImode, regno), src);
        }
    }
    emit_move_insn(
        stack_pointer_rtx,
        gen_rtx_fmt_ee(
            PLUS, SImode, stack_pointer_rtx, GEN_INT(toy_stack_size)));
    // RTX_FRAME_RELATED_P(insn) = 1;
    emit_jump_insn(gen_return());
    // RTX_FRAME_RELATED_P(insn) = 1;
}

static void toy_function_arg_advance(
    cumulative_args_t cum_v, const function_arg_info &arg) {}

#define TARGET_FUNCTION_ARG_ADVANCE toy_function_arg_advance

static rtx toy_function_arg(
    cumulative_args_t cum_v, const function_arg_info &arg) {
    CUMULATIVE_ARGS *cum = get_cumulative_args(cum_v);
    rtx ret = gen_rtx_REG(arg.mode, GP_ARG_FIRST + cum->num_gprs);
    cum->num_gprs++;
    return ret;
}

#define TARGET_FUNCTION_ARG toy_function_arg

rtx toy_function_value(
    const_tree valtype, const_tree fntype_or_decl,
    bool outgoing ATTRIBUTE_UNUSED) {
    return gen_rtx_REG(TYPE_MODE(valtype), GP_ARG_FIRST);
}

#define TARGET_FUNCTION_VALUE toy_function_value

struct gcc_target targetm = TARGET_INITIALIZER;
