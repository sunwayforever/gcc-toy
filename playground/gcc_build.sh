#!/bin/bash
N=`nproc`
mkdir ../build_gcc
pushd ../build_gcc
../configure --target=riscv64-unknown-elf --disable-shared --disable-threads \
             --enable-languages=c \
             --with-gnu-as=no \
             --with-gnu-ld=no \
             --with-system-zlib=no \
             --disable-libatomic --disable-libmudflap \
             --disable-libssp --disable-libquadmath \
             --disable-libgomp --disable-nls --disable-bootstrap --src=../
make inhibit-libc=true all-gcc -j ${N}
popd
