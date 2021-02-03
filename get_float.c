#include <ctype.h>
#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0;

int getch(void);
void ungetch(int);
float getfloat(float* pn);

float getfloat(float* pn) {
    int c, sign;

    while (isspace(c = getch()))
        ;
    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetch(c);
        return 0;
    }
    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-') {
        c = getch();
    }
    int dec = 0;
    for (*pn = 0; isdigit(c) || (c == '.' && dec == 0 ); c = getch()) {
        if (c == '.') {
            dec = 1;
            continue;
        }
        if (dec == 0) {
            *pn = 10 * *pn + (c - '0');
        } else {
            *pn += (c - '0') / (float)dec;
            dec *= 10;
        }
    }
    *pn *= sign;
    if (c != EOF) {
        ungetch(c);
    }
    return c;
}

int getch(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp >= BUFSIZE) {
        print("ungetch; too many characters\n");
    } else {
        buf[bufp++] = c;
    }
}