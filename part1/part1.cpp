#include <iostream>
#include <stdio.h>
#include <stack>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>

sem_t mutex;
sem_t empty;
sem_t full;
sem_t count_mutex;

int count;
int pthr;
int cthr;
int buffer_size;

void *producer(void *tid);
void *consumer(void *tid);

std::stack<char> buffer;

int main(int argc, char **argv)
{
    //reading command line args
    buffer_size = atoi(argv[2]);
    pthr = atoi(argv[4]);
    cthr = atoi(argv[6]);
    count = atoi(argv[8]);

    //initialize the correct number of threads
    pthread_t thr[pthr + cthr];
    //initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&count_mutex, 0, 1);
    sem_init(&empty, 0, buffer_size);
    sem_init(&full, 0, 0);
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
    //destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&count_mutex);

    return 0;
}
void *producer(void *tid)
{

    while (1)
    {
        //code provided in handout
        sem_wait(&empty);
        sem_wait(&mutex);
        // use a binary semaphore to make sure only 1 thread accesses count at a time
        sem_wait(&count_mutex);
        //if there are items left to be added to the buffer, add one
        if (count > 0)
        {
            buffer.push('X');
            count--;
        }
        //if not, exit the thread and post semaphores
        else
        {
            sem_post(&count_mutex);
            sem_post(&mutex);
            sem_post(&full);
            pthread_exit(NULL);
        }
        //post the count semaphore now that we are done using that variable
        sem_post(&count_mutex);
        printf("p:<%lu>, item: %c, at %lu\n", (long unsigned int)tid, 'X', buffer.size() - 1);
        //code provided in handout
        sem_post(&mutex);
        sem_post(&full);
    }
}
void *consumer(void *tid)
{
    while (1)
    {
        //code provided in handout
        sem_wait(&full);
        sem_wait(&mutex);
        //if there are no items left to be put in the buffer, and the buffer is empty, it's time to exit
        if (count == 0 && buffer.empty())
        {
            sem_post(&mutex);
            sem_post(&empty);
            pthread_exit(NULL);
        }
        // but if the buffer is not empty, take something out of it
        else if (!buffer.empty())
        {
            buffer.pop();
            printf("c:<%lu>, item: %c, at %lu\n", (long unsigned int)tid, 'X', buffer.size());
        }
        //code provided in handout
        sem_post(&mutex);
        sem_post(&empty);
    }
}
