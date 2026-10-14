//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Fixed point arithemtics, implementation.
//


#ifndef __M_FIXED__
#define __M_FIXED__




//
// Fixed point, 32bit as 16.16.
//
#define FRACBITS		16
#define FRACUNIT		(1<<FRACBITS)

typedef int fixed_t;

#include <stdio.h>
#include "stdlib.h"

#include "doomtype.h"
#include "i_system.h"

#include <stdint.h>

typedef uint8_t u8;
typedef int8_t i8;

typedef uint16_t u16;
typedef int16_t i16;

typedef uint32_t u32;
typedef int32_t i32;

typedef uint64_t u64;
typedef int64_t i64;

#ifdef MELTED_MOON_RISCV32
#define ADDR_UDIV64_INP_LEFT_LO ((u32)0x16000010ul)
#define ADDR_UDIV64_INP_LEFT_HI ((u32)0x16000014ul)
#define ADDR_UDIV64_INP_RIGHT_LO ((u32)0x16000018ul)
#define ADDR_UDIV64_INP_RIGHT_HI ((u32)0x1600001cul)
#define ADDR_UDIV64_OUTP_QUOT_LO ((u32)0x16000010ul)
#define ADDR_UDIV64_OUTP_QUOT_HI ((u32)0x16000014ul)
#define ADDR_UDIV64_OUTP_REMA_LO ((u32)0x16000018ul)
#define ADDR_UDIV64_OUTP_REMA_HI ((u32)0x1600001cul)

#define ADDR_IDIV64_INP_LEFT_LO ((u32)0x16000020ul)
#define ADDR_IDIV64_INP_LEFT_HI ((u32)0x16000024ul)
#define ADDR_IDIV64_INP_RIGHT_LO ((u32)0x16000028ul)
#define ADDR_IDIV64_INP_RIGHT_HI ((u32)0x1600002cul)
#define ADDR_IDIV64_OUTP_QUOT_LO ((u32)0x16000020ul)
#define ADDR_IDIV64_OUTP_QUOT_HI ((u32)0x16000024ul)
#define ADDR_IDIV64_OUTP_REMA_LO ((u32)0x16000028ul)
#define ADDR_IDIV64_OUTP_REMA_HI ((u32)0x1600002cul)

//#define ADDR_IMUL_FIXED_INP_LEFT ((u32)0x16000030ul)
//#define ADDR_IMUL_FIXED_INP_RIGHT ((u32)0x16000034ul)
//#define ADDR_IMUL_FIXED_OUTP_PROD ((u32)0x16000030ul)
//
//#define ADDR_IDIV_FIXED_INP_LEFT ((u32)0x16000038ul)
//#define ADDR_IDIV_FIXED_INP_RIGHT ((u32)0x1600003cul)
//#define ADDR_IDIV_FIXED_OUTP_QUOT ((u32)0x16000038ul)

static volatile u32* mm_udiv64_inp_left_lo = (
    (volatile u32*)(ADDR_UDIV64_INP_LEFT_LO)
);
static volatile u32* mm_udiv64_inp_left_hi = (
    (volatile u32*)(ADDR_UDIV64_INP_LEFT_HI)
);
static volatile u32* mm_udiv64_inp_right_lo = (
    (volatile u32*)(ADDR_UDIV64_INP_RIGHT_LO)
);
static volatile u32* mm_udiv64_inp_right_hi = (
    (volatile u32*)(ADDR_UDIV64_INP_RIGHT_HI)
);
static volatile u32* mm_udiv64_outp_quot_lo = (
    (volatile u32*)(ADDR_UDIV64_OUTP_QUOT_LO)
);
static volatile u32* mm_udiv64_outp_quot_hi = (
    (volatile u32*)(ADDR_UDIV64_OUTP_QUOT_HI)
);
static volatile u32* mm_udiv64_outp_rema_lo = (
    (volatile u32*)(ADDR_UDIV64_OUTP_REMA_LO)
);
static volatile u32* mm_udiv64_outp_rema_hi = (
    (volatile u32*)(ADDR_UDIV64_OUTP_REMA_HI)
);

static volatile u32* mm_idiv64_inp_left_lo = (
    (volatile u32*)(ADDR_IDIV64_INP_LEFT_LO)
);
static volatile u32* mm_idiv64_inp_left_hi = (
    (volatile u32*)(ADDR_IDIV64_INP_LEFT_HI)
);
static volatile u32* mm_idiv64_inp_right_lo = (
    (volatile u32*)(ADDR_IDIV64_INP_RIGHT_LO)
);
static volatile u32* mm_idiv64_inp_right_hi = (
    (volatile u32*)(ADDR_IDIV64_INP_RIGHT_HI)
);
static volatile u32* mm_idiv64_outp_quot_lo = (
    (volatile u32*)(ADDR_IDIV64_OUTP_QUOT_LO)
);
static volatile u32* mm_idiv64_outp_quot_hi = (
    (volatile u32*)(ADDR_IDIV64_OUTP_QUOT_HI)
);
static volatile u32* mm_idiv64_outp_rema_lo = (
    (volatile u32*)(ADDR_IDIV64_OUTP_REMA_LO)
);
static volatile u32* mm_idiv64_outp_rema_hi = (
    (volatile u32*)(ADDR_IDIV64_OUTP_REMA_HI)
);


//static volatile u32* mm_imul_fixed_inp_left = (
//    (volatile u32*)(ADDR_IMUL_FIXED_INP_LEFT)
//);
//static volatile u32* mm_imul_fixed_inp_right = (
//    (volatile u32*)(ADDR_IMUL_FIXED_INP_RIGHT)
//);
//static volatile u32* mm_imul_fixed_outp_prod = (
//    (volatile u32*)(ADDR_IMUL_FIXED_OUTP_PROD)
//);
//
//static volatile u32* mm_idiv_fixed_inp_left = (
//    (volatile u32*)(ADDR_IDIV_FIXED_INP_LEFT)
//);
//static volatile u32* mm_idiv_fixed_inp_right = (
//    (volatile u32*)(ADDR_IDIV_FIXED_INP_RIGHT)
//);
//static volatile u32* mm_idiv_fixed_outp_quot = (
//    (volatile u32*)(ADDR_IDIV_FIXED_OUTP_QUOT)
//);

static inline u64 mm_udivdi3(u64 left, u64 right) {
    *mm_udiv64_inp_left_lo = (u32)left;
    *mm_udiv64_inp_left_hi = (u32)(left >> 32u);
    *mm_udiv64_inp_right_lo = (u32)right;
    *mm_udiv64_inp_right_hi = (u32)(right >> 32u);
    return (u64)(
        ((u64)(*mm_udiv64_outp_quot_lo))
        | ((u64)(*mm_udiv64_outp_quot_hi) << 32u)
    );
}
static inline u64 mm_umoddi3(u64 left, u64 right) {
    *mm_udiv64_inp_left_lo = (u32)left;
    *mm_udiv64_inp_left_hi = (u32)(left >> 32u);
    *mm_udiv64_inp_right_lo = (u32)right;
    *mm_udiv64_inp_right_hi = (u32)(right >> 32u);
    return (u64)(
        ((u64)(*mm_udiv64_outp_rema_lo))
        | ((u64)(*mm_udiv64_outp_rema_hi) << 32u)
    );
}

static inline i64 mm_divdi3(i64 left, i64 right) {
    *mm_idiv64_inp_left_lo = (u32)left;
    *mm_idiv64_inp_left_hi = (u32)(left >> 32u);
    *mm_idiv64_inp_right_lo = (u32)right;
    *mm_idiv64_inp_right_hi = (u32)(right >> 32u);
    return (u64)(
        ((u64)(*mm_idiv64_outp_quot_lo))
        | ((u64)(*mm_idiv64_outp_quot_hi) << 32u)
    );
}
static inline i64 mm_moddi3(i64 left, i64 right) {
    *mm_idiv64_inp_left_lo = (u32)left;
    *mm_idiv64_inp_left_hi = (u32)(left >> 32u);
    *mm_idiv64_inp_right_lo = (u32)right;
    *mm_idiv64_inp_right_hi = (u32)(right >> 32u);
    return (u64)(
        ((u64)(*mm_idiv64_outp_rema_lo))
        | ((u64)(*mm_idiv64_outp_rema_hi) << 32u)
    );
}

//static inline u32 mm_fixed_mul(u32 rs1, u32 rs2) {
//    u32 rd;
//    __asm__ volatile (
//        ".insn r 0x0B, 0, 0, %0, %1, %2"
//        : "=r"(rd)              // output: rd register
//        : "r"(rs1), "r"(rs2)    // inputs: rs1, rs2
//        :                       // no clobbers
//    );
//    return rd;
//    //*mm_imul_fixed_inp_left = left;
//    //*mm_imul_fixed_inp_right = right;
//    //return (i32)(*mm_imul_fixed_outp_prod);
//}
static inline u32 mm_fixed_div(u32 rs1, u32 rs2) {
    u32 rd;
    __asm__ volatile (
        ".insn r 0x0B, 0, 1, %0, %1, %2"
        : "=r"(rd)              // output: rd register
        : "r"(rs1), "r"(rs2)    // inputs: rs1, rs2
        :                       // no clobbers
    );
    return rd;
    //*mm_idiv_fixed_inp_left = left;
    //*mm_idiv_fixed_inp_right = right;
    //return (i32)(*mm_idiv_fixed_outp_quot);
}
#endif

static inline fixed_t FixedMul	(fixed_t a, fixed_t b);
static inline fixed_t FixedDiv	(fixed_t a, fixed_t b);

// Fixme. __USE_C_FIXED__ or something.

static inline fixed_t
FixedMul
( fixed_t	a,
  fixed_t	b )
{
    //#ifdef MELTED_MOON_RISCV32
    //return (fixed_t) mm_fixed_mul(a, b);
    //#else
    return ((int64_t) a * (int64_t) b) >> FRACBITS;
    //#endif
}



//
// FixedDiv, C version.
//
static inline fixed_t FixedDiv(fixed_t a, fixed_t b)
{
    //#ifdef MELTED_MOON_RISCV32
    //return (fixed_t) mm_fixed_div(a, b);
    //#else
    if ((abs(a) >> 14) >= abs(b))
    {
        return (a^b) < 0 ? INT_MIN : INT_MAX;
    }
    else
    {
	int64_t result;

        #ifdef MELTED_MOON_RISCV32
        return (fixed_t) mm_fixed_div(a, b);
        #else
	result = ((int64_t) a << 16) / b;
	#endif

	return (fixed_t) result;
    }
    //#endif
}




#endif
