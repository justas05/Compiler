int switch_(int a, int b)
{
    switch(a)
    {
    case 0:
	a = 5;
	break;
    case 1:
	a = b+5;
	break;
    case 9:
	b = 4;
	break;
    }

    b = a;
    return b;
}
