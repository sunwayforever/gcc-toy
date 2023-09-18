all:build

GCC_PREFIX ?= /opt/gcc-riscv/

GDB ?=
DEBUG ?=
RISCV ?=
SOFT_FLOAT ?=
ARM ?=
ifeq (${SOFT_FLOAT}, 1)
	ARCH := rv32im
	ABI := ilp32
else
	ARCH := rv32g
	ABI := ilp32d
endif

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

# COMMON_CC1FLAGS :=
COMMON_CC1FLAGS := -fenable-tree-vect -ftree-loop-vectorize # -fdump-tree-vect-details

ifeq (${RISCV}, 1)
CC1 := ../build_riscv/gcc/cc1
CC1FLAGS := -I${GCC_PREFIX}/lib/gcc/riscv64-unknown-linux-gnu/12.2.0/include/ \
			-I${GCC_PREFIX}/sysroot/usr/include -I${GCC_PREFIX}/sysroot/usr/include/linux \
			-D__riscv -D__riscv_xlen=32 -fdwarf2-cfi-asm -march=${ARCH} -mabi=${ABI} \
			${COMMON_CC1FLAGS}
LD := ${GCC_PREFIX}/bin/riscv64-unknown-linux-gnu-gcc -march=rv32g -mabi=${ABI}
AS := ${GCC_PREFIX}/bin/riscv64-unknown-linux-gnu-as -march=rv32g
OBJDUMP := ${GCC_PREFIX}/bin/riscv64-unknown-linux-gnu-objdump
QEMU := /opt/qemu-7.1.0/bin/qemu-riscv32
${TEST_ASM}:riscv
else ifeq (${ARM}, 1)
CC1 := ../build_arm/gcc/cc1
CC1FLAGS := -I/usr/aarch64-linux-gnu/include -I/usr/aarch64-linux-gnu/include/linux \
			${COMMON_CC1FLAGS}
# 			-I /usr/lib/gcc-cross/aarch64-linux-gnu/9/include/
LD := aarch64-linux-gnu-gcc
AS := aarch64-linux-gnu-as
OBJDUMP := aarch64-linux-gnu-objdump
QEMU := /opt/qemu-7.1.0/bin/qemu-aarch64
${TEST_ASM}:arm
else
CC1 := ../build/gcc/cc1
CC1FLAGS := -I${GCC_PREFIX}/lib/gcc/riscv64-unknown-linux-gnu/12.2.0/include/ \
			-I${GCC_PREFIX}/sysroot/usr/include -I${GCC_PREFIX}/sysroot/usr/include/linux \
			-D__riscv -D__riscv_xlen=32 -fdwarf2-cfi-asm -mzbb -fenable-rtl-peephole2 ${COMMON_CC1FLAGS}
ifneq (${SOFT_FLOAT}, 1)
	CC1FLAGS += -mhard-float
endif
LD := ${GCC_PREFIX}/bin/riscv64-unknown-linux-gnu-gcc -march=rv32g -mabi=${ABI}
AS := ${GCC_PREFIX}/bin/riscv64-unknown-linux-gnu-as -march=rv32gv_zbb -mabi=${ABI}
OBJDUMP := ${GCC_PREFIX}/bin/riscv64-unknown-linux-gnu-objdump
QEMU := /opt/qemu-7.1.0/bin/qemu-riscv32 -cpu rv32,v=true,vlen=128,vext_spec=v1.0
${TEST_ASM}:gcc
endif

DEBUG_FLAG := 2>/dev/null
ifeq (${DEBUG}, 1)
	DEBUG_FLAG := -fdump-rtl-all -fdump-tree-all
endif
ifeq (${DEBUG}, 2)
	CC1FLAGS += -g
endif
ifeq (${GDB}, 2)
	CC1FLAGS += -g
endif

${TEST_ASM}:%.s:%.c
ifeq (${GDB}, 1)
	./gdb.sh -e ${CC1} -a "${CC1FLAGS} -O0 $< -o $@ ${DEBUG_FLAG}"
else
	${CC1} ${CC1FLAGS} -O0 $< -o $@ ${DEBUG_FLAG}
endif

${RUN_ELF}:%.elf:%.o
	${LD} $^ -O0 -o $@ -static -lgcc -lm

${RUN_APP}:%:%.elf
ifeq (${GDB}, 2)
	./qemu-gdb.sh $<
else
	${QEMU} $<
endif

${TEST_OBJ}:%.o:%.s
	${AS} -c -o $@ $< # && ${OBJDUMP} -d $@

${TEST_ASM}:FORCE

build:${TEST_ASM}

run:${RUN_APP}

clean:
	rm -f ${TEST_ASM} ${TEST_OBJ} ${RUN_ELF} *.c.*

FORCE:

gcc:FORCE
	DEFAULT_ASSEMBLER=${GCC_PREFIX}/bin/riscv64-unknown-linux-gnu-as ./toy_build.sh

riscv:FORCE
	./riscv_build.sh

arm:FORCE
	./arm_build.sh
