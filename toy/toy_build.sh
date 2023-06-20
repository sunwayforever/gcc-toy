#!/bin/bash
N=`nproc`
if [[ -d "../build" ]]; then
    pushd ../build
    make inhibit-libc=true all-gcc -j $N
    popd
else
    mkdir ../build
    pushd ../build
    ../configure --target=toy-elf --disable-shared --disable-threads \
                 --enable-languages=c \
                 --with-gnu-as=no \
                 --with-gnu-ld=no \
                 --with-system-zlib=no \
                 --disable-libatomic --disable-libmudflap \
                 --disable-libssp --disable-libquadmath \
                 --disable-libgomp --disable-nls --disable-bootstrap --src=../
    make inhibit-libc=true all-gcc -j $N
    popd
fi
