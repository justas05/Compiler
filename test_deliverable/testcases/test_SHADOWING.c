int shadowing(int a, int b)
{
    {
	int a = 2;
	b = a;
    }

    return b + a;
}
