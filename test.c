#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int main() {
//     int number = 0;
//     int age = 19;
//     int* ptr_age;

//     ptr_age = &age;

//     number = *ptr_age;

//     printf("Adress of age: %p\n", &age);
//     printf("Value of ptr_age: %p\n", ptr_age);
//     printf("Value of number: %d\n", number);
//     printf("Value where ptr_age points: %d\n", *ptr_age);

//     *ptr_age = 22;

//     printf("Value of age: %d\n", age);

//     return EXIT_SUCCESS;
// }

char *memory[2];

int main()
{
    memory[0] = "Hej";
    memory[1] = "Världen";
    printf("%s %s\n", memory[0], memory[1]);
    memory[0] = "Hejsan";
    printf("%s %s\n", memory[0], memory[1]);
    getchar();
    return 0;
}