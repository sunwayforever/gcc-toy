# GCC Toy RISC-V Backend

This repository is used to show how to build a RISC-V backend step by step. 

## References

1. http://atgreen.github.io/ggx/

## Getting started

### build riscv64-unknown-linux-gnu-gcc

```
$> git clone https://github.com/riscv-collab/riscv-gnu-toolchain
$> cd /path/to/riscv-gnu-toolchain
$> configure --prefix=/opt/gcc-riscv --with-arch=rv64gc --with-abi=lp64d --enable-multilib
$> make linux
```

### build Toy backend and test

```
$> cd /path/to/gcc-toy/toy
$> make gcc
$> make 
$> make run
```
