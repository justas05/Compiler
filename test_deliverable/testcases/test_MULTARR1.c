char multiarr1(char a, char b, char c, char d, char e)
{
    char x[2][3] = {
	{a, b,},
	{c, d, e},
    };

    return x[1][2];
}
