all:build

GCC_PREFIX ?= /opt/gcc-riscv/bin/riscv64-unknown-linux-gnu-

TEST_SRC := $(shell find ./ -name "*.c" -printf '%P\n')
TEST_ASM := $(patsubst %.c,%.s,${TEST_SRC})
TEST_OBJ := $(patsubst %.c,%.o,${TEST_SRC})

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

${TEST_OBJ}:%.o:%.s
	${GCC_PREFIX}gcc -c -o $@ $< && ${GCC_PREFIX}objdump -d $@

${TEST_ASM}:FORCE

build:${TEST_ASM}

clean:
	rm -f ${TEST_ASM} ${TEST_OBJ} *.c.*

FORCE:

gcc:FORCE
	./toy_build.sh

riscv:FORCE
	./riscv_build.sh
