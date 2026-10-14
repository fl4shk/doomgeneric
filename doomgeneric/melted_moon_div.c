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

u64 mm_udivdi3(u64 left, u64 right) {
    *mm_udiv64_inp_left_lo = (u32)left;
    *mm_udiv64_inp_left_hi = (u32)(left >> 32u);
    *mm_udiv64_inp_right_lo = (u32)right;
    *mm_udiv64_inp_right_hi = (u32)(right >> 32u);
    return (u64)(
        ((u64)(*mm_udiv64_outp_quot_lo))
        | ((u64)(*mm_udiv64_outp_quot_hi) << 32u)
    );
}
u64 mm_umoddi3(u64 left, u64 right) {
    *mm_udiv64_inp_left_lo = (u32)left;
    *mm_udiv64_inp_left_hi = (u32)(left >> 32u);
    *mm_udiv64_inp_right_lo = (u32)right;
    *mm_udiv64_inp_right_hi = (u32)(right >> 32u);
    return (u64)(
        ((u64)(*mm_udiv64_outp_rema_lo))
        | ((u64)(*mm_udiv64_outp_rema_hi) << 32u)
    );
}

i64 mm_divdi3(i64 left, i64 right) {
    *mm_idiv64_inp_left_lo = (u32)left;
    *mm_idiv64_inp_left_hi = (u32)(left >> 32u);
    *mm_idiv64_inp_right_lo = (u32)right;
    *mm_idiv64_inp_right_hi = (u32)(right >> 32u);
    return (u64)(
        ((u64)(*mm_idiv64_outp_quot_lo))
        | ((u64)(*mm_idiv64_outp_quot_hi) << 32u)
    );
}
i64 mm_moddi3(i64 left, i64 right) {
    *mm_idiv64_inp_left_lo = (u32)left;
    *mm_idiv64_inp_left_hi = (u32)(left >> 32u);
    *mm_idiv64_inp_right_lo = (u32)right;
    *mm_idiv64_inp_right_hi = (u32)(right >> 32u);
    return (u64)(
        ((u64)(*mm_idiv64_outp_rema_lo))
        | ((u64)(*mm_idiv64_outp_rema_hi) << 32u)
    );
}
#endif
