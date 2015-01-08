#include "stdafx.h"
#include "irw.h"
#include "EmptyContainer.h"
#include <cstdlib>
#include <string>

using namespace std;

unsigned int interval,receive;

void Command_Receiving(char **Arg, int ArgCount)
{
	if(ArgCount<2)
	{
		ClientPrint("Usage: setreceivingcontainer [object]");
		ClientPrint("Target container to set.");
		RequestTarget(TARGET_OBJECT,&Catch_Receiving);
	}
	else
		Catch_Receiving(ArgToInt(Arg[1]), 0, 0, 0);
	return;	
}
void Catch_Receiving(unsigned int Serial, unsigned short X, unsigned short Y, int Z)
{
	GameObject Obj;
	string msg="Receiving Container Set 0x";
	if(Serial == INVALID_SERIAL)
		return;
	else
		if(GetObjectInfo(Serial, INVALID_IDX, &Obj) == OBJECT_NOTFOUND)
		{
			ClientPrint("Invalid Container");
			return;
		}
	CleanIRWProfileSection(NULL,"EmptyContainer");
	SetIRWProfileInt(NULL,"EmptyContainer","ReceivingContainer",Serial);
	char buffer[20];
	itoa(Serial,buffer,16);
	msg+=strupr(buffer);
	ClientPrint(msg.c_str());
	FREECHAR(Obj);
	return;
}
void Command_EmptyContainer(char **Arg, int ArgCount)
{
	GameObject temp;
	if(ArgCount<2)
	{
		ClientPrint("Usage: emptycontainer delay [container] [receivingcontainer]");
		return;
	}
	interval=ArgToInt(Arg[1]);
	if(ArgCount>3)
		receive=ArgToInt(Arg[3]);
	else
	{
		int t;
		GetIRWProfileInt(NULL, "EmptyContainer", "ReceivingContainer", &t);
		receive=t;
		if(receive==INVALID_SERIAL)
		{
			ClientPrint("No Receive Container has been setted.");
			return;
		}
	}
	if(GetObjectInfo(receive,INVALID_IDX,&temp)==OBJECT_NOTFOUND)
	{
		ClientPrint("Invalid Receive Container Serial.");
		return;
	}
	FREECHAR(temp);
	if(ArgCount>2)
		Catch_Empty(ArgToInt(Arg[2]),0,0,0);
	else
		RequestTarget(TARGET_OBJECT,&Catch_Empty);
	
}
void Catch_Empty(unsigned int Serial, unsigned short X, unsigned short Y, int Z)
{
	GameObject Obj;
	int* list;
	int i;
	if(Serial == INVALID_SERIAL)
		return;
	else
		if(GetObjectInfo(Serial, INVALID_IDX, &Obj) == OBJECT_NOTFOUND)
		{
			ClientPrint("Invalid Container");
			return;
		}
	FREECHAR(Obj);
	list=ListItemsInContainer(Serial);
	if(list==NULL)
		return;
	for(i=1;i<=list[0];i++)
	{
		GetObjectInfo(INVALID_SERIAL,list[i],&Obj);
		if(Obj.Container==Serial)
		{
			PickupItem(Obj.Serial,Obj.Quantity);
			DropItemInContainer(Obj.Serial,receive);
		}
		FREECHAR(Obj);
		Sleep(interval);
	}
	return;
}
void Command_Snoop(char **Arg, int ArgCount)
{
	if(ArgCount>1)
		Catch_Snoop(ArgToInt(Arg[1]),0,0,0);
	else
	{
		ClientPrint("usage: ,snoop [serial]");
		RequestTarget(TARGET_OBJECT,&Catch_Snoop);
	}
}
void Catch_Snoop(unsigned int Serial, unsigned short X, unsigned short Y, int Z)
{
	GameObject Obj;
	unsigned char Buf[7];
	if(Serial == INVALID_SERIAL)
		return;
	else
		if(GetObjectInfo(Serial, INVALID_IDX, &Obj) == OBJECT_NOTFOUND)
		{
			ClientPrint("Invalid Container");
			return;
		}
	if(GumpFromGraphic(Obj.Graphic)==-1)
	{
		FREECHAR(Obj);
		return;
	}
	Buf[0]=0x24;
	PackUInt32(Buf+1,Serial);
	PackUInt16(Buf+5,GumpFromGraphic(Obj.Graphic));
	SendToClient(Buf,sizeof(Buf));
	FREECHAR(Obj);
	return;
}

int GumpFromGraphic(int graphic)
{
    int gump=-1;

    switch(graphic)
    {
    case 0x0e75:      // backpack
    case 0x0e79:      // box/pouch
        gump = 0x3C;
        break;
    case 0x0e76:      // leather bag
        gump = 0x3D;
        break;
    case 0x0e77:      // barrel
    case 0x0e7F:      // keg
        gump = 0x3E;
        break;
    case 0x0e7A:      // square basket
        gump = 0x3F;
        break;
    case 0x0e40:      // metal & gold chest
    case 0x0e41:      // metal & gold chest
        gump = 0x42;
        break;
    case 0x0e7D:      // wooden box
        gump = 0x43;
        break;
    case 0x0e3C:      // large wooden crate
    case 0x0e3D:      // large wooden crate
    case 0x0e3E:      // small wooden create
    case 0x0e3F:      // small wooden crate
    case 0x0e7E:      // wooden crate
        gump = 0x44;
        break;
    case 0x0e42:      // wooden & gold chest
    case 0x0e43:      // wooden & gold chest
        gump = 0x49;
        break;
    case 0x0e7C:      // silver chest
        gump = 0x4A;
        break;
    case 0x0e80:      // brass box
        gump = 0x4B;
        break;
    case 0x0e83:
        gump = 0x3E;
    case 0x09aa:      // wooden box
        gump=0x43;
        break;
    case 0x09A8:      // metal box
        gump=0x4B;  // fix from Daemar
        break;
    case 0x0990:      // round basket
        gump=0x41;
        break;
    case 0x09A9:      // small wooden crate
        gump=0x44;
        break;
    case 0x09AB:      // metal & silver chest
        gump=0x4A;
        break;
    case 0x09AC:
    case 0x09B1:
        gump = 0x41;
        break;
    case 0x09B0:
        gump = 0x3C;
        break;
    case 0x09B2:      // bank box (..OR.. backpack 2)
        gump = 0x4A;
        break;
    case 0x0a30:   // chest of drawers (fancy)
    case 0x0a38:   // chest of drawers (fancy)
        gump=0x48;
        break;

    case 0x0a4C:   // fancy armoire (open)
    case 0x0a4D:   // fancy armoire
    case 0x0a50:   // fancy armoire (open)
    case 0x0a51:   // fancy armoire
        gump=0x4E;
        break;

    case 0x0a4E:   // wooden armoire (open)
    case 0x0a4F:   // wooden armoire
    case 0x0a52:   // wooden armoire (open)
    case 0x0a53:   // wooden armoire
        gump=0x4F;
        break;

    case 0x0a97:   // bookcase
    case 0x0a98:   // bookcase
    case 0x0a99:   // bookcase
    case 0x0a9A:   // bookcase
    case 0x0a9B:   // bookcase
    case 0x0a9C:   // bookcase
    case 0x0a9D:    // bookcase (empty)
    case 0x0a9E:    // bookcase (empty)
        gump=0x4D;
        break;

    case 0x0a2C:   // chest of drawers (wood)
    case 0x0a34:   // chest of drawers (wood)
    case 0x0a35:   // dresser
    case 0x0a3C:   // dresser
    case 0x0a3D:   // dresser
    case 0x0a44:   // dresser
        gump=0x51;
        break;
    case 0x2006:      // coffin
        gump=0x09;
        break;
    case 0x0Fae:    // barrel with lids
        gump = 0x3E;
        break;

    case 0x1Ad7:    // potion kegs
        gump = 0x3E;
        break;

    case 0x1940:    // barrel with lids
        gump = 0x3E;
        break;
    default:
        gump = 0x3C;    // backpack
        break;
    }
    return gump;
}