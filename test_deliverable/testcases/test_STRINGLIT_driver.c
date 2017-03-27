#include <string.h>

char *stringlit(char *);

int main()
{
    char a[50];
    return strcmp(stringlit(a), "Hello World!");
}
