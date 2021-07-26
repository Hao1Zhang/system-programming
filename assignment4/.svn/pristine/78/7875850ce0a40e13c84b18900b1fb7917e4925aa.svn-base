/*
 * @Author       : Hao Zhang
 * @Date         : 2021-06-04 17:58:37
 * @LastEditors  : Hao Zhang
 * @LastEditTime : 2021-06-04 19:31:01
 * @FilePath     : \system programming\assignment4\slow_functions.c
 */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

// ## PART 1
static int anti_cheat = 0;

static pthread_mutex_t lock; 

void * slow_function1(void * empty)
{
	
	static int special = 0;
	sleep(1);

	pthread_mutex_lock(&lock); 
	if ( anti_cheat == 1 )
	{
		special = special + 1;
	}
	if (special > 9)
	{
		printf("Work Done\n");
	}
	pthread_mutex_unlock(&lock); 	
	sleep(1);
	if (special > 10)
	{
		printf("FAIL!!\n");
	}
	return NULL;
}

void * slow_function2(void * empty)
{
	if (pthread_mutex_init(&lock, NULL) != 0) 
	{ 
		printf("FAIL!\n"); 
	} 
	printf("Start 2\n");
	anti_cheat = 1;
	sleep(2);
	printf("End 2\n");
}

// ## PART 2
static char buffer[100];
static int write_calls = 0;
static int read_calls = 0;
static int written = 0;

void * bad_write(void * input)
{
	sleep(0.01);
	strcpy(buffer, (char *)input);
	sleep(0.01);
	written = 1;
	sleep(0.01);
	write_calls = write_calls + 1;
	sleep(0.01);
}

void * bad_read(void * empty)
{
	sleep(0.01);
	printf("%s", buffer);
	sleep(0.01);
	written = 0;
	sleep(0.01);
	strcpy(buffer, "Fail!!\n");
	sleep(0.01);
	read_calls = read_calls + 1;
	sleep(0.01);
}

int get_written()
{
	return written;
}

void get_output()
{
	printf("Read:    %d\n", read_calls);
	printf("Written: %d\n", write_calls);
}
