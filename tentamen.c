#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;

void *ThreadSafePrintFile(void *input)
{
    const char *name = (const char *)input;

    pthread_mutex_lock(&m_mutex);

    FILE *filePtr = fopen(name, "r");

    if (filePtr == NULL)
        return NULL;

    char buffer[32];

    while (1)
    {
        int readSize = fread(buffer, 1, 31, filePtr);

        if (readSize == 0)
            break;

        buffer[readSize] = 0;
        printf("%s", buffer);
    }

    pthread_mutex_unlock(&m_mutex);

    return NULL;
}

int main()
{
    pthread_t thread;
    pthread_create(&thread, NULL, ThreadSafePrintFile, "test.txt");
    pthread_join(thread, NULL);
    getchar();
    return EXIT_SUCCESS;
}