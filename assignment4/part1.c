/*
 * @Author       : Hao Zhang
 * @Date         : 2021-06-04 18:03:58
 * @LastEditors  : Hao Zhang
 * @LastEditTime : 2021-06-05 23:55:47
 * @FilePath     : \system programming\assignment4\part1.c
 */
#include "slow_functions.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
static pthread_mutex_t lock;
int main()
{
    int pok;
    int count = 0;
    int i;
    //create child  pthread for slow_function1
    pthread_t func1;
    for (i = 0; i < 10; i++) // create 10 child threads in order to make special to 10
    {
        //ten child thread executes at the same time, so they will add one by one than sleep together
        //, and by lock, they will not make a mess
        pok = pthread_create(&func1, NULL, &slow_function1, NULL);
        if (pok != 0)
            printf("cannot produce func1\n");
    }

    //create child pthread for slow_function1
    pthread_t func2;
    pok = pthread_create(&func2, NULL, &slow_function2, NULL);
    if (pok != 0)
        printf("cannot produce func2\n");

    //wait for thread func2 finish, then return
    //note that func1 is not need to finish because we have already got 'Work Done'
    int jok = pthread_join(func2, NULL);
    if (jok != 0)
        printf("cannot join\n");
    return 0;
}