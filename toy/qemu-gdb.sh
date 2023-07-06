#!/bin/bash
if [[ $# == 0 ]]; then
    echo "usage qemu-gdb.sh <exec>"
    exit 1
fi

app=$1

cat >/tmp/gdb.cmds <<hello
target remote localhost:12345
hello

/opt/qemu-7.1.0/bin/qemu-riscv32 -cpu rv32,v=true,vlen=128,vext_spec=v1.0 -g 12345 ./$app &

qemu_pid=$!

/opt/gcc-riscv/bin/riscv64-unknown-linux-gnu-gdb $app -x /tmp/gdb.cmds

kill -9 $qemu_pid &>/dev/null
