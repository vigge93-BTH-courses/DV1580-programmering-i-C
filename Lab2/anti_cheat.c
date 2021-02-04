#include <ftw.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THREADS 8
#define NOT_RUNNING 0
#define RUNNING 1

static int readFile(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);

void *threadFunction(void *input);

void clearRunningFlag(void *arg);

typedef struct threadInput
{
    const char *fname;
    int threadId;
    intmax_t fsize;
} threadInput;

pthread_t threadPool[THREADS];

volatile sig_atomic_t threadFlags[THREADS];

volatile threadInput threadData[THREADS];
volatile char *memory[THREADS];

int main(int argc, char *argv[])
{
    char path[] = "C:/Users/vigge93/Desktop/BTH/Programmering_i_C_DV1580_/Lab2/hash_test_tree";
    for (int i = 0; i < THREADS; i++)
    {
        threadFlags[i] = NOT_RUNNING;
    }
    nftw(path, readFile, 20, 0);
    getchar();
    return EXIT_SUCCESS;
}

static int readFile(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
    // printf("%-3s %2d ",
    //        (tflag == FTW_D) ? "d" : (tflag == FTW_DNR) ? "dnr"
    //                             : (tflag == FTW_DP)    ? "dp"
    //                             : (tflag == FTW_F)     ? "f"
    //                             : (tflag == FTW_NS)    ? "ns"
    //                             : (tflag == FTW_SL)    ? "sl"
    //                             : (tflag == FTW_SLN)   ? "sln"
    //                                                    : "???",
    //        ftwbuf->level);

    // if (tflag == FTW_NS)
    //     printf("-------");
    // else
    //     printf("%7jd", (intmax_t)sb->st_size);

    // printf("   %-40s %d %s\n",
    //        fpath, ftwbuf->base, fpath + ftwbuf->base);
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
        pthread_create(&threadPool[i], NULL, threadFunction, &threadData[i]);
    }
    return 0; /* To tell nftw() to continue */
}

void *threadFunction(void *input)
{
    threadInput data = *(threadInput *)input;
    int id = data.threadId;
    pthread_cleanup_push(clearRunningFlag, &id);
    threadFlags[id] = RUNNING;

    const char *filename = data.fname;
    int filesize = data.fsize;
    printf("Id: %d\tFilename: %s\tFilesize: %d\n", id, filename, filesize);

    pthread_cleanup_pop(1);

    return NULL;
}

void clearRunningFlag(void *args)
{
    int idx = *(int *)args;
    threadFlags[idx] = NOT_RUNNING;
}
