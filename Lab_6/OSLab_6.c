#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#define NUM_READERS 10
#define NUM_WRITERS 1
#define BUFFER_SIZE 164

void *read_buff(void *buffer);
void *write_buff(void *buffer);

static pthread_mutex_t mutexR;
static pthread_mutex_t mutexW;

static int size = 0;

int main()
{
    char buff[BUFFER_SIZE];

    pthread_t threadsR[NUM_READERS];
    pthread_t threadW;

    pthread_mutex_init(&mutexR, 0);
    pthread_mutex_init(&mutexW, 0);

    pthread_create(&threadW, 0, write_buff, buff);

    for (int i = 0; i < NUM_READERS; ++i)
    {
        int threadR = pthread_create(&threadsR[i], 0, read_buff, buff);

        if (threadR != 0)
        {
            printf("Thread cant be created!");
            return -1;
        }
    }

    pthread_join(threadW, NULL);

    for (int i = 0; i < NUM_READERS; ++i)
    {
        pthread_join(threadsR[i], 0);
    }

    return 0;
}

void *read_buff(void *buffer)
{
    while (size != BUFFER_SIZE)
    {
        pthread_mutex_lock(&mutexW);
        printf("Reader %d -> %s\n", (int)pthread_self(), (char*)buffer);
        pthread_mutex_unlock(&mutexW);
        sleep(1);
    }
    return 0;
}

void *write_buff(void *buffer)
{
    while (size != BUFFER_SIZE)
    {
        pthread_mutex_lock(&mutexW);

        sprintf((char*)buffer, "%d",size);

        printf("Writer -> %s\n", (char*)buffer);

        pthread_mutex_unlock(&mutexW);
        sleep(1);
        size++;
    }
    return NULL;
}
