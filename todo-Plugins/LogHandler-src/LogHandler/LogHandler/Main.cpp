#include <windows.h>
#include "commctrl.h"
#include "Main.h"
#include "irw.h"
#include "resource.h"

static HWND LogWnd = NULL;
static HINSTANCE DLLInst = NULL;
static BOOL Inited = FALSE;

int Flags;
int Val[13]={	NOFILTER_LOG,NETWORK_LOG,BUILDER_LOG,COMPRESSOR_LOG,API_LOG,
				WARNING_LOG,ERROR_LOG,CRYPT_LOG,INTEREST_LOG,CLIENTIRW_LOG,
				IRWSERVER_LOG,SERVERIRW_LOG,IRWCLIENT_LOG
			};

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
	int TempFlag;
	if(Inited == TRUE)
		return FALSE;

	Inited = TRUE;

	LogWnd = CreateDialog(DLLInst, MAKEINTRESOURCE(DIALOG_LOG), GetMainTabWindow(), (DLGPROC)LogDlgProc);

	if(LogWnd == NULL)
		MBOut("LOGHANDLER PLUGIN", "Could not create the dialog"); 
	else
		AddTab(LogWnd, "Logs", FALSE);
	if(GetIRWProfileInt(NULL, "LogHandler", "Flags", &TempFlag)==TRUE)
	{
		int i;
		for(i=0;i<13;i++)
			if((TempFlag&(1<<i))!=0)
			{
				SendMessage(LogWnd,WM_COMMAND,MAKEWPARAM(i+1001,BN_CLICKED),NULL);
				SendMessage(GetDlgItem(LogWnd,i+1001),BM_SETCHECK,BST_CHECKED,0);
			}
			else
				LogBlock(Val[i]);
	}
	return TRUE;
}

void UnloadPlugin(void)
{
	if(Inited == FALSE)
		return;	
	SetIRWProfileInt(NULL, "LogHandler", "Flags", Flags);
		
	return;
}

void GetPluginInfo(char *Text, int Size)
{
	strncpy(Text, "LogHandler plugin", Size);
	return;
}

LRESULT CALLBACK LogDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_COMMAND: 
			int Param=LOWORD(wParam)-1001;
			if((Param>=0)&&(Param<13))
			{
				Flags^=(1<<Param);
				if(!(Flags&(1<<Param)))
					LogBlock(Val[Param]);
				else
					LogUnblock(Val[Param]);
			}
			break;	
	}
	return FALSE;
}