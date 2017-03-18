int fact(int n)
{
	if(n <= 1) {
		return 1;
	} else {
		int x = fact(n-1);
		return x * n;
	}
}

int main() {
	return fact(5);
}
