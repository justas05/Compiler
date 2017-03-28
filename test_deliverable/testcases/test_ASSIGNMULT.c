int assignmult()
{
    int x[10][10];
    int j = 0;
    int k = 0;
    
    for(j = 0; j < 10; ++j)
	for(k = 0; k < 10; ++k)
	{
	    x[j][k] = j*10+k;
	}

    return x[4][2];
}
