#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NOT_COMPLETED 0
#define COMPLETED 1

typedef struct threadInput
{
    int *value;
    volatile sig_atomic_t *flag;
} threadInput;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *threadFunction(void *input);

int main(int argc, char *argv[])
{
    int slots = 1;
    int threads = 8;
    int maxValue = 1000;
    int threads_started = 0;
    int *memory = malloc(sizeof(int) * slots);
    memset(memory, 0, sizeof(int) * slots);

    volatile sig_atomic_t *thread_completed = malloc(sizeof(sig_atomic_t) * threads);

    pthread_t handle[threads];

    for (int n = 0; n < threads; n++)
    {
        threadInput input;
        input.value = &memory[0];
        input.flag = thread_completed + n;
        pthread_create(&handle[n], NULL, threadFunction, &input);
        ++threads_started;
    }

    for (int i = 50; i < maxValue; i += 50)
    {
        printf("%i\n", i);
        while (threads_started < i)
        {
            for (int j = 0; j < threads && threads_started < i; j++)
            {
                if (*(thread_completed + j) == COMPLETED)
                {
                    threadInput input;
                    input.value = &memory[0];
                    input.flag = thread_completed + j;
                    pthread_create(&handle[j], NULL, threadFunction, &input);
                    ++threads_started;
                    // printf("Number of threads started: %d\n", threads_started);
                }
            }
        }
        for (int n = 0; n < threads; n++)
        {
            pthread_join(handle[n], NULL);
        }
        printf("Current value: %i\n", memory[0]);
        slots += 1;
        memory = realloc(memory, sizeof(int) * slots);
        memory[slots - 1] = memory[0];
        memory[0] = 0;
        threads_started = 0;
    }

    printf("%d\n", *memory);
    printf("Press any key to exit...\n");
    getchar();
    return EXIT_SUCCESS;
}

void *threadFunction(void *input)
{
    threadInput data = *(threadInput *)input;

    *data.flag = NOT_COMPLETED;

    pthread_mutex_lock(&mutex);
    *(data.value) += 1;
    pthread_mutex_unlock(&mutex);

    *(data.flag) = COMPLETED;
    return NULL;
}