#include <ftw.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREADS 8

static int readFile(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);

void *threadFunction(void *input);

typedef struct threadInput
{
    const char *fname;
    int fsize;
} threadInput;

pthread_t threadPool[THREADS];

int main(int argc, char *argv[])
{
    char path[] = "C:/Users/vigge93/Desktop/BTH/Programmering_i_C_DV1580_/Lab2/hash_test_tree";
    nftw(path, readFile, 20, 0);
    getchar();
    return EXIT_SUCCESS;
}

static int readFile(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
    printf("%-3s %2d ",
           (tflag == FTW_D) ? "d" : (tflag == FTW_DNR) ? "dnr"
                                : (tflag == FTW_DP)    ? "dp"
                                : (tflag == FTW_F)     ? "f"
                                : (tflag == FTW_NS)    ? "ns"
                                : (tflag == FTW_SL)    ? "sl"
                                : (tflag == FTW_SLN)   ? "sln"
                                                       : "???",
           ftwbuf->level);

    if (tflag == FTW_NS)
        printf("-------");
    else
        printf("%7jd", (intmax_t)sb->st_size);

    printf("   %-40s %d %s\n",
           fpath, ftwbuf->base, fpath + ftwbuf->base);

    return 0; /* To tell nftw() to continue */
}

void *threadFunction(void *input)
{
}
