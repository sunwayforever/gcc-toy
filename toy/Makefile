all:build

TEST_SRC := $(shell find ./ -name "*.c" -printf '%P\n')
TEST_ASM := $(patsubst %.c,%.s,${TEST_SRC})

GDB ?=
DEBUG ?=

${TEST_ASM}:%.s:%.c
ifeq (${GDB}, 1)
	gdb.sh -e ../build/gcc/cc1 -a "-O0 $< -o $@" ${DEBUG}
else
	../build/gcc/cc1 -O0 $< -o $@ ${DEBUG}
endif

${TEST_ASM}:FORCE

build:${TEST_ASM}

clean:
	rm -f ${TEST_ASM} ${TEST_OBJ} *.c.*

FORCE:

gcc:FORCE
	./toy_build.sh
