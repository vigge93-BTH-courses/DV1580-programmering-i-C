#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#define NOT_COMPLETED 0
#define COMPLETED 1
#define MAX_VALUE 200000
#define STEP 50
#define THREADS 1

pthread_mutex_t mutex[THREADS];

void *threadFunction(void *input);

volatile int *memory;

volatile sig_atomic_t current_index = 0;

int main(int argc, char *argv[])
{
    struct timeval begin, end;
    gettimeofday(&begin, 0);
    memory = malloc(sizeof(int) * THREADS);
    memset(memory, 0, sizeof(int) * THREADS);

    pthread_t handle[THREADS];

    for (int i = 0; i < THREADS; i++)
    {
        mutex[i] = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_lock(&mutex[i]);
    }
    for (int n = 0; n < THREADS; n++)
    {
        pthread_create(&handle[n], NULL, threadFunction, (int *)n);
    }
    for (int n = 0; n < THREADS; n++)
    {
        pthread_join(handle[n], NULL);
    }
    gettimeofday(&end, 0);
    for (int i = 0; i < current_index; i++)
    {
        printf("Memory at index %d:\t%d\n", i, memory[i]);
    }
    printf("Execution time: %.3f seconds\n", (end.tv_sec - begin.tv_sec) + (end.tv_usec - begin.tv_usec) * 1e-6);
    printf("Press enter to exit...\n");
    getchar();
    free(memory);
    return EXIT_SUCCESS;
}

void *threadFunction(void *input)
{
    int id = (int)input;
    int idx = current_index++;
    pthread_mutex_unlock(&mutex[id]);

    while (1)
    {
        for (int i = 0; i < (idx + 1) * STEP; ++i)
        {
            pthread_mutex_lock(&mutex[id]);
            ++memory[idx];
            pthread_mutex_unlock(&mutex[id]);
        }
        for (int i = 0; i < THREADS; i++)
        {
            pthread_mutex_lock(&mutex[i]);
        }
        if (current_index >= MAX_VALUE / STEP)
        {
            for (int i = 0; i < THREADS; i++)
            {
                pthread_mutex_unlock(&mutex[i]);
            }
            break;
        }
        idx = current_index++;
        memory = realloc(memory, sizeof(int) * current_index);
        memory[current_index - 1] = 0;
        for (int i = 0; i < THREADS; i++)
        {
            pthread_mutex_unlock(&mutex[i]);
        }
    }
    return NULL;
}