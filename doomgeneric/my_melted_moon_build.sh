#!/bin/bash

#make -f Makefile.melted_moon_riscv32 clean \
#    && make -f Makefile.melted_moon_riscv32 -j8 \
#    && make -f Makefile.melted_moon_riscv32 -j8 dasm \
#    && make DO_TIMEDEMO_3=1 -f Makefile.melted_moon_riscv32 clean \
#    && make DO_TIMEDEMO_3=1 -f Makefile.melted_moon_riscv32 -j8 \
#    && make DO_TIMEDEMO_3=1 -f Makefile.melted_moon_riscv32 -j8 dasm \
#    && make DO_POLLINFO=1 -f Makefile.melted_moon_riscv32 clean \
#    && make DO_POLLINFO=1 -f Makefile.melted_moon_riscv32 -j8 \
#    && make DO_POLLINFO=1 -f Makefile.melted_moon_riscv32 -j8 dasm \
#    && make DO_TIMEDEMO_3=1 DO_POLLINFO=1 -f Makefile.melted_moon_riscv32 clean \
#    && make DO_TIMEDEMO_3=1 DO_POLLINFO=1 -f Makefile.melted_moon_riscv32 -j8 \
#    && make DO_TIMEDEMO_3=1 DO_POLLINFO=1 -f Makefile.melted_moon_riscv32 -j8 dasm \

make -f Makefile.melted_moon_riscv32 -j8 all \
    && make -f Makefile.melted_moon_riscv32 -j8 dasm \
    && make DO_TIMEDEMO_3=1 -f Makefile.melted_moon_riscv32 -j8 all \
    && make DO_TIMEDEMO_3=1 -f Makefile.melted_moon_riscv32 -j8 dasm \
    && make DO_POLLINFO=1 -f Makefile.melted_moon_riscv32 -j8 all \
    && make DO_POLLINFO=1 -f Makefile.melted_moon_riscv32 -j8 dasm \
    && make DO_TIMEDEMO_3=1 DO_POLLINFO=1 -f Makefile.melted_moon_riscv32 -j8 all \
    && make DO_TIMEDEMO_3=1 DO_POLLINFO=1 -f Makefile.melted_moon_riscv32 -j8 dasm

#make -f Makefile.melted_moon_riscv32 -j8 all \
#    && make DO_TIMEDEMO_3=1 -f Makefile.melted_moon_riscv32 -j8 all \
#    && make DO_POLLINFO=1 -f Makefile.melted_moon_riscv32 -j8 all \
#    && make DO_TIMEDEMO_3=1 DO_POLLINFO=1 -f Makefile.melted_moon_riscv32 -j8 all \
