#include <stdio.h>

#include "m_argv.h"

#include "doomgeneric.h"

//#ifdef MELTED_MOON_RISCV32
//pixel_t* DG_ScreenBuffer = (pixel_t*)(FB_0_BASE);
//#else
pixel_t* DG_ScreenBuffer = NULL;
//#endif

void M_FindResponseFile(void);
void D_DoomMain (void);


void doomgeneric_Create(int argc, char **argv)
{
	// save arguments
    myargc = argc;
    myargv = argv;

	M_FindResponseFile();

    #ifndef MELTED_MOON_RISCV32
	DG_ScreenBuffer = malloc(DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);
	#else
	#endif

	DG_Init();

	D_DoomMain ();
}

