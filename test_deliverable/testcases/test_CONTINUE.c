int cont(int a, int b)
{
    int c = 0;
    while(a < b)
    {
	++a;
	if(a < b/2)
	{
	    continue;
	}
	else
	{
	    c += 2;
	}
    }

    return c;
}
