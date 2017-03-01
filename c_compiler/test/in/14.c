int function_1(int a, int b) {
    int x = 0;

    do {
	int c = a + b;
	x += a;
	x = x * b;
	x -= c;
    } while(x < 500);

    return x;
}
