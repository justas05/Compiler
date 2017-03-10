int main()
{
	int a = 5;
	int b = 49;
	int c = 239;
	int d = 23;
	int f = 234;
	int g = 12;
	
	a = d;
	b = f;
	g = f;
	a = g;
	d = g;
	a = d;
	g = c;
	c = a;
	a = f;
	return a;
}
