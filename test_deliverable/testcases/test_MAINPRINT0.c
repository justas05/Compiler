int printf(const char *format, ...);

int main(int argc, char *argv[])
{
    printf("program name : %s\nInput variables : %d\n", argv[0], argc);
    return 0;
}
