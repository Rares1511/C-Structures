void universal_swap(void *el1, void *el2, int len)
{
    unsigned char *p = el1, *q = el2, tmp;
    for (int i = 0; i != len; i++)
    {
        tmp = p[i];
        p[i] = q[i];
        q[i] = tmp;
    }
}

int universal_compare(const void *el1, const void *el2, int len)
{
    const unsigned char *p = el1, *q = el2;
    for (int i = 0; i != len; i++)
    {
        if (q[i] != p[i])
            return p[i] - q[i];
    }
    return 0;
}