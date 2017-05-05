int sort(int *, int);

int main()
{
	int x[5]={5, 2, 3, 7, 4};
	sort(x, 5);
	return !( x[0]==2 && x[1]==3 && x[2]==4 && x[3]==5 && x[4]==7 );
}
