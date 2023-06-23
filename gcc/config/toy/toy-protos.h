// 2023-06-20 18:28
#ifndef TOY_PROTOS_H
#define TOY_PROTOS_H

extern void toy_asm_out_constructor(rtx, int);
extern void toy_asm_out_destructor(rtx, int);
extern HOST_WIDE_INT toy_initial_elimination_offset(int from, int to);
extern bool toy_legitimize_move(machine_mode mode, rtx dest, rtx src);
extern void toy_expand_int_scc(rtx target, int code, rtx op0, rtx op1);
#endif  // TOY_PROTOS_H
