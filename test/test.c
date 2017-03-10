#include "stdio.h"

int main() {

    int x = 4;
    int y = 5;
    int a = 6;

    float b = 2.f;

    float c = b / 3.f; 

    int z = x + y + a;

    printf("%.4f\n", c);
    
    return (int)c;
}
