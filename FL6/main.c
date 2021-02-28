#include "list.h"
#include <stdio.h>
#include <stdlib.h>

void add_testdata();

int main()
{
    int i, value;
    char key[256];
    extern int count;
    char fname[512];

    add_testdata();

    while (1)
    {
        printf("1. Insert key value.\n");
        printf("2. Print value of key.\n");
        printf("3. Print all.\n");
        printf("4. Save to file.\n");
        printf("0. Exit\n");

        scanf("%d", &i);

        switch (i)
        {
        case 1:
            printf("Enter a key\n");
            scanf("%s", key);
            printf("Enter a value\n");
            scanf("%d", &value);
            insert_at_end(key, value);
            break;
        case 2:
            printf("Enter a key\n");
            scanf("%s", key);
            printf("-------> %d\n", get_element(key));
            break;
        case 3:
            printf("-------> ");
            print_all();
            break;
        case 4:

            printf("Enter filename\n");
            scanf("%s", fname);
            print_file(fname);
            break;
        case 0:
            return 0;
            break;
        default:
            printf("Please enter valid input.\n");
            break;
        }
        printf("Number of elements: %d\n\n", count);
    }
}

void add_testdata()
{
    insert_at_end("kalle", 23);
    insert_at_end("pelle", 44);
    insert_at_end("nisse", 345);
    //    insert_at_end(NULL, 345);
    insert_at_end("anna", 234);
}