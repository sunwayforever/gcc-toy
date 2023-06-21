all:build

TEST_SRC := $(shell find ./ -name "*.c" -printf '%P\n')
TEST_ASM := $(patsubst %.c,%.s,${TEST_SRC})

GDB ?=
DEBUG ?=
RISCV ?=

ifeq (${RISCV}, 1)
CC1 := ../build_riscv/gcc/cc1
else
CC1 := ../build/gcc/cc1
endif

${TEST_ASM}:%.s:%.c
ifeq (${GDB}, 1)
	gdb.sh -e ${CC1} -a "-O0 $< -o $@" ${DEBUG}
else
	${CC1} -O0 $< -o $@ ${DEBUG}
endif

${TEST_ASM}:FORCE

build:${TEST_ASM}

clean:
	rm -f ${TEST_ASM} ${TEST_OBJ} *.c.*

FORCE:

gcc:FORCE
	./toy_build.sh

riscv:FORCE
	./riscv_build.sh
