void quickSort(int*, int, int);

int main()
{
	int a[9] = { 7, 12, 1, -2, 0, 15, 4, 11, 9};
	quickSort( a, 0, 8);

	return !( a[0]==-2 && a[1]==0 && a[2]==1 && a[3]==4 && a[4]==7 && a[5]==9 && a[6]==11 && a[7]==12 && a[8]==15 );
}
