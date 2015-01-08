
#ifndef _MAIN_H_INCLUDED
#define _MAIN_H_INCLUDED

int InitPlugin( void );
void UnloadPlugin( void );
void GetPluginInfo( char *Text, int Size );

void Command_Grab( char **Arg, int ArgCount );
void Grab_Object( unsigned int Serial, unsigned short X, unsigned short Y, int Z );

#endif 
