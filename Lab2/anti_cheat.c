#include <ftw.h>
#include <openssl/md5.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THREADS 8
#define NOT_RUNNING 0
#define RUNNING 1
#define CHUNK_SIZE 1024

static int readFile(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);

void *threadFunction(void *input);

void clearRunningFlag(void *arg);

typedef struct threadInput
{
    const char *fname;
    int threadId;
    intmax_t fsize;
    int fileId;
} threadInput;

pthread_t threadPool[THREADS];

volatile sig_atomic_t threadFlags[THREADS];

volatile unsigned char **combinedHash;
volatile sig_atomic_t combinedHashSize = 0;
pthread_mutex_t hashMutex = PTHREAD_MUTEX_INITIALIZER;

volatile threadInput threadData[THREADS];
volatile char memory[THREADS][CHUNK_SIZE];

volatile sig_atomic_t fileId = 0;

int main(int argc, char *argv[])
{
    char path[] = "C:/Users/vigge93/Desktop/BTH/Programmering_i_C_DV1580_/Lab2/hash_test_tree";
    combinedHash = malloc(THREADS * MD5_DIGEST_LENGTH);
    combinedHashSize = THREADS;
    for (int i = 0; i < THREADS; i++)
    {
        threadFlags[i] = NOT_RUNNING;
        combinedHash[i] = malloc(MD5_DIGEST_LENGTH);
    }
    nftw(path, readFile, THREADS, 0);
    for (int i = 0; i < THREADS; ++i)
    {
        pthread_join(threadPool[i], NULL);
    }
    unsigned char finalHash[MD5_DIGEST_LENGTH];
    // MD5_Update
    MD5_CTX context;
    MD5_Init(&context);
    for (int i = 0; i < combinedHashSize; i++)
    {
        MD5_Update(&context, combinedHash[i], MD5_DIGEST_LENGTH);
    }
    MD5_Final(finalHash, &context);

    // XOR
    // for (int i = 0; i < combinedHashSize; i++)
    // {
    //     for (int j = 0; j < MD5_DIGEST_LENGTH; j++)
    //     {
    //         finalHash[j] = finalHash[j] ^ combinedHash[i][j];
    //     }
    // }
    char StringMD5[33];
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        sprintf(&StringMD5[i * 2], "%02x", finalHash[i]);
    }
    printf("Final hash: %s\n", StringMD5);
    getchar();
    free(combinedHash);
    return EXIT_SUCCESS;
}

static int readFile(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
    if (tflag == FTW_F)
    {
        int i = 0;
        while (threadFlags[i] != NOT_RUNNING)
        {
            i = (i + 1) % THREADS;
        }
        threadData[i].fname = malloc(strlen(fpath) + 1);
        strcpy(threadData[i].fname, fpath);
        threadData[i].threadId = i;
        threadData[i].fsize = (intmax_t)sb->st_size;
        threadData[i].fileId = fileId++;
        threadFlags[i] = RUNNING;
        pthread_create(&threadPool[i], NULL, threadFunction, &threadData[i]);
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
    int filesize = data.fsize;
    int fileId = data.fileId;
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_CTX context;
    MD5_Init(&context);

    printf("Id: %d\tFilename: %s\tFilesize: %d\n", id, filename, filesize);

    FILE *file = fopen(filename, "rb");
    int readSize;
    do
    {
        readSize = fread(memory[id], 1, CHUNK_SIZE, file);
        MD5_Update(&context, memory[id], readSize);
    } while (readSize > 0);
    fclose(file);
    MD5_Final(hash, &context);

    pthread_mutex_lock(&hashMutex);
    while (fileId >= combinedHashSize)
    {
        combinedHash = realloc(combinedHash, combinedHashSize * 2 * MD5_DIGEST_LENGTH);
        combinedHashSize *= 2;
        for (int i = combinedHashSize / 2; i < combinedHashSize; i++)
        {
            combinedHash[i] = malloc(MD5_DIGEST_LENGTH);
        }
    }
    strcpy(combinedHash[fileId], hash);
    pthread_mutex_unlock(&hashMutex);

    pthread_cleanup_pop(1);

    return NULL;
}

void clearRunningFlag(void *args)
{
    int idx = *(int *)args;
    threadFlags[idx] = NOT_RUNNING;
}
