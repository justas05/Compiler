int f()
{
    int a = 5;
    int b = 8;
    {
	int b = 2;
	a = b-1;
    }

    return a * b;
}
