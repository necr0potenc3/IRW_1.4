void Command_Drop(char **Arg, int ArgCount);
void Command_Catch(char **Arg, int ArgCount);
void Command_UnCatch(char **Arg, int ArgCount);
void Drop_Target(unsigned int Serial, unsigned short X, unsigned short Y, int Z);
void Catch_Target(unsigned int Serial, unsigned short X, unsigned short Y, int Z);
bool IsOwner(GameObject Obj);