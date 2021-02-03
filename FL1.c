#include <stdio.h>

int addera(int a, int b);
int multi(int a, int b);
void count();

int main() {
    printf("Summan är: %i\n", addera(5, 8));
    printf("Produkten är: %i\n", multi(5, 8));
    count();
    return 0;
}

int addera(int a, int b) {
    return a + b;
}

int multi(int a, int b) {
    return a * b;
}

void count() {
    for (uint8_t i = 1; i > 0; i++) {
        printf("%i\n", i);
    }
}