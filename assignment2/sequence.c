
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
//this part is to parse the space

char *strsep(char **stringp, const char *delim) {
    char *rv = *stringp;
    if (rv) {
        *stringp += strcspn(*stringp, delim);
        if (**stringp)
            *(*stringp)++ = '\0';
        else
            *stringp = 0; }
    return rv;
}

void parse(char *str, char **cmd)
{
    int i;
    for (i = 0; i < 256; i++)
    {
        cmd[i] = strsep(&str, " ");
        if (cmd[i] == NULL)
            break;
    }
}
/*Function definition of main()*/
int main()
{
    char *cmd[1000];
    char line[100][256];
    pid_t pid;
    int count = 0;
    int i;

    while (1)
    {
        if (fgets(line[count], 256, stdin) == NULL)
        {
            break;
        }

        count++;
    }
    /* Reading file line by line */
    for (i = 0; i < count; i++)
    {
        strtok(line[i], "\n");
        pid = fork();
        // Fork Fail
        if (pid < 0)
        {
            printf("Failed Fork\n");
        }
        else if (pid == 0)
        {
            parse(line[i], cmd);
            execvp(*cmd, cmd);
        }
        //parent
        else
        {
            wait(NULL);
        }
    }
    return 0;
}
