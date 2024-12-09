#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"
#include "functions.h"
#include <ctype.h>
#include <fcntl.h>

void variableNamePlaceHolder(char *arg, char *result)
{
    char buffer[MAX_INPUT];
    char *start = arg, *end;
    buffer[0] = '\0';

    while ((start = strstr(start, "$")) != NULL)
    {
        strncat(buffer, arg, start - arg);
        start++;

        end = start;
        while (*end && (isalnum(*end) || *end == '_'))
        {
            end++;
        }

        char variableName[MAX_INPUT];
        strncpy(variableName, start, end - start);
        variableName[end - start] = '\0';

        char *value = getenv(variableName);
        if (value)
        {
            strcat(buffer, value);
        }
        else
        {
            strcat(buffer, "");
        }

        arg = end;
        start = end;
    }

    strcat(buffer, arg);
    strcpy(result, buffer);
}

void replacePlaceHolder(char **args)
{
    char buffer[MAX_INPUT];
    for (int i = 0; args[i] != NULL; i++)
    {
        variableNamePlaceHolder(args[i], buffer);
        strcpy(args[i], buffer);
    }
}

void nonBuiltIn(char **args)
{
    if (fork() == 0)
    {
        execvp(args[0], args);
        perror("exec");
        exit(EXIT_FAILURE);
    }
    wait(NULL);
}

void customRedirect(char **args, char *outputFile)
{
    if (fork() == 0)
    {
        if (outputFile)
        {
            int fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("error opening the output file!");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(args[0], args);
        perror("exec");
        exit(EXIT_FAILURE);
    }
    wait(NULL);
}

void parsing(char *cmd)
{
    char *args[MAX_ARGS];
    int argCounter = 0;
    char *outputFile = NULL;

    char *outputRedirect = strstr(cmd, ">");
    if (outputRedirect)
    {
        *outputRedirect = '\0';
        outputFile = strtok(outputRedirect + 1, " ");
        if (outputFile == NULL)
        {
            fprintf(stderr, "no output file for >\n");
            return;
        }
    }

    while ((args[argCounter++] = strsep(&cmd, " ")) != NULL)
        ;
    args[argCounter - 1] = NULL;

    replacePlaceHolder(args);

    if (strcmp(args[0], "cd") == 0)
    {
        commandCD(args[1]);
    }
    else if (strcmp(args[0], "pwd") == 0)
    {
        commandPWD();
    }
    else if (strcmp(args[0], "set") == 0)
    {
        commandSET(args[1], args[2]);
    }
    else if (strcmp(args[0], "unset") == 0)
    {
        commandUNSET(args[1]);
    }
    else if (args[0] != NULL)
    {
        customRedirect(args, outputFile);
    }
}
