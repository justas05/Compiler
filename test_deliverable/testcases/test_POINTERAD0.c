int pointeradd(int *a);

int main()
{
    int x[5] = {1, 2, 3, 4, 5};
    return !( 4 == pointeradd(&x[1]) );
}
