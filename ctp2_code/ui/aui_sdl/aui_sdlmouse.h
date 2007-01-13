#ifndef __aui_sdl__aui_sdlmouse_h__
#define __aui_sdl__aui_sdlmouse_h__ 1

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#include "ctp2_config.h"

#if defined(__AUI_USE_SDL__)

#include "aui_mouse.h"
#include "aui_sdlinput.h"

class aui_SDLMouse : public aui_Mouse, public aui_SDLInput {
public:
	aui_SDLMouse(AUI_ERRCODE *retval, MBCHAR *ldlBlock,
	             BOOL useExclusiveMode);
	virtual ~aui_SDLMouse();

protected:
	aui_SDLMouse();

public:
	virtual AUI_ERRCODE Acquire();
	virtual AUI_ERRCODE Unacquire();
	virtual AUI_ERRCODE ReactToInput();
	virtual AUI_ERRCODE GetInput();
	
};

#endif // defined(__AUI_USE_SDL__)

#endif
