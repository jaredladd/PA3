#include <iostream>
#include <stdio.h>
#include <stack>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 1
sem_t mutex;
sem_t empty;
sem_t full;

int count = 10;
int pthr = 5;
int cthr = 5;

void *producer(void *tid);
void *consumer(void *tid);

std::stack<char> buffer;

int main(int argc, char **argv)
{

    pthread_t thr[pthr + cthr];
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);
    for (long i = 0; i < pthr; i++)
    {
        pthread_create(&thr[i], NULL, producer, (void *)i);
    }

    for (long i = pthr; i < pthr + cthr; i++)
    {
        pthread_create(&thr[i], NULL, consumer, (void *)i);
    }

    // create -p # of producer threads
    // create -c # of consumer threads

    for (int i = 0; i < pthr + cthr; i++)
    {
        pthread_join(thr[i], NULL);
    }

    // if (count == 0)
    // {
    //     // for (int i = 0; i < pthr + cthr; i++)
    //     // {
    //     //     pthread_cancel(thr[i]);
    //     // }
    //     pthread_cancel(thr);
    //     sem_destroy(&mutex);
    // }

    return 0;
}
void *producer(void *tid)
{
    std::cout << count;
    while (count != 0)
    {
        //printf("%d", empty);
        sem_wait(&empty);
        sem_wait(&mutex);
        // insert X into the first available slot in the buffer insert('X');
        buffer.push('X');
        count--;
        //std::cout << "p: " << (long)tid << " item: X at " << buffer.size() - 1 << '\n';

        printf("p:<%lu>, item: %c, at %lu\n", (long unsigned int)tid, 'X', buffer.size());
        sem_post(&mutex);
        sem_post(&full);
    }
    exit(0);
}
void *consumer(void *tid)
{
    while (1)
    {
        sem_wait(&full);
        sem_wait(&mutex);
        // remove X from the last used slot in the buffer remove();
        //std::cout << (long)tid << '\n';

        buffer.pop();
        //std::cout << "c: " << (long)tid << " item: X at " << buffer.size() << '\n';
        printf("c:<%lu>, item: %c, at %lu\n", (long unsigned int)tid, 'X', buffer.size());

        sem_post(&mutex);
        sem_post(&empty);

        //std::cout << count << '\n';
    }
    exit(0);
}