void Command_Receiving(char **Arg, int ArgCount);
void Command_EmptyContainer(char **Arg, int ArgCount);
void Command_Snoop(char **Arg, int ArgCount);
void Catch_Receiving(unsigned int Serial, unsigned short X, unsigned short Y, int Z);
void Catch_Empty(unsigned int Serial, unsigned short X, unsigned short Y, int Z);
void Catch_Snoop(unsigned int Serial, unsigned short X, unsigned short Y, int Z);
int GumpFromGraphic(int graphic);
