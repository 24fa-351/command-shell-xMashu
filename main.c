#include <stdio.h>
#include <string.h>
#include "functions.h"

#define MAX_INPUT 1024

int main()
{
    char input[MAX_INPUT];

    while (1)
    {
        printf("xsh# ");
        fflush(stdout);
        if (!fgets(input, sizeof(input), stdin))
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0)
            break;

        parsing(input);
    }

    return 0;
}
