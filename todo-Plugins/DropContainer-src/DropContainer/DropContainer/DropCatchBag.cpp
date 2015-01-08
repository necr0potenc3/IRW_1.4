#include "stdafx.h"
#include "irw.h"
#include "DropCatchBag.h"
#include <cstdlib>
#include <cmath>

using namespace std;

int quantity;
int CoordX,CoordY,CoordZ;

void Command_UnCatch(char **Arg, int ArgCount)
{
	SetCatchBag(GetPlayerBackpack());
	ClientPrint("CatchBag unset.");
	return;
}
void Command_Catch(char **Arg, int ArgCount)
{
	if(ArgCount<2)
	{
		ClientPrint("Usage: setcatchbag [object]");
		ClientPrint("Target container to catch item that fall into backpack.");
		RequestTarget(TARGET_OBJECT,&Catch_Target);
	}
	else
		Catch_Target(ArgToInt(Arg[1]), 0, 0, 0);
	return;
}
void Catch_Target(unsigned int Serial, unsigned short X, unsigned short Y, int Z)
{
	GameObject Obj;
	if(Serial == INVALID_SERIAL)
		return;
	else
		if(GetObjectInfo(Serial, INVALID_IDX, &Obj) == OBJECT_NOTFOUND)
		{
			ClientPrint("Invalid Container");
			return;
		}
	SetCatchBag(Obj.Serial);
	FREECHAR(Obj);
	return;
}
void Command_Drop(char **Arg, int ArgCount)
{
	if(ArgCount>1)
	{
		quantity=ArgToInt(Arg[1]);
		if(quantity<1)
			quantity=1;
	}
	else
	{
		quantity=0;
		ClientPrint("Usage: drop [quantity] [X Y Z] [object]");
		ClientPrint("Example: drop 6 0 0 0 mymoney");
	}
	GameObject pg;
	GetObjectInfo(GetPlayerSerial(),INVALID_IDX,&pg);
	if(ArgCount>3)
	{
		if((ArgToInt(Arg[2])>=0)&&(fabs(static_cast<double>(pg.X-ArgToInt(Arg[2])))<6.0))
			CoordX=ArgToInt(Arg[2]);
		else
		{
			FREECHAR(pg);
			ClientPrint("Invalid X Coordinate");
			return;
		}
		if((ArgToInt(Arg[3])>=0)&&(fabs(static_cast<double>(pg.Y-ArgToInt(Arg[3])))<6.0))
			CoordY=ArgToInt(Arg[3]);
		else
		{
			FREECHAR(pg);
			ClientPrint("Invalid Y Coordinate");
			return;
		}
		if(ArgCount==5)
			CoordZ=ArgToInt(Arg[4]);
		else
			CoordZ=pg.Z;
	}
	else
	{
		CoordX=pg.X;
		CoordY=pg.Y;
		CoordZ=pg.Z;
	}
	FREECHAR(pg);
	if(ArgCount>5)
		Drop_Target(ArgToInt(Arg[5]), 0, 0, 0);
	else
		RequestTarget(TARGET_OBJECT,&Drop_Target);
}
void Drop_Target(unsigned int Serial, unsigned short X, unsigned short Y, int Z)
{
	GameObject Obj;
	if(Serial == INVALID_SERIAL)
		return;
	else
		if(GetObjectInfo(Serial, INVALID_IDX, &Obj) == OBJECT_NOTFOUND)
		{
			ClientPrint("Invalid Object");
			return;
		}
	if(!IsOwner(Obj))
	{
		FREECHAR(Obj);
		ClientPrint("Invalid Object");
		return;
	}
	if(quantity>0)
		PickupItem(Obj.Serial, quantity );
	else
		PickupItem(Obj.Serial, Obj.Quantity  );
	DropItem(Obj.Serial,CoordX,CoordY,CoordZ);
	FREECHAR(Obj);
	return;
}
bool IsOwner(GameObject Obj)
{
	unsigned int contser=Obj.Container;
	GameObject temp;
	while(contser!=INVALID_SERIAL)
	{	
		if(GetObjectInfo(contser,INVALID_IDX,&temp)==OBJECT_NOTFOUND)
		{
			FREECHAR(Obj);
			FREECHAR(temp);
			return false;
		}
		contser=temp.Container;
	}
	if(temp.Serial!=GetPlayerSerial())
	{
		FREECHAR(temp);
		FREECHAR(Obj);
		return false;
	}
	return true;
}