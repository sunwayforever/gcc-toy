all:build

GNU := ${HOME}/program/toolchain/riscv64-unknown-linux-gnu
GNU_PREFIX ?= ${GNU}/bin/riscv64-unknown-linux-gnu-

GDB ?=
DEBUG ?=
O ?=

ARCH := rv64g
ABI := lp64d

TEST_SRC := $(shell find ./ -name "*.c" -printf '%P\n')
TEST_SRC := $(filter-out ${MAIN_SRC}, ${TEST_SRC})
TEST_ASM := $(patsubst %.c,%.s,${TEST_SRC})
TEST_OBJ := $(patsubst %.c,%.o,${TEST_SRC})
TEST_ELF := $(patsubst %.c,%.elf,${TEST_SRC})
RUN_ELF := $(patsubst %.elf, run_%,${TEST_ELF})
DEBUG_ELF := $(patsubst %.elf,debug_%,${TEST_ELF})

DEBUG ?=
# DEBUG := -fdump-rtl-all -fdump-tree-all

COMMON_CC1FLAGS :=

CC1 := ../build_gcc/gcc/cc1
CC1FLAGS := -I${GNU}/lib/gcc/riscv64-unknown-linux-gnu/13.2.0/include/ \
			-I${GNU}/sysroot/usr/include -I${GNU}/sysroot/usr/include/linux \
			 -march=${ARCH} -mabi=${ABI} -O${O} \
			${COMMON_CC1FLAGS}
LD := ${GNU_PREFIX}gcc -march=${ARCH} -mabi=${ABI}
AS := ${GNU_PREFIX}as -march=${ARCH}
OBJDUMP := ${GNU_PREFIX}objdump
QEMU := qemu-riscv64

${TEST_ASM}:%.s:%.c
ifeq (${GDB}, 1)
	./gdb.sh -e ${CC1} -a "${CC1FLAGS} $< -o $@ ${DEBUG}"
else
	${CC1} ${CC1FLAGS} $< -o $@ ${DEBUG}
endif

${TEST_ELF}:%.elf:%.o
	${LD} $^ -o $@ -static -lgcc -lm

${RUN_ELF}:run_%:%.elf
	${QEMU} $<

${DEBUG_ELF}:debug_%:%.elf
	./qemu-gdb.sh $<

${TEST_OBJ}:%.o:%.s
	${AS} -c -o $@ $<

${TEST_ASM}:FORCE

build:${TEST_ASM}

clean:
	rm -f ${TEST_ASM} ${TEST_OBJ} ${TEST_ELF} *.c.*

FORCE:

gcc:FORCE
	./gcc_build.sh
