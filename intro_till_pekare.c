#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_mem(size_t size, int* ptr);

int main() {

    int Int1 = 0;
    int Int2 = 0;
    int Int3 = 0;
    int Int4 = 0;

    printf("%p\n%p\n%p\n%p\n", &Int1, &Int2, &Int3, &Int4);

    printf("%ld\n%ld\n%ld\n", &Int1 - &Int2, &Int2 - &Int3, &Int3 - &Int4);

    char* pChar1 = (char*)&Int1;
    char* pChar2 = (char*)&Int2;
    char* pChar3 = (char*)&Int3;
    char* pChar4 = (char*)&Int4;

    printf("%ld\n%ld\n%ld\n", pChar1 - pChar2, pChar2 - pChar3, pChar3 - pChar4);

    pChar1[0] = 1;
    pChar1[1] = 2;
    pChar1[2] = 3;
    pChar1[3] = 4;

    printf("%d\n%d\n%d\n%d\n", Int1, Int2, Int3, Int4);

    char* ptr = (char*)&Int2;

    memcpy(ptr, pChar1, 4);

    printf("%d\n%d\n%d\n%d\n", Int1, Int2, Int3, Int4);

    void* memory = malloc(sizeof(int) * 16);
    void* memory2 = malloc(sizeof(int) * 16);

    int* intPtr = (int*)memory;
    int* intPtr2 = (int*)memory2;

    for(int i = 0; i < 16; ++i) {
        *(intPtr2+i) = i;
    }

    print_mem(32, intPtr);

    memset(intPtr, 0, sizeof(int) * 16);

    print_mem(32, intPtr);

    printf("%p\n%p\n", intPtr, intPtr2);

    intPtr = realloc(intPtr, sizeof(int) * 32);
    
    printf("%p\n%p\n", intPtr, intPtr2);
    
    print_mem(32, intPtr);
    print_mem(32, intPtr2);
    
    int* intPtr3 = intPtr;

    for(int i = 0 ; i < 32; ++i) {
        *intPtr3 = i;
        intPtr3++;
    }

    print_mem(32, intPtr);

    free(intPtr);

    intPtr2 = realloc(intPtr2, 32);

    memset(intPtr2, 1, sizeof(int) * 32);

    print_mem(32, intPtr2);
    print_mem(32, intPtr);

    return EXIT_SUCCESS;
}

void print_mem(size_t size, int* ptr) {
    for(size_t i = 0; i < size; i++) {
        printf("%d ", *(ptr+i));
    }
    printf("\n");
}