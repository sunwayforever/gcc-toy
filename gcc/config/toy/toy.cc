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

static bool toy_legitimate_address_p(
    machine_mode mode, rtx x, bool strict_p) {
    return true;
}

#undef TARGET_LEGITIMATE_ADDRESS_P
#define TARGET_LEGITIMATE_ADDRESS_P toy_legitimate_address_p

struct gcc_target targetm = TARGET_INITIALIZER;
