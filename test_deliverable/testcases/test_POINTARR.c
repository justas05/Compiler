int pointarr(int *a, int b, int c, int d, int e)
{
    int x[2][2] = {
	{b, c},
	{d, e}
    };
    
    a = &x[0][1];
    return *a;
}
