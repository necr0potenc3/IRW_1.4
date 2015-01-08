/******************************************************************************\
* 
* 
*  Copyright (C) 2004 Daniel 'Necr0Potenc3' Cavalcanti
* 
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
* 
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
* 
* 
\******************************************************************************/

#include <windows.h>
#include <stdio.h>
#include "Main.h"
#include "irw.h"


HINSTANCE DLLInst = NULL;
BOOL Inited = FALSE;

BOOL APIENTRY DllMain(HINSTANCE hDLLInst, DWORD fdwReason, LPVOID lpvReserved)
{
    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hDLLInst);
			DLLInst = hDLLInst;
		}break;
        case DLL_PROCESS_DETACH:
		{
			/* to make sure the dll wasn't simply loaded and InitPlugin() wasnt called */
			if(Inited == TRUE)
				UnloadPlugin();
		}break;
        case DLL_THREAD_ATTACH:		break;
        case DLL_THREAD_DETACH:		break;
    }
    return TRUE;
}


int InitPlugin(void)
{
	if(Inited == TRUE)
		return FALSE;

	Inited = TRUE;
	AddCommand("usetype", Command_UseType);

	return TRUE;
}

void UnloadPlugin(void)
{
	if(Inited == FALSE)
		return;	

	return;
}

void GetPluginInfo(char *Text, int Size)
{
	strncpy(Text, "Use Object by Graphic and color", Size);
	return;
}

void Command_UseType(char **Arg, int ArgCount)
{
	unsigned int Serial = INVALID_SERIAL;
	unsigned short Graphic = INVALID_GRAPHIC;
	unsigned short Color = INVALID_COLOR;

	if(ArgCount < 2)
	{
		ClientPrint("Usage: usetype graphic [color]");
		return;
	}

	Graphic = ArgToInt(Arg[1]);
	if(ArgCount == 3)
		Color = ArgToInt(Arg[2]);

	Serial = FindItemInContainer(Graphic, Color, GetPlayerSerial());

	if(Serial == INVALID_SERIAL)
	{
		ClientPrintWarning("Could not find graphic 0x%04X in backpack", Graphic);
		return;
	}

	UseObject( Serial );

	return;
}
