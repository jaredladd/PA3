#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 10
sem_t mutex;
sem_t empty = N;
sem_t full = 0;

int count = 10;
int pthr = 5;
int cthr = 5;

void *producer(void *tid);
void *consumer(void *tid);

int main(int argc, char **argv)
{

    pthread_t thr;
    for (int i = 0; i < pthr; i++)
    {
        pthread_create(&thr, NULL, producer, (void *)i);
    }
    //pthread_create(&thr, NULL, producer, (void *)1);
    for (int i = pthr; i < pthr + cthr; i++)
    {
        pthread_create(&thr, NULL, consumer, (void *)i);
    }
    //pthread_create(&thr, NULL, consumer, (void *)2);
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);
    // for (int index_producer = 0; index_producer < 5; index_producer++)
    // {
    //     pthread_create(&thr[index_producer], NULL, producer, NULL);
    // }

    // for (int index_consumer = 5; index_consumer < 10; index_consumer++)
    // {
    //     pthread_create(&thr[index_consumer], NULL, consumer, NULL);
    // }

    // create -p # of producer threads
    // create -c # of consumer threads
    pthread_join(thr, NULL);

    pthread_cancel(thr);
    sem_destroy(&mutex);
    return 0;
}
void *producer(void *tid)
{
    while (1)
    {
        sem_wait(&empty);
        sem_wait(&mutex);
        // insert X into the first available slot in the buffer insert('X');
        printf("%d", (int)tid);
        sem_post(&mutex);
        sem_post(&full);
    }
}
void *consumer(void *tid)
{
    while (1)
    {
        sem_wait(&full);
        sem_wait(&mutex);
        // remove X from the last used slot in the buffer remove();
        printf("%d", (int)tid);

        sem_post(&mutex);
        sem_post(&empty);
    }
}
