#include "stdafx.h"
#include "irw.h"
#include "DropCatchBag.h"
#include "EmptyContainer.h"
#include "Main.h"

HINSTANCE DLLInst = NULL;
BOOL Inited = FALSE;

BOOL APIENTRY DllMain( HINSTANCE hDLLInst, 
                       DWORD  fdwReason, 
                       LPVOID lpReserved
					 )
{
	switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hDLLInst);
			DLLInst = hDLLInst;
			break;
        case DLL_PROCESS_DETACH:
			/* to make sure the dll wasn't simply loaded and InitPlugin() wasnt called */
			if(Inited == TRUE)
				UnloadPlugin();
			break;
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
	AddCommand("drop", Command_Drop);
	AddCommand("setcatchbag", Command_Catch);
	AddCommand("unsetcatchbag", Command_UnCatch);
	AddCommand("setreceivingcontainer",Command_Receiving);
	AddCommand("emptycontainer",Command_EmptyContainer);
	AddCommand("snoop",Command_Snoop);
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
	strncpy(Text, "Drop,Bag,Container Plugin by Xan", Size);
	return;
}
