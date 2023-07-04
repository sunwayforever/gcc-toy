#!/bin/bash
while getopts "b:a:e:" arg; do
    case $arg in
        b)
            bp="b $OPTARG"$'\n'"$bp"
            ;;
        a)
            args=$OPTARG
            ;;
        e)
            exec=$OPTARG
            ;;
    esac
done

if [[ -z "${exec}" || -z "${args}" ]]; then
    echo "usage gdb.sh -b <break> -a exec -a args"
fi

cat >/tmp/gdb.cmds <<hello
set cwd $PWD
set breakpoint pending on
set follow-fork-mode child
file $exec
set args $args
$bp
run
hello

pkill -9 -f /tmp/gdb.cmds
sleep 1
gdb -x /tmp/gdb.cmds
