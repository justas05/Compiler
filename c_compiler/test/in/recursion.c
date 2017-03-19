int fact(int n)
{
    if(n <= 1)
	return 1;
    else
	return fact(n-1) * n;
}

int main() {
    return fact(5) + fact(2) + fact(1);
}
