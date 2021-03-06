/*
 * @Author       : Hao Zhang
 * @Date         : 2021-06-04 17:58:37
 * @LastEditors  : Hao Zhang
 * @LastEditTime : 2021-06-05 23:55:39
 * @FilePath     : \system programming\assignment4\part2.c
 */
#include "slow_functions.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

// How nice of me to include a global that tells you how many commands there were :)
static int total_commands = 0;
//initialize a lock and a condition
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t has_written = PTHREAD_COND_INITIALIZER;
pthread_cond_t has_read = PTHREAD_COND_INITIALIZER;
static int count = 0; //how many string are wrote by writer and have not been readed yet
// ####################################################################################
// ## Please write some code in the following two functions

void *writer(void *in_ptr)
{
	int i;
	//must include bad_write;
	char **commands = (char **)in_ptr; //take the parameter as a char **

	//for every string,use bad write
	for (i = 0; i < total_commands; i++)
	{
		//lock and unlock to make sure that reader and writer will not execute at the same time
		pthread_mutex_lock(&lock);

		//when there is a write has not been readed
		while (count >= 1)
		{
			pthread_cond_wait(&has_read, &lock); //wait for the signal that bad_read has been called
		}

		bad_write((void *)commands[i]);
		count++; //after write, count++ so that it will larger or equal than 1

		pthread_mutex_unlock(&lock);
		//send signal to reader
		pthread_cond_signal(&has_written);
	}
}

void *reader(void *empty)
{
	int i;
	//must include bad_read

	//for every string,use bad read
	for (i = 0; i < total_commands; i++)
	{
		//lock and unlock to make sure that reader and writer will not execute at the same time
		pthread_mutex_lock(&lock);

		//when there no write has not been readed
		while (count < 1)
		{
			pthread_cond_wait(&has_written, &lock); //wait for the signal that bad_write has been called
		}

		bad_read(NULL);
		count--; //after read, count-- so that it will less than 1
		pthread_mutex_unlock(&lock);
		//send signal to writter
		pthread_cond_signal(&has_read);
	}
}

// ################################################################################
// ## DO NOT MODIFY

int main()
{

	// ## Parse STDIN and read into commands
	char *commands[100];
	char line[256];
	while (fgets(line, 256, stdin))
	{
		commands[total_commands] = (char *)(malloc(strlen(line) * sizeof(char)));
		strcpy(commands[total_commands], line);
		total_commands = total_commands + 1;
	}

	pthread_t write1;
	pthread_t read1;

	// Creates a thread which executes writer!
	if (pthread_create(&write1, NULL, writer, commands))
	{
		fprintf(stderr, "Error creating write thread\n");
		return 1;
	}

	// Creates a thread which executes reader!
	if (pthread_create(&read1, NULL, reader, NULL))
	{
		fprintf(stderr, "Error creating read thread\n");
		return 1;
	}

	// Ensure Threads Join Well
	if (pthread_join(write1, NULL))
	{
		fprintf(stderr, "Error joining write thread\n");
		return 2;
	}

	if (pthread_join(read1, NULL))
	{
		fprintf(stderr, "Error joining read thread\n");
		return 2;
	}

	// Did you really call the right functions?
	get_output();

	// I am a responsible memory user!
	int command_i;
	for (command_i = 0; command_i < total_commands; command_i++)
	{
		free(commands[command_i]);
	}
	return 0;
}
// ## DO NOT MODIFY
// ################################################################################
