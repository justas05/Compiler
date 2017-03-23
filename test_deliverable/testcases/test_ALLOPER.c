int alloper(int a, int b, int c, int d, int f)
{
    a = b * c + d - f;
    {
	int c = 2;
	b += c + 2;
    }
    a -= b;
    d = a - c;
    
    if((a == b || a != c || b > d || f < d) && a)
    {
	a = 2 * b;
    }
    else
    {
	a -= d;
    }

    do
    {
	a += 1;
	f++;
    }
    while(f < 50);
    
    return a;
}
