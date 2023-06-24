all:build

GCC_PREFIX ?= /opt/gcc-riscv/bin/riscv64-unknown-linux-gnu-

MAIN_SRC := $(shell find ./ -name "*.main.c" -printf '%P\n')
MAIN_ELF := $(patsubst %.main.c,%.elf,${MAIN_SRC})
${MAIN_ELF}:%.elf:%.main.c

TEST_SRC := $(shell find ./ -name "*.c" -printf '%P\n')
TEST_SRC := $(filter-out ${MAIN_SRC}, ${TEST_SRC})
TEST_ASM := $(patsubst %.c,%.s,${TEST_SRC})
TEST_OBJ := $(patsubst %.c,%.o,${TEST_SRC})

RUN_ELF := $(patsubst %.c,%.elf,${TEST_SRC})
RUN_APP := $(patsubst %.elf, %,${RUN_ELF})

GDB ?=
DEBUG ?=
RISCV ?=

ifeq (${RISCV}, 1)
CC1 := ../build_riscv/gcc/cc1
CC1FLAGS := -march=rv32g
else
CC1 := ../build/gcc/cc1
endif

${TEST_ASM}:%.s:%.c
ifeq (${GDB}, 1)
	gdb.sh -e ${CC1} -a "-O0 $< -o $@" ${DEBUG}
else
	${CC1} ${CC1FLAGS} -O0 $< -o $@ ${DEBUG}
endif

${RUN_ELF}:%.elf:%.s
	${GCC_PREFIX}gcc -march=rv32g -mabi=ilp32d $^ -O0 -o $@ -static

${RUN_APP}:%:%.elf
	qemu-riscv32 $<

${TEST_OBJ}:%.o:%.s
	${GCC_PREFIX}gcc -c -o $@ $< && ${GCC_PREFIX}objdump -d $@

${TEST_ASM}:FORCE

build:${TEST_ASM}

run:${RUN_APP}

clean:
	rm -f ${TEST_ASM} ${TEST_OBJ} ${RUN_ELF} *.c.*

FORCE:

gcc:FORCE
	./toy_build.sh

riscv:FORCE
	./riscv_build.sh
