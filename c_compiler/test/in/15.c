int add(int a, int b)
{
    int c = a + b;
    return c;
}

int sub(int a, int b)
{
    int c = a - b;
    return c;
}

int mult_by_5(int a)
{
    int b = 5;
    return a * b;
}

int add_5(int a)
{
    int i;
    for(i = 0; i < 5; ++i)
    {
	int b = ++a;
    }

    return a;
}
