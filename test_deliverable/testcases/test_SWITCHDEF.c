int switchdef(int a)
{
    switch(a)
    {
    case 0:
	return 38;
    case 1:
	return 2;
    case 2:
	a = 64;
	a += 23;
	a %= 23;
    default:
	a -= 4;
	break;
    }

    return a;
}
