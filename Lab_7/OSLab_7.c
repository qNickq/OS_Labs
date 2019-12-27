#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#define NUM_READERS 10
#define NUM_WRITERS 1
#define BUFFER_SIZE 164

void *read_buff(void *buffer);
void *write_buff(void *buffer);

static pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

static int size = 0;

int main()
{
    pthread_rwlock_init(&rwlock, NULL);

    char buff[BUFFER_SIZE];

    pthread_t threadsR[NUM_READERS];
    pthread_t threadW;

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

    pthread_rwlock_destroy(&rwlock);
    return 0;
}

void *read_buff(void *buffer)
{
    while (size != BUFFER_SIZE)
    {
        pthread_rwlock_trywrlock(&rwlock);
        printf("Reader %d -> %s\n", (int)pthread_self(), (char*)buffer);
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
    
    return 0;
}

void *write_buff(void *buffer)
{
    while (size != BUFFER_SIZE)
    {
        pthread_rwlock_tryrdlock(&rwlock);
        sprintf((char*)buffer, "%d",size);
        printf("Writer -> %s\n", (char*)buffer);
        pthread_rwlock_unlock(&rwlock);

        sleep(1);
        size++;
    }
    return NULL;
}
