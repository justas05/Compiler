int Fibonacci(int);

int main()
{
    int n, i = 0, c, res = 0;

    n = 10;
 
    for ( c = 1 ; c <= n ; c++ )
    {
	res = Fibonacci(i);
	i++; 
    }
 
    return res;
}

int Fibonacci(int n)
{
    if ( n == 0 )
	return 0;
    else if ( n == 1 )
	return 1;
    else
	return ( Fibonacci(n-1) + Fibonacci(n-2) );
}
