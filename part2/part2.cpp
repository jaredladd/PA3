#include <iostream>
#include <stdio.h>
#include <stack>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>

int itemCount;
int pthr;
int cthr;
int buffer_size;

void *producer(void *tid);
void *consumer(void *tid);
void mon_insert(char alpha);
void mon_remove();

std::stack<char> buffer;

int main(int argc, char **argv)
{
    //reading command line args
    buffer_size = atoi(argv[2]);
    pthr = atoi(argv[4]);
    cthr = atoi(argv[6]);
    itemCount = atoi(argv[8]);

    //initialize the correct number of threads
    pthread_t thr[pthr + cthr];

    //create producer threads
    for (long i = 0; i < pthr; i++)
    {
        pthread_create(&thr[i], NULL, producer, (void *)i);
    }
    //create consumer threads
    for (long i = pthr; i < pthr + cthr; i++)
    {
        pthread_create(&thr[i], NULL, consumer, (void *)i);
    }
    //join the threads
    for (int i = 0; i < pthr + cthr; i++)
    {
        pthread_join(thr[i], NULL);
    }
    return 0;
}

void *producer(void *tid)
{
    char alpha;
    while (1)
    {
        alpha = 'a' + rand() % 26;
        mon_insert(alpha);
    }
}

void *consumer(void *tid)
{
    char result;
    while (1)
    {
        mon_remove();
    }
}

sem_t semThread;
// add more variables as necessary // define condition variable struct // define monitor struct

void mon_insert(char alpha)
{
    sem_wait(&semThread);
    // implement either Hoare or Mesa paradigm
    // synchronization and bookkeeping
    if (buffer.size() == buffer_size)
    {
        sem_post(&semThread);
    }
    else
    {
        buffer.push(alpha);
        printf("p:<>, item: %c, at %lu\n", alpha, buffer.size() - 1);
        sem_post(&semThread);
    }
}

void mon_remove()
{
    char result;
    sem_wait(&semThread);
    if (buffer.size() == 0)
    {
        sem_post(&semThread);
    }
    else
    {
        char result = buffer.top();
        buffer.pop();
        printf("c:<>, item: %c, at %lu\n", result, buffer.size() - 1);
        sem_post(&semThread);
    }
}