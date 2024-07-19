#include "large_number_internal.h"

cs_codes large_number_add_helper(large_number *ln1, large_number ln2)
{
    if (ln2.size > ln1->cap)
    {
        ln1->cap = ln2.size + 10;
        ln1->vec = realloc(ln1->vec, ln1->cap);
        if (!ln1->vec)
            return CS_MEM;
    }

    int temp = 0, i;
    for (i = 0; i < ln1->size && i < ln2.size; i++)
    {
        ln1->vec[i] = ln1->vec[i] + ln2.vec[i] + temp;
        temp = ln1->vec[i] / 10;
        ln1->vec[i] %= 10;
    }

    while (i < ln1->size)
    {
        ln1->vec[i] = ln1->vec[i] + temp;
        temp = ln1->vec[i] / 10;
        ln1->vec[i] %= 10;
        i++;
    }

    while (i < ln2.size)
    {
        ln1->vec[i] = ln2.vec[i] + temp;
        temp = ln1->vec[i] / 10;
        ln1->vec[i] %= 10;
        i++;
    }

    while (temp != 0)
    {
        ln1->vec[i] = temp;
        temp = ln1->vec[i] / 10;
        ln1->vec[i] %= 10;
        i++;
    }

    ln1->size = i;

    return CS_SUCCESS;
}