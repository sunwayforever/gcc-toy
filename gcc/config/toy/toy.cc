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

void toy_print_operand(FILE *file, rtx x, int code) {
    rtx operand = x;

    switch (GET_CODE(operand)) {
        case REG:
            fprintf(file, "%s", reg_names[REGNO(operand)]);
            return;

        case MEM:
            output_address(VOIDmode, XEXP(operand, 0));
            return;

        default:
            if (CONSTANT_P(operand)) {
                output_addr_const(file, operand);
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

HOST_WIDE_INT
toy_initial_elimination_offset(int from, int to) { return 0; }

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
                            force_reg(GET_MODE(target), const0_rtx), target)));
            break;
        default:
            abort();
    }
}
struct gcc_target targetm = TARGET_INITIALIZER;
