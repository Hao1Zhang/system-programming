/*
 * @Author       : Hao Zhang
 * @Date         : 2021-05-23 21:09:54
 * @LastEditors  : Hao Zhang
 * @LastEditTime : 2021-05-23 21:10:02
 * @FilePath     : \assignment3\smsh3.c
 */
/*
 * @Author       : Hao Zhang
 * @Date         : 2021-05-23 10:30:41
 * @LastEditors  : Hao Zhang
 * @LastEditTime : 2021-05-23 19:20:19
 * @FilePath     : \assignment3\smsh2.c
 */
/**  smsh1.c  small-shell version 1
 **		first really useful version after prompting shell
 **		this one parses the command line into strings
 **		uses fork, exec, wait, and ignores signals
 **/

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include	"smsh.h"

#define	DFL_PROMPT	"> "

int main()
{
	char	*cmdline, *prompt, **arglist;
	int	result;
	void	setup();

	prompt = DFL_PROMPT ;
	setup();

	while ( (cmdline = next_cmd(prompt, stdin)) != NULL ){
		if ( (arglist = splitline(cmdline)) != NULL  ){
			result = executePiped(arglist);
			freelist(arglist);
		}
		free(cmdline);
	}
	return 0;
}

void setup()
/*
 * purpose: initialize shell
 * returns: nothing. calls fatal() if trouble
 */
{
	signal(SIGINT,  SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
	fprintf(stderr,"Error: %s,%s\n", s1, s2);
	exit(n);
}
