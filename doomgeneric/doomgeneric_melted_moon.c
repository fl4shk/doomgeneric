#include "doomgeneric.h"
#include "doomkeys.h"
#include "i_video.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>

typedef uint8_t u8;
typedef int8_t i8;

typedef uint16_t u16;
typedef int16_t i16;

typedef uint32_t u32;
typedef int32_t i32;

typedef uint64_t u64;
typedef int64_t i64;

//extern int snprintf(char *__restrict __s, size_t __maxlen, const char *__restrict __format, ...);
//extern int vsnprintf(char *s, size_t n, const char *fmt, va_list ap);
//int printf(const char* fmt, ...);
//int puts(const char* s);

#include "../submodules/skinny_fs/skinny_fs.h"
#define MY_DBG_STDOUT_FILENAME "./my_dbg_stdout.txt"

#define ARR_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define FB_0_BASE 0x08000000ul//(1ul << 28ul)//0x0a000000ul//0x4000000ull //0x800000ull //0x1000000ull
#define FB_1_BASE 0x0a000000ul//((1ul << 28ul) | (1ul << 25ul)) //(FB_0_BASE + (320 * 240))
#define PAL_BASE 0x0c000000ul

#define FB_PAGE_BASE 0x0c000400ul
#define ADDR_DBG_PRINT ((u32)0xe000000ul)
#define ADDR_DBG_EXIT ((u32)0xe000004ul)
#define ADDR_TIMER_USEC_LO ((u32)0xe000000ul)
//#define ADDR_TIMER_USEC_HI ((u32)0xe000004ul)
#define ADDR_TIMER_SEC_LO ((u32)0xe000008ul)
//#define ADDR_TIMER_SEC_HI ((u32)0xe00000cul)
#define ADDR_POLL_INFO ((u32)0xe00000cul)

#define FULL_SCREENHEIGHT (240u)


#ifdef MELTED_MOON_RISCV32
#define ADDR_UDIV64_INP_LEFT_LO ((u32)0xe000010ul)
#define ADDR_UDIV64_INP_LEFT_HI ((u32)0xe000014ul)
#define ADDR_UDIV64_INP_RIGHT_LO ((u32)0xe000018ul)
#define ADDR_UDIV64_INP_RIGHT_HI ((u32)0xe00001cul)
#define ADDR_UDIV64_OUTP_QUOT_LO ((u32)0xe000010ul)
#define ADDR_UDIV64_OUTP_QUOT_HI ((u32)0xe000014ul)
#define ADDR_UDIV64_OUTP_REMA_LO ((u32)0xe000018ul)
#define ADDR_UDIV64_OUTP_REMA_HI ((u32)0xe00001cul)

#define ADDR_IDIV64_INP_LEFT_LO ((u32)0xe000020ul)
#define ADDR_IDIV64_INP_LEFT_HI ((u32)0xe000024ul)
#define ADDR_IDIV64_INP_RIGHT_LO ((u32)0xe000028ul)
#define ADDR_IDIV64_INP_RIGHT_HI ((u32)0xe00002cul)
#define ADDR_IDIV64_OUTP_QUOT_LO ((u32)0xe000020ul)
#define ADDR_IDIV64_OUTP_QUOT_HI ((u32)0xe000024ul)
#define ADDR_IDIV64_OUTP_REMA_LO ((u32)0xe000028ul)
#define ADDR_IDIV64_OUTP_REMA_HI ((u32)0xe00002cul)

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

u64 __udivdi3(u64 left, u64 right) {
    *mm_udiv64_inp_left_lo = (u32)left;
    *mm_udiv64_inp_left_hi = (u32)(left >> 32u);
    *mm_udiv64_inp_right_lo = (u32)right;
    *mm_udiv64_inp_right_hi = (u32)(right >> 32u);
    return (u64)(
        ((u64)(*mm_udiv64_outp_quot_lo))
        | ((u64)(*mm_udiv64_outp_quot_hi) << 32u)
    );
}
u64 __umoddi3(u64 left, u64 right) {
    *mm_udiv64_inp_left_lo = (u32)left;
    *mm_udiv64_inp_left_hi = (u32)(left >> 32u);
    *mm_udiv64_inp_right_lo = (u32)right;
    *mm_udiv64_inp_right_hi = (u32)(right >> 32u);
    return (u64)(
        ((u64)(*mm_udiv64_outp_rema_lo))
        | ((u64)(*mm_udiv64_outp_rema_hi) << 32u)
    );
}

i64 __divdi3(i64 left, i64 right) {
    *mm_idiv64_inp_left_lo = (u32)left;
    *mm_idiv64_inp_left_hi = (u32)(left >> 32u);
    *mm_idiv64_inp_right_lo = (u32)right;
    *mm_idiv64_inp_right_hi = (u32)(right >> 32u);
    return (u64)(
        ((u64)(*mm_idiv64_outp_quot_lo))
        | ((u64)(*mm_idiv64_outp_quot_hi) << 32u)
    );
}
i64 __moddi3(i64 left, i64 right) {
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

static volatile u8* _melted_moon_dbg_print = (
    (volatile u8*)(
        ADDR_DBG_PRINT
    )
);
static volatile u32* _melted_moon_dbg_exit = (
    (volatile u32*)(
        ADDR_DBG_EXIT
    )
);
static volatile u32* _melted_moon_timer_usec_lo = (
    (volatile u32*)(ADDR_TIMER_USEC_LO)
);
//static volatile u32* _melted_moon_timer_usec_hi = (
//    (volatile u32*)(ADDR_TIMER_USEC_HI)
//);
static volatile u32* _melted_moon_timer_sec_lo = (
    (volatile u32*)(ADDR_TIMER_SEC_LO)
);
//static volatile u32* _melted_moon_timer_sec_hi = (
//    (volatile u32*)(ADDR_TIMER_SEC_HI)
//);
static volatile u32* _melted_moon_poll_info = (
    (volatile u32*)(ADDR_POLL_INFO)
);

static volatile uint8_t* _melted_moon_fb_0 = (
    (volatile uint8_t*)FB_0_BASE
);
//static volatile uint8_t* _melted_moon_fb_1 = (
//    (volatile uint8_t*)FB_1_BASE
//);
static volatile u32* _melted_moon_fb_0_u32 = (
    (volatile u32*)FB_0_BASE
);
//static volatile u32* _melted_moon_fb_1_u32 = (
//    (volatile u32*)FB_1_BASE
//);
static volatile u32* _melted_moon_pal = (
    (volatile u32*)PAL_BASE
);
static volatile u32* _melted_moon_fb_page = (
    (volatile u32*)FB_PAGE_BASE
);
//static volatile uint16_t* _melted_moon_fb_0 = (
//    (volatile uint16_t*)FB_BASE
//);
//static volatile uint16_t* _melted_moon_fb_1 = (
//    ((volatile uint16_t*)FB_BASE) + FB_SIZE
//);

void my_doom_print(const char* str) {
    //void* my_dbg_stdout = tiny_fs_fopen(MY_DBG_STDOUT_FILENAME, "w");
    //tiny_fs_fwrite(my_dbg_stdout, str, sizeof(u8) * strlen(str));
    //tiny_fs_fclose(my_dbg_stdout);
    const int my_str_len = strlen(str);
    for (int i=0; i<my_str_len; ++i) {
        *_melted_moon_dbg_print = str[i];
    }
}
//#undef my_printf
//#define my_printf(...)
//    do {
//        enum {
//            MY_PRINTF_BUF_SIZE = 1024,
//        };
//        char my_printf_buf[MY_PRINTF_BUF_SIZE];
//        const int my_str_len = snprintf(my_printf_buf, MY_PRINTF_BUF_SIZE, __VA_ARGS__);
//        for (int i=0; i<my_str_len; ++i) {
//            *_melted_moon_dbg_print = my_printf_buf[i];
//        }
//    } while (0)

extern void my_doom_exit(int code);
//void my_doom_exit(int code) {
//    *_melted_moon_dbg_exit = code;
//    for (;;) {
//    }
//}

//void* fopen(const char* path, const char* mode) {
//    printf(
//        "attempting to open the file \"%s\" with mode \"%s\"\n",
//        path, mode
//    );
//    void* temp = skinny_fs_fopen(path, mode);
//    if (temp == NULL) {
//        printf(
//            "That file was *not found*!\n"
//        );
//    }
//    return temp;
//}
//int fclose(void* stream) {
//    skinny_fs_fclose(stream);
//    return 0;
//}
//size_t fread(
//    void* buf, size_t size, size_t n, void* stream
//) {
//    return skinny_fs_fread(stream, buf, size * n);
//}
//size_t fwrite(
//    const void* buf, size_t size, size_t n, void* stream
//) {
//    return skinny_fs_fwrite(stream, buf, size * n);
//}
//
//int fseek(
//    void* stream, long offset, int whence
//) {
//    return skinny_fs_fseek(stream, offset, whence);
//}
//
//long ftell(void* stream) {
//    return skinny_fs_ftell(stream);
//}
//
//int feof(void* stream)
//{
//    return skinny_fs_feof(stream);
//}
//
int rename(
    const char* old_filename, const char* new_filename
) {
    //printf(
    //    "rename(): DEBUG: old:\"%s\" new:\"%s\"\n",
    //    old_filename, new_filename
    //);
    return skinny_fs_rename(old_filename, new_filename);
}
//
//
////static inline int _my_other_fprintf_impl(void* stream, const char* fmt, ...) {
////    enum {
////        BUF_SIZE = 1024,
////    };
////    char buf[BUF_SIZE];
////    va_list ap;
////
////    va_start(ap, fmt);
////    const int i = vsnprintf(buf, BUF_SIZE, fmt, ap);
////    va_end(ap);
////    skinny_fs_fwrite(stream, buf, i);
////
////    return i;
////}
//
//
//int fprintf(void* stream, const char* fmt, ...) {
//    //enum {
//    //    BUF_SIZE = 1024,
//    //};
//    //char buf[BUF_SIZE];
//    //va_list ap;
//
//    //va_start(ap, fmt);
//    //const int i = vsnprintf(buf, BUF_SIZE, fmt, ap);
//    //va_end(ap);
//    //skinny_fs_fwrite(stream, buf, i);
//
//    return i;
//}
//
////static inline int _my_other_printf_impl(const char* fmt, ...) {
////    enum {
////        BUF_SIZE = 1024,
////    };
////    char buf[BUF_SIZE];
////    va_list ap;
////
////    va_start(ap, fmt);
////    const int my_str_len = vsnprintf(buf, BUF_SIZE, fmt, ap);
////    va_end(ap);
////
////    for (int i=0; i<my_str_len; ++i) {
////        *_melted_moon_dbg_print = buf[i];
////    }
////
////    return my_str_len;
////}
//
//int printf(const char* fmt, ...) {
//    //enum {
//    //    BUF_SIZE = 1024,
//    //};
//    //char buf[BUF_SIZE];
//    //va_list ap;
//
//    //va_start(ap, fmt);
//    //const int my_str_len = vsnprintf(buf, BUF_SIZE, fmt, ap);
//    //va_end(ap);
//
//    //for (int i=0; i<my_str_len; ++i) {
//    //    *_melted_moon_dbg_print = buf[i];
//    //}
//
//    //return my_str_len;
//    return 0;
//}


int mm_doom_putc(char c, FILE* file) {
    (void)file;
    *_melted_moon_dbg_print = c;
    return (int)c;
}

int mm_doom_getc(FILE* file) {
    (void)file;
    return (int)'a';
}
static FILE __stdio = FDEV_SETUP_STREAM(
    mm_doom_putc,
    mm_doom_getc,
    NULL,
    _FDEV_SETUP_RW
);

FILE* const stdout = &__stdio;
FILE* const stderr = &__stdio;
FILE* const stdin = &__stdio;

//int puts(const char* s) {
//    //return _my_other_printf_impl(s);
//    int ret = 0;
//    while (*s) {
//        *_melted_moon_dbg_print = *s;
//        ++s;
//        ++ret;
//    }
//    return ret;
//}

//int fputs(const char* s, void* stream) {
//    //return _my_other_fprintf_impl(stream, s);
//    enum {
//        BUF_SIZE = 1024u;
//    };
//    char buf[BUF_SIZE];
//    snprintf();
//
//}

int open(const char* path, int flags, ...) {
    //va_list ap;
    //va_start(ap, flags);

    //const char* mode = va_arg(ap, const char*);

    //va_end(ap);
    //printf(
    //    "open(): path:\"%s\" flags:%x mode:\"%s\"\n",
    //    path, flags, mode
    //);

    // the `...` is unused!
    if (flags == O_RDONLY) {
        //printf(
        //    "open(): O_RDONLY: path:\"%s\"\n",
        //    path
        //);
        intptr_t ret = (intptr_t)skinny_fs_fopen(path, "r");
        //printf(
        //    "open(): O_RDONLY: ret:%x\n",
        //    (int)ret
        //);
        return (int)ret;
    } else if (flags == O_WRONLY) {
        //printf(
        //    "open(): O_WRONLY: path:\"%s\"\n",
        //    path
        //);
        return (int)(intptr_t)skinny_fs_fopen(path, "w");
    } else {
        printf( 
            "open(): eek! %x\n",
            flags
        );
        return (int)(-1);
    }
    //return skinny_fs_fopen
}

int close(int fd) {
    // temporary!
    //printf(
    //    "close(): fd:0x%x\n",
    //    fd
    //);
    skinny_fs_fclose((void*)(intptr_t)fd);
    return 0;
};

off_t lseek(int fd, off_t offset, int whence) {
    off_t ret = skinny_fs_fseek((void*)(intptr_t)fd, offset, whence);
    //printf(
    //    "lseek(): fd:%x offset:%i whence:%i -> ret:%i\n",
    //    fd, (int)offset, whence, (int)ret
    //);
    return ret;
}

ssize_t read(int fd, void* buf, size_t count) {
    ssize_t ret = skinny_fs_fread((void*)(intptr_t)fd, buf, count);
    //printf(
    //    "read(): fd:%x buf:%p count:%u -> ret:%i\n",
    //    fd, buf, (unsigned)count, (int)ret
    //);
    return ret;
}
ssize_t write(int fd, void* buf, size_t count) {
    //return skinny_fs_fwrite((void*)(intptr_t)fd, buf, count);
    ssize_t ret = skinny_fs_fwrite((void*)(intptr_t)fd, buf, count);
    //printf(
    //    "write(): fd:%x buf:%p count:%u -> ret:%i\n",
    //    fd, buf, (unsigned)count, (int)ret
    //);
    return ret;
}

void _exit(int status) {
    for (;;) {
    }
}


extern u8 doom1_wad[];
extern u8 doom1_wad_end[];
extern u8 DEMO1_lmp[];
extern u8 DEMO1_lmp_end[];
extern u8 DEMO2_lmp[];
extern u8 DEMO2_lmp_end[];
extern u8 DEMO3_lmp[];
extern u8 DEMO3_lmp_end[];

void DG_Init() {
    //DG_ScreenBuffer = (pixel_t*)FB_0_BASE;

}

void DG_SetWindowTitle(const char* title) {
    // do nothing!
}


#define ARR_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))


//#define MELTED_MOON_TICRATE (35u)
//static inline i32 sleep_ms_helper(void) {
//    //return (
//    //    (u32)(*_melted_moon_timer_usec_lo) * 1000u
//    //);
//    i32 sec, usec;
//    i32 newtics;
//    static i32 basetime = 0;
//
//    //doom_gettime(&sec, &usec);
//    sec = *_melted_moon_timer_sec_lo;
//    usec = *_melted_moon_timer_usec_lo;
//    if (!basetime) {
//        basetime = sec;
//    }
//    newtics = (
//        //(
//        //    (sec - basetime) * MELTED_MOON_TICRATE + usec * MELTED_MOON_TICRATE / 1000000u
//        //)
//
//        //(sec - basetime) + usec / 1000000u
//        //((sec - basetime) + usec / 1000000u) // * 1000u
//        //((sec - basetime)) * 1000u
//        //* 1000u // multiply by 1000 to convert from seconds to milliseconds
//        //usec * 1000u
//        (
//            (
//                (sec * 100000)  // convert seconds to microseconds...
//                + usec          // ...then add `usec`
//            ) / 1000            // then convert to millseconds...
//        ) 
//    );
//    //printf(
//    //    "sleep_ms_helper(): basetime:%i sec:%i usec:%i -> newtics:%i\n",
//    //    basetime, sec, usec, newtics
//    //);
//    return newtics;
//}
void DG_SleepMs(u32 ms) {
    ////i32 basetime = sleep_ms_helper(); 

    ////i32 temp = 0;
    ////printf(
    ////    "(first) DG_SleepMs(ms:%i): basetime:%i temp:%i\n",
    ////    ms, basetime, temp
    ////);
    ////while (((temp = sleep_ms_helper()) - basetime) < ms) {
    ////    // sleep!
    ////    //printf("\n");
    ////    printf(
    ////        "DG_SleepMs(ms:%i): basetime:%i temp:%i subtract:%i\n",
    ////        ms, basetime, temp, temp - basetime
    ////    );
    ////}


    ////return (
    ////    (u32)(*_melted_moon_timer_usec_lo) * 1000u
    ////);
    ////i32 sec, usec;
    //i32 newtics;
    //static i32 basetime = 0;

    ////doom_gettime(&sec, &usec);
    ////sec = *_melted_moon_timer_sec_lo;
    ////usec = *_melted_moon_timer_usec_lo;
    ////if (!basetime) {
    ////    basetime = sec;
    ////}
    //basetime = (
    //    (
    //        ((*_melted_moon_timer_sec_lo) * 100000)  // convert seconds to microseconds...
    //        + (*_melted_moon_timer_usec_lo)          // ...then add `usec`
    //    ) / 1000                                     // then convert to millseconds...
    //);
    //while (
    //    (
    //        (
    //            newtics = (
    //                (
    //                    ((*_melted_moon_timer_sec_lo) * 100000)  // convert seconds to microseconds...
    //                    + (*_melted_moon_timer_usec_lo)          // ...then add `usec`
    //                ) / 1000                                     // then convert to millseconds...
    //            ) 
    //        ) - basetime 
    //    ) < ms //* 1000
    //) {
    //    //printf(
    //    //    "DG_SleepMs(ms:%i): basetime:%i sec:%i usec:%i -> newtics:%i\n",
    //    //    ms, basetime, sec, usec, newtics
    //    //);
    //    //*_melted_moon_dbg_print = '\0';
    //    //printf("testificate\n");
    //}
    ////printf(
    ////    "sleep_ms_helper(): basetime:%i sec:%i usec:%i -> newtics:%i\n",
    ////    basetime, sec, usec, newtics
    ////);
    ////return newtics;

    ////printf("testificate\n");
    u32 basetime = DG_GetTicksMs();
    while ((DG_GetTicksMs() - basetime) < ms) {
        printf("");
    }
}


//static u32 _ticks = 0;

u32 DG_GetTicksMs() {
    //_ticks += 3;
    //++_ticks;
    //printf(
    //    "DG_GetTicksMs(): returning %u\n",
    //    (unsigned)_ticks
    //);
    //return _ticks;
    //static u32 basetime = 0u;
    //if (basetime == 0u) {
    //    basetime = (
    //        (
    //            ((*_melted_moon_timer_sec_lo) * 100000u)    // convert seconds to microseconds...
    //            + (*_melted_moon_timer_usec_lo)             // ...then add `usec`
    //        )
    //    );
    //    return basetime; /// 1000u;                            // then convert to millseconds...
    //}
    return (
        (
            (
                ((*_melted_moon_timer_sec_lo) * 1000000u)   // convert seconds to microseconds...
                + (*_melted_moon_timer_usec_lo)             // ...then add `usec`
            ) / 1000u                                       // then convert to millseconds...
        )
    );
}

static bool _which_frame = true;

extern boolean palette_changed;
extern struct color colors[256];

static inline void _my_set_rgb888_palette(void) {
    if (palette_changed) {
        for (size_t i=0; i<ARR_SIZE(colors); ++i) {
            _melted_moon_pal[i] = (
                (colors[i].b << 16u)
                | (colors[i].g << 8u)
                | (colors[i].r << 0u)
            );
        }
        palette_changed = false;
    }
}


// "J1,A,B,X,Y,LT,RT,Select,Start;",

static const int _key_map_arr[] = {
    //--------
    KEY_RIGHTARROW,     // button: right
    KEY_LEFTARROW,      // button: left
    KEY_DOWNARROW,      // button: down
    KEY_UPARROW,        // button: up
    ////--------
    KEY_FIRE,           // fire gun (button: A)
    KEY_USE,            // action (open door, press switch, etc.) (button: B)
    KEY_ENTER,          // menu accept (button: X)
    KEY_FIRE,           // fire gun (button: Y)
    //--------
    KEY_STRAFE_L,       // strafe left (button: LT)
    KEY_STRAFE_R,       // strafe right (button: RT)
    //--------
    KEY_TAB,            // bring up map (button: select)
    KEY_ESCAPE,         // pause (button: start)
};
static const size_t _key_map_arr_size = ARR_SIZE(_key_map_arr);

static u32 _joystick_0 = 0x0;

#define KEYQUEUE_SIZE 16

static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;


static inline void addKeyToQueue(int pressed, unsigned int keyCode) {
	//unsigned char key = convertToDoomKey(keyCode);
	unsigned char key = _key_map_arr[keyCode];

	unsigned short keyData = (pressed << 8) | key;

	s_KeyQueue[s_KeyQueueWriteIndex] = keyData;
	s_KeyQueueWriteIndex++;
	s_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}

int DG_GetKey(int* pressed, unsigned char* doomKey) {
	if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex) {
		//key queue is empty

		return 0;
	} else {
		unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
		s_KeyQueueReadIndex++;
		s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

		*pressed = keyData >> 8;
		*doomKey = keyData & 0xFF;

		return 1;
	}
}

void DG_DrawFrame() {
    _my_set_rgb888_palette();

    #ifdef MELTED_MOON_HAVE_POLL_INFO
    u32 temp_poll_info = *_melted_moon_poll_info;
    if ((temp_poll_info & ((1u << _key_map_arr_size) - 1u)) != 0x0u) {
        for (u32 key_idx=0u; key_idx<_key_map_arr_size; ++key_idx) {
            const u32 mask = 1u << key_idx;
            if (
                //temp_poll_info & mask
                (!(_joystick_0 & mask))
                && (temp_poll_info & mask)
            ) {
                addKeyToQueue(1, key_idx);
            } else if (
                (_joystick_0 & mask)
                && (!(temp_poll_info & mask))
            ) {
                addKeyToQueue(0, key_idx);
            }
        }
    } 
    _joystick_0 = temp_poll_info & ((1u << _key_map_arr_size) - 1u);

    *_melted_moon_poll_info = temp_poll_info;
    if (temp_poll_info & (1u << 31u)) { // are we already in VBlank?
    } else {
        while (!((temp_poll_info = *_melted_moon_poll_info) & (1u << 31u))) { // wait for VBlank
            printf("");
        }
        *_melted_moon_poll_info = 1u << 31u;
    }
    #endif      // MELTED_MOON_HAVE_POLL_INFO

    *_melted_moon_fb_page = (u32)_which_frame;
    //++_ticks;
    _which_frame = !_which_frame;
    if (!_which_frame) {
        DG_ScreenBuffer = (pixel_t*)FB_0_BASE;
    } else {
        DG_ScreenBuffer = (pixel_t*)FB_1_BASE;
    }
}

//static inline void my_fb_test_do_it(int argc, char** argv) {
//    //for (size_t i=0; i<6; i+=6) {
//        colors[0].r = 0xff;
//        colors[0].g = 0xa0;
//        colors[0].b = 0x00;
//
//        colors[1].r = 0xff;
//        colors[1].g = 0xff;
//        colors[1].b = 0xff;
//
//        colors[2].r = 0xff;
//        colors[2].g = 0x00;
//        colors[2].b = 0xff;
//
//        colors[3].r = 0x00;
//        colors[3].g = 0xa0;
//        colors[3].b = 0xff;
//    //}
//    _my_set_rgb888_palette();
//    _which_frame = false;
//    for (;;) {
//        #ifdef MELTED_MOON_HAVE_POLL_INFO
//        if ((*_melted_moon_poll_info) & (1u << 31u)) 
//        #endif      // MELTED_MOON_HAVE_POLL_INFO
//        {
//            #ifdef MELTED_MOON_HAVE_POLL_INFO
//            *_melted_moon_poll_info = (1u << 31u);//0x1u;
//            #endif      // MELTED_MOON_HAVE_POLL_INFO
//            //const unsigned char* my_screen_buf = doom_get_framebuffer(1); //screens()[0]; //
//
//            //memcpy(_melted_moon_fb_0, my_screen_buf, SCREENWIDTH * SCREENHEIGHT);
//            //for (size_t i=0; i<SCREENWIDTH * SCREENHEIGHT; i+=8) {
//            //    //_melted_moon_fb_0[i] = _rgb555_pal[my_screen_buf[i]];
//            //    _melted_moon_fb_0[i+0] = 0x0; //my_screen_buf[i+0];
//            //    _melted_moon_fb_0[i+1] = 0x0; //my_screen_buf[i+1];
//            //    _melted_moon_fb_0[i+2] = 0x0; //my_screen_buf[i+2];
//            //    _melted_moon_fb_0[i+3] = 0x0; //my_screen_buf[i+3];
//            //    _melted_moon_fb_0[i+4] = 0x1; //my_screen_buf[i+4];
//            //    _melted_moon_fb_0[i+5] = 0x1; //my_screen_buf[i+5];
//            //    _melted_moon_fb_0[i+6] = 0x1; //my_screen_buf[i+6];
//            //    _melted_moon_fb_0[i+7] = 0x1; //my_screen_buf[i+7];
//            //}
//
//            for (
//                size_t i=0;
//                i<SCREENWIDTH * SCREENHEIGHT;
//                i+=4
//            ) {
//                if (!_which_frame) {
//                    _melted_moon_fb_0_u32[(i >> 2u) + 0] = (
//                        //(my_screen_buf[i+0] << 0u)
//                        //| (my_screen_buf[i+1] << 8u)
//                        //| (my_screen_buf[i+2] << 16u)
//                        //| (my_screen_buf[i+3] << 24u)
//                        (((i & 0x3) + 0u) << 0u)
//                        | (((i & 0x3) + 1u) << 8u)
//                        | (((i & 0x3) + 2u) << 16u)
//                        | (((i & 0x3) + 3u) << 24u)
//                    );
//                } else {
//                    _melted_moon_fb_0_u32[(i >> 2u) + 0] = (
//                        //(my_screen_buf[i+0] << 0u)
//                        //| (my_screen_buf[i+1] << 8u)
//                        //| (my_screen_buf[i+2] << 16u)
//                        //| (my_screen_buf[i+3] << 24u)
//
//                        //(((i & 0x3) + 0u) << 0u)
//                        //| (((i & 0x3) + 1u) << 8u)
//                        //| (((i & 0x3) + 2u) << 16u)
//                        //| (((i & 0x3) + 3u) << 24u)
//                        //0u
//                        (((i & 0x3) + 0u) << 0u)
//                        | (((i & 0x3) + 1u) << 8u)
//                        | (((i & 0x3) + 0u) << 16u)
//                        | (((i & 0x3) + 1u) << 24u)
//                    );
//                }
//            }
//            //if (!_which_frame) {
//            //    // for the emulator
//            //    _melted_moon_fb_1[(SCREENWIDTH * FULL_SCREENHEIGHT) - 1] = 0u;
//            //} else {
//            //    // for the emulator
//            //    _melted_moon_fb_0[(SCREENWIDTH * FULL_SCREENHEIGHT) - 1] = 0u;
//            //}
//            *_melted_moon_fb_page = !_which_frame;
//            _which_frame = !_which_frame;
//        }
//    }
//    //_melted_moon_fb_0[0] = 0x7c1f;
//
//    //for (uint32_t i=1; i<SCREENWIDTH; ++i) {
//    //    _melted_moon_fb_0[i] = _melted_moon_fb_0[i - 1] + 1;
//    //}
//
//    //while (true) {
//    //}
//}

int main(int argc, char** argv) {
    const u32 doom1_wad_size = doom1_wad_end - doom1_wad;
    const u32 DEMO1_lmp_size = DEMO1_lmp_end - DEMO1_lmp;
    const u32 DEMO2_lmp_size = DEMO2_lmp_end - DEMO2_lmp;
    const u32 DEMO3_lmp_size = DEMO3_lmp_end - DEMO3_lmp;
    //*_melted_moon_dbg_print = '1';

    char* my_argv[] = {
        "doom.exe",
        "-iwad",
        "doom1.wad",
        //"-timedemo",
        //"DEMO1"
        #ifdef MELTED_MOON_DOOM_TIMEDEMO_3
        "-timedemo",
        "DEMO3"
        #endif      // MELTED_MOON_DOOM_TIMEDEMO_3
    };
    int my_argc = sizeof(my_argv) / sizeof(my_argv[0]);

    {
        //--------
        skinny_fs_handle_t* temp_file = NULL;
        //--------
        temp_file = (skinny_fs_handle_t*)skinny_fs_file_init(
            "doom1.wad",
            doom1_wad,
            doom1_wad_size
        );
        printf(
            "doom1.wad: handle:%x file:%x\n",
            (u32)temp_file, (u32)temp_file->f
        );
        skinny_fs_fclose((void*)temp_file);
        //--------
        temp_file = (skinny_fs_handle_t*)skinny_fs_file_init(
            "DEMO1.lmp",
            DEMO1_lmp,
            DEMO1_lmp_size
        );
        printf(
            "DEMO1.lmp: handle:%x file:%x\n",
            (u32)temp_file, (u32)temp_file->f
        );
        skinny_fs_fclose((void*)temp_file);
        //--------
        temp_file = (skinny_fs_handle_t*)skinny_fs_file_init(
            "DEMO2.lmp",
            DEMO2_lmp,
            DEMO2_lmp_size
        );
        printf(
            "DEMO2.lmp: handle:%x file:%x\n",
            (u32)temp_file, (u32)temp_file->f
        );
        skinny_fs_fclose((void*)temp_file);
        //--------
        temp_file = (skinny_fs_handle_t*)skinny_fs_file_init(
            "DEMO3.lmp",
            DEMO3_lmp,
            DEMO3_lmp_size
        );
        printf(
            "DEMO3.lmp: handle:%x file:%x\n",
            (u32)temp_file, (u32)temp_file->f
        );
        skinny_fs_fclose((void*)temp_file);
        //--------
    }
    DG_ScreenBuffer = (pixel_t*)FB_1_BASE;
    ////my_fb_test_do_it(argc, argv);
    doomgeneric_Create(my_argc, my_argv);

    for (;;) {
        doomgeneric_Tick();
    }

    return 0;
}
