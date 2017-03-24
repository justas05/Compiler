void empty(int *a);

int main()
{
    int a = 39;
    empty(&a);
    return !( 13 == a );
}
