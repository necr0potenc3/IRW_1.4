/*
	usage: grab [quantity] [serial/lasttarget]
*/

#include <windows.h>
#include <stdio.h>
#include "main.h"
#include "irw.h"

HINSTANCE DLLInst = NULL;
BOOL Inited = FALSE;
int GrabQuantity = 0;

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
	AddCommand("grab", Command_Grab);

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
	strncpy(Text, "Grab object to backpack", Size);
	return;
}

void Command_Grab( char **Arg, int ArgCount )
{
	if(ArgCount < 2)
	{
		ClientPrint( "Target object to grab all" );
		GrabQuantity = 0;
		RequestTarget( TARGET_OBJECT, &Grab_Object );
		return;
	}
	
	GrabQuantity = atoi( Arg[1] );

	if(ArgCount < 3)
	{
		if( !GrabQuantity )
			ClientPrint( "Target object to grab all" );
		else
			ClientPrint( "Grab %i of what?", GrabQuantity );
			
		RequestTarget( TARGET_OBJECT, &Grab_Object );
		return;
	}

	Grab_Object( ArgToInt( Arg[2] ), 0 ,0 ,0 );
	return;
}


void Grab_Object( unsigned int Serial, unsigned short X, unsigned short Y, int Z )
{
	GameObject Obj;

	if( Serial == INVALID_SERIAL )
	{
		ClientPrint( "Invalid target" );
		return;
	}

	if( GetObjectInfo( Serial, INVALID_IDX, &Obj ) == OBJECT_NOTFOUND )
	{
		ClientPrint( "Object not in the world list" );
		return;
	}

	int quantity = Obj.Quantity;
	
	if( !quantity )
		quantity = 1;

	if( GrabQuantity && GrabQuantity < quantity )
		quantity = GrabQuantity;

	MoveToContainer( Obj.Serial, quantity, GetPlayerBackpack() );

	FREECHAR(Obj);
	return;
}
