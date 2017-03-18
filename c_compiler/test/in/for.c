int main()
{
    int x, y, z;

    y = 0;
    for(x = 0; x < 50; x = x+1) {
	y = y + 1;
    }

    z = y + x - 5;

    return z;
}
