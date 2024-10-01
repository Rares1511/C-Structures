#include <stdlib.h>

#include "../include/unittest.h"

void free_test_res(void *v_res)
{
    test_res *res = (test_res *)v_res;

    free(res->test_name);
}