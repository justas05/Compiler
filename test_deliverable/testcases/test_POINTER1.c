int pointer1(int a)
{
    int *b = &a;

    a += *b;
    return a;
}
