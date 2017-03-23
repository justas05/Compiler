int not(int);

int main()
{
    return !( 5 == (not(0) * 5 + not(4)) );
}
