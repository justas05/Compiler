int iterate(int a, int b)
{
    while(a < b)
    {
	if(a == b / 2)
	{
	    break;
	}
	++a;
    }

    return a;
}
