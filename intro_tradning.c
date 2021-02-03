#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *ThreadWork(void *input);

typedef struct ThreadData
{
    int data;
} ThreadData;

int m_sum = 0;
pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER; // Lås

int main(int argc, char *argv[])
{
    int num_thread = 1000;
    pthread_t handle[num_thread];

    ThreadData data;
    data.data = 1;

    for (int i = 0; i < num_thread; i++)
    {
        pthread_create(&handle[i], NULL, ThreadWork, &data);
    }

    for (int i = 0; i < num_thread; i++)
    {
        // pthread_detach(handle[i]);     // Strunta i returvärdet
        pthread_join(handle[i], NULL); // Vänta tills funktionen är klar
    }
    printf("%d\n", m_sum);

    pthread_mutex_destroy(&m_mutex);

    printf("Press any key to quit\n");
    getchar();
    return EXIT_SUCCESS;
}

void *ThreadWork(void *input)
{
    ThreadData data = *(ThreadData *)input;
    sleep(1);

    // Alternativ 1 (mutex)
    pthread_mutex_lock(&m_mutex);
    m_sum += 1;
    pthread_mutex_unlock(&m_mutex);

    // Alternativ 2 (atomic operations)
    __sync_fetch_and_add(&m_sum, 1);

    return NULL;
}