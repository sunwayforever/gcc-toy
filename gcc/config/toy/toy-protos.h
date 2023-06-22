// 2023-06-20 18:28
#ifndef TOY_PROTOS_H
#define TOY_PROTOS_H

extern void toy_asm_out_constructor(rtx, int);
extern void toy_asm_out_destructor(rtx, int);
extern HOST_WIDE_INT toy_initial_elimination_offset(int from, int to);
extern bool toy_legitimize_move(rtx dest, rtx src);
#endif  // TOY_PROTOS_H
