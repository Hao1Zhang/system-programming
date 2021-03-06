/* execute.c - code used by small shell to execute commands */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include "smsh.h"
#include <fcntl.h>
#include <glob.h>

char **check_command_type(char **command)
{

    int count = 0; //this count the lenth of command
    int i;
    char type = 'n';
    while (command[count] != NULL)
    {
        count++;
    }

    for (i = 0; i < count; i++)
    {
        if (command[i][0] == '>')
        {
            close(1);
            open(command[i + 1], O_CREAT | O_WRONLY, (S_IRUSR | S_IWUSR | S_IXUSR)); //create or write only next command(file name)
            command[i] = NULL;
            return command;
        }
        else if (command[i][0] == '<')
        {
            close(0);
            open(command[i + 1], O_RDONLY); //create or write only next command(file name)
            command[i] = NULL;
            return command;
        }
    }
    return command;
}

int check_global(char **command)
{

    int count = 0; //this count the lenth of command
    int i;
    char type = 'n';
    while (command[count] != NULL)
    {
        count++;
    }
    for (i = 0; i < count; i++)
    {
        int s = 0;
        while (command[i][s] != '\0')
        { //check every char if have *
            if (command[i][s] == '*')
            {
                return 1;
            }
            s++;
        }
    }
    return 0;
}

int execute(char *argv[])
/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors

 */
{
    int pid;
    int child_info = -1;

    if (argv[0] == NULL) /* nothing succeeds	*/
        return 0;

    if ((pid = fork()) == -1)
        perror("fork");
    else if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execvp(argv[0], argv);
        perror("cannot execute command");
        exit(1);
    }
    else
    {
        if (wait(&child_info) == -1)
            perror("wait");
    }
    return child_info;
}

int executePiped(char *argv[])
/*  PIPE
 * purpose: run a multiple program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors
 */
{
    int child_info = -1;
    int count = 0; //this count the lenth of argv

    if (argv[0] == NULL) /* nothing succeeds	*/
        return 0;

    while (argv[count] != NULL)
    {
        count++;
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

            //parse token
            char *token;
            char **command;
            command = malloc(256);
            token = strtok(argv[index], " ");
            int number_token = 0;
            while (token != NULL)
            {
                command[number_token] = token;
                // get the next token
                token = strtok(NULL, " ");
                number_token++;
            }

            command = check_command_type(command);

            int global = 0;
            global = check_global(command);

            int count1 = 0; //this count the lenth of command
            int i;
            while (command[count1] != NULL)
            {
                count1++;
            }

            if (global == 1)
            {
                for (i = 0; i < count1; i++)
                {
                    int j = 0;
                    while (command[i][j] != '\0')
                    {
                        if (command[i][j] == '*')
                        {
                            glob_t g;
                            g.gl_offs = 1;
                            glob(command[i], GLOB_DOOFFS, NULL, &g);
                            g.gl_pathv[0] = command[0];
                            execvp(g.gl_pathv[0], &g.gl_pathv[0]);
                        }
                        j++;
                    }
                }
            }
            else
            {
                //execute
                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);
                execvp(command[0], command);
                perror("cannot execute command");
                exit(1);
            }

            freelist(command);
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
            if (wait(&child_info) == -1)
                perror("wait");
        }
    }
    return child_info;
}