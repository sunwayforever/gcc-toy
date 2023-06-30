all:build

GCC_PREFIX ?= /opt/gcc-riscv/

GDB ?=
DEBUG ?=
RISCV ?=

MAIN_SRC := $(shell find ./ -name "*.main.c" -printf '%P\n')
MAIN_ELF := $(patsubst %.main.c,%.elf,${MAIN_SRC})
${MAIN_ELF}:%.elf:%.main.c

TEST_SRC := $(shell find ./ -name "*.c" -printf '%P\n')
TEST_SRC := $(filter-out ${MAIN_SRC}, ${TEST_SRC})
ifeq (${RISCV}, 1)
TEST_SRC := $(filter-out builtin.c, ${TEST_SRC})
endif
TEST_ASM := $(patsubst %.c,%.s,${TEST_SRC})
TEST_OBJ := $(patsubst %.c,%.o,${TEST_SRC})

RUN_ELF := $(patsubst %.c,%.elf,${TEST_SRC})
RUN_APP := $(patsubst %.elf, %,${RUN_ELF})

CC1FLAGS := -I${GCC_PREFIX}/lib/gcc/riscv64-unknown-linux-gnu/12.2.0/include/ \
			-I${GCC_PREFIX}/sysroot/usr/include -I${GCC_PREFIX}/sysroot/usr/include/linux \
			-D__riscv -D__riscv_xlen=32

ifeq (${RISCV}, 1)
CC1 := ../build_riscv/gcc/cc1
CC1FLAGS += -march=rv32g -mabi=ilp32d
${TEST_ASM}:riscv
else
CC1 := ../build/gcc/cc1
${TEST_ASM}:gcc
endif

DEBUG_FLAG := 2>/dev/null
ifeq (${DEBUG}, 1)
	DEBUG_FLAG := -fdump-rtl-all
endif

${TEST_ASM}:%.s:%.c
ifeq (${GDB}, 1)
	gdb.sh -e ${CC1} -a "${CC1FLAGS} -O0 $< -o $@ ${DEBUG_FLAG}"
else
	${CC1} ${CC1FLAGS} -O0 $< -o $@ ${DEBUG_FLAG}
endif

${RUN_ELF}:%.elf:%.s
	${GCC_PREFIX}/bin/riscv64-unknown-linux-gnu-gcc -march=rv32g -mabi=ilp32d \
		$^ -O0 -o $@ -static

${RUN_APP}:%:%.elf
	qemu-riscv32 $<

${TEST_OBJ}:%.o:%.s
	${GCC_PREFIX}/bin/riscv64-unknown-linux-gnu-gcc -c -o $@ $< && \
		${GCC_PREFIX}/bin/riscv64-unknown-linux-gnu-objdump -d $@

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
