#ifndef DOOM_GENERIC
#define DOOM_GENERIC

#include <stdlib.h>
#include <stdint.h>

#ifndef DOOMGENERIC_RESX
#define DOOMGENERIC_RESX 640
#endif  // DOOMGENERIC_RESX

#ifndef DOOMGENERIC_RESY
#define DOOMGENERIC_RESY 400
#endif  // DOOMGENERIC_RESY


#ifdef CMAP256

typedef uint8_t pixel_t;

#else  // CMAP256

typedef uint32_t pixel_t;

#endif  // CMAP256


//#define FB_0_BASE 0x08000000ul//(1ul << 28ul)//0x0a000000ul//0x4000000ull //0x800000ull //0x1000000ull
//#define FB_1_BASE 0x0a000000ul//((1ul << 28ul) | (1ul << 25ul)) //(FB_0_BASE + (320 * 240))
//#define PAL_BASE 0x0c000000ul
//
//#define FB_PAGE_BASE 0x0c000400ul

//#define FB_0_BASE 0x08000000ul//(1ul << 28ul)//0x0a000000ul//0x4000000ull //0x800000ull //0x1000000ull
//#define FB_1_BASE 0x0a000000ul//((1ul << 28ul) | (1ul << 25ul)) //(FB_0_BASE + (320 * 240))
//#define PAL_BASE 0x0c000000ul
//
//#define FB_PAGE_BASE 0x0c000400ul

#define FB_0_BASE 0x10000000ul//(1ul << 28ul)//0x0a000000ul//0x4000000ull //0x800000ull //0x1000000ull
#define FB_1_BASE 0x12000000ul//((1ul << 28ul) | (1ul << 25ul)) //(FB_0_BASE + (320 * 240))
#define PAL_BASE 0x14000000ul
#define FB_PAGE_BASE 0x14000400ul
//#define TCM_BASE 0x18000000ul


#define ADDR_DBG_PRINT (0x16000000ul)
#define ADDR_DBG_EXIT (0x16000004ul)
#define ADDR_TIMER_USEC_LO (0x16000000ul)
//#define ADDR_TIMER_USEC_HI (0x16000004ul)
#define ADDR_TIMER_SEC_LO (0x16000008ul)
//#define ADDR_TIMER_SEC_HI (0x1600000cul)
#define ADDR_POLL_INFO (0x1600000cul)


extern pixel_t* DG_ScreenBuffer;

#ifdef __cplusplus
extern "C" {
#endif

void doomgeneric_Create(int argc, char **argv);
void doomgeneric_Tick();


//Implement below functions for your platform
void DG_Init();
void DG_DrawFrame();
void DG_SleepMs(uint32_t ms);
uint32_t DG_GetTicksMs();
int DG_GetKey(int* pressed, unsigned char* key);
void DG_SetWindowTitle(const char * title);

#ifdef __cplusplus
}
#endif

#endif //DOOM_GENERIC
