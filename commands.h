#ifndef COMMANDS_H
#define COMMANDS_H

void commandCD(char *path);
void commandPWD();
void commandSET(char *name, char *value);
void commandUNSET(char *name);
void nonBuiltIn(char **args);

#endif
