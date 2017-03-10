int main() {
	int x = 23;
	int y = 52;

	x = x + y;
	x = x + x;
	y = x - y;
	x = x - y;

	return x;
}
