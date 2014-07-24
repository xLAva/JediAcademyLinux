#include "sdl_glw.h"

#include <SDL2/SDL.h>


void Sys_QueEvent( int time, sysEventType_t type, int value, int value2, int ptrLength, void *ptr );
void IN_DeactivateMouse( void );


void WG_CheckHardwareGamma();
void WG_RestoreGamma();

extern SDL_Window*   s_pSdlWindow;
extern SDL_Renderer* s_pSdlRenderer;



