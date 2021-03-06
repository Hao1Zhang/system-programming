

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define STRING_LENGTH 4096

int main(int argc, char const *argv[])
{
    char line[100][STRING_LENGTH]; 
    int count = 0;               

    while (1)
    {
        if (fgets(line[count], STRING_LENGTH, stdin) == NULL)
        {
            break;
        }
        else
        {
            if (line[count][0] == '\n')
            {
                count--;
            }

            count++;
        }
    }

    int i = 0;
    for (i = 0; i < count; i++)
    {
        if (line[i][strlen(line[i]) - 1] == '\n')
        {
            line[i][strlen(line[i]) - 1] = '\0';
        }
    }

    int new_pipe_fds[2];
    int old_pipe_fds[2];

    int pid;

    int index;
    for (index = 0; index < count; index++)
    {
        if (index + 1 < count)
        {
            pipe(new_pipe_fds);
        }
        pid = fork();
        if (pid == 0)
        {
            if (index - 1 >= 0)
            {
                dup2(old_pipe_fds[0], STDIN_FILENO);
                close(old_pipe_fds[0]);
                close(old_pipe_fds[1]);
            }
            if (index + 1 < count)
            {

                close(new_pipe_fds[0]);
                dup2(new_pipe_fds[1], STDOUT_FILENO);
                close(new_pipe_fds[1]);
            }
            char *token;
            char *command[100];
            token = strtok(line[index], " ");
            int number_token = 0;
            do
            {
                command[number_token] = token;
                token = strtok(NULL, " ");
                number_token++;
            } while (token != NULL);
            command[number_token] = NULL;
            execvp(command[0], command);
        }

        else
        {
            if (index - 1 >= 0)
            {
                close(old_pipe_fds[0]);
                close(old_pipe_fds[1]);
            }
            if (index + 1 < count)
            {
                old_pipe_fds[0] = new_pipe_fds[0];
                old_pipe_fds[1] = new_pipe_fds[1];
            }
            wait(NULL);
        }
    }

    return 0;
}
