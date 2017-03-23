int goto_(int a)
{
    goto helloWorld;

start_:
    a += 2;

    goto end_;

helloWorld:
    a *= 3;
    goto start_;

end_:
    a -= 1;
    return a;
}
