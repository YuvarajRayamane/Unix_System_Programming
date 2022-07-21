#include <stdio.h>
#include <setjmp.h>

void func1(jmp_buf buf);
void func2(jmp_buf buf);

int main() {
    printf("Entered the main function\n");
    jmp_buf buffer;
    int res = setjmp(buffer);

    if (res == 1) {
        printf("Calling user defined function 2 from main\n");
        func2(buffer);
    } else if (res == 2) {
        printf("Now res is 2\n");
    } else {
        printf("Calling user defined function 1 from main\n");
        func1(buffer);
    }
    return 0;
}

void func1(jmp_buf buf) {
    printf("Inside the user defined function 1\n");
    longjmp(buf, 1);
}

void func2(jmp_buf buf) {
    printf("Inside the user defined function 2\n");
    longjmp(buf, 2);
}
