int f()
{}

int g = 2;

int x(int y) {
	int z = 3;

	if(y < z || g < z) {
		int r;
		++y;
	} else if(y == z) {
		int f;
		--y;
	} else return y;
	
	return g;
}
