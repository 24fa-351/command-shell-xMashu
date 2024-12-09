#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"
#include <stdlib.h>

void commandCD(char *path)
{
    if (path == NULL)
    {
        const char *home = getenv("HOME");
        if (home == NULL)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
        if (chdir(home) != 0)
        {
            perror("cd");
        }
    }
    else
    {
        if (chdir(path) != 0)
        {
            perror("cd");
        }
    }
}

void commandPWD()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)))
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("pwd");
    }
}

void commandSET(char *name, char *value)
{
    if (name == NULL || value == NULL)
    {
        fprintf(stderr, "ex. set name ___");
        return;
    }
    if (setenv(name, value, 1) != 0)
    {
        perror("setenv");
    }
}

void commandUNSET(char *name)
{
    if (name == NULL)
    {
        fprintf(stderr, "wrong name?\n");
        return;
    }
    if (unsetenv(name) != 0)
    {
        perror("unset");
    }
}
