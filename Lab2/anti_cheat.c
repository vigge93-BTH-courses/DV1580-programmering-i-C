#include <ftw.h>
#include <omp.h>
#include <openssl/md5.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THREADS 10
#define NOT_RUNNING 0
#define RUNNING 1
#define CHUNK_SIZE 1024 * 1024 * 8

static int readFile(const char *fpath, const struct stat *sb, int tflag);

void *threadFunction(void *input);

void clearRunningFlag(void *arg);

typedef struct threadInput
{
    char fname[512];
    int threadId;
    int fileId;
} threadInput;

pthread_t threadPool[THREADS];

volatile sig_atomic_t threadFlags[THREADS];

unsigned char **combinedHash;
sig_atomic_t combinedHashSize = 0;
pthread_mutex_t hashMutex = PTHREAD_MUTEX_INITIALIZER;

threadInput threadData[THREADS];
char memory[THREADS][CHUNK_SIZE];

sig_atomic_t fileId = 0;

const char excludeExt[] = ".cfg";

int main(int argc, char *argv[])
{
    double t0 = omp_get_wtime();
    if (argc < 2)
    {
        printf("Too few arguments");
        return EXIT_FAILURE;
    }
    char *path = argv[1];
    char *trueHash = argc >= 3 ? argv[2] : "";

    combinedHash = malloc(THREADS * MD5_DIGEST_LENGTH);
    combinedHashSize = THREADS;
    for (int i = 0; i < THREADS; i++)
    {
        threadFlags[i] = NOT_RUNNING;
        combinedHash[i] = malloc(MD5_DIGEST_LENGTH);
    }

    ftw(path, readFile, THREADS);

    for (int i = 0; i < THREADS; ++i)
    {
        while (threadFlags[i] == RUNNING)
            ;
    }

    unsigned char finalHash[MD5_DIGEST_LENGTH] = {0};

    // MD5_Update
    MD5_CTX context;
    MD5_Init(&context);
    for (int i = 0; i < fileId; i++)
    {
        MD5_Update(&context, combinedHash[i], MD5_DIGEST_LENGTH);
    }
    MD5_Final(finalHash, &context);

    // XOR
    // for (int i = 0; i < fileId; i++)
    // {
    //     for (int j = 0; j < MD5_DIGEST_LENGTH; j++)
    //     {
    //         finalHash[j] = finalHash[j] ^ combinedHash[i][j];
    //     }
    // }

    double time = omp_get_wtime() - t0;

    char StringMD5[33];
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        sprintf(&StringMD5[i * 2], "%02x", finalHash[i]);
    }

    printf("True hash: %s\n", trueHash);
    printf("Game hash: %s\n", StringMD5);
    strcmp(StringMD5, trueHash) == 0 ? printf("Game files OK\n") : printf("Game files differ!\n");
    printf("Time consumed %fs\n", time);

    for (int i = 0; i < combinedHashSize; i++)
    {
        free(combinedHash[i]);
    }
    free(combinedHash);
    printf("Press enter to exit...\n");
    getchar();
    return EXIT_SUCCESS;
}

static int readFile(const char *fpath, const struct stat *sb, int tflag)
{
    if (tflag == FTW_F && strstr(fpath, excludeExt) == NULL)
    {
        int i = 0;
        while (threadFlags[i] == RUNNING)
        {
            i = (i + 1) % THREADS;
        }
        strcpy(threadData[i].fname, fpath);
        threadData[i].threadId = i;
        threadData[i].fileId = fileId++;
        threadFlags[i] = RUNNING;
        pthread_create(&threadPool[i], NULL, threadFunction, &threadData[i]);
        pthread_detach(threadPool[i]);
    }
    return 0;
}

void *threadFunction(void *input)
{
    threadInput data = *(threadInput *)input;
    int id = data.threadId;

    pthread_cleanup_push(clearRunningFlag, &id);
    threadFlags[id] = RUNNING;

    const char *filename = data.fname;
    int fileId = data.fileId;

    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_CTX context;
    MD5_Init(&context);

    FILE *file = fopen(filename, "rb");
    int readSize;
    do
    {
        readSize = fread(memory[id], 1, CHUNK_SIZE, file);
        MD5_Update(&context, memory[id], readSize);
    } while (readSize > 0);
    MD5_Final(hash, &context);
    fclose(file);

    pthread_mutex_lock(&hashMutex);
    while (fileId >= combinedHashSize)
    {
        combinedHash = realloc(combinedHash, combinedHashSize * 2 * MD5_DIGEST_LENGTH);
        combinedHashSize *= 2;
        for (int i = combinedHashSize / 2; i < combinedHashSize; ++i)
        {
            combinedHash[i] = malloc(MD5_DIGEST_LENGTH);
        }
    }
    memcpy(combinedHash[fileId], hash, MD5_DIGEST_LENGTH);
    pthread_mutex_unlock(&hashMutex);

    pthread_cleanup_pop(1);

    return NULL;
}

void clearRunningFlag(void *args)
{
    int idx = *(int *)args;
    threadFlags[idx] = NOT_RUNNING;
}