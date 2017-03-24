void pointer2(int *);

int main()
{
    int a = 83;
    pointer2(&a);
    return !( 238 == a );
}
