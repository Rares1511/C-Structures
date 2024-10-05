#include <stdlib.h>
#include <string.h>

#include "../include/unittest.h"

/* initializes the test res structure with the given name for the test and returns 0 for success, -1 otherwise */
int test_res_init(test_res *res, char test_name[])
{
    res->return_code = 0;
    res->test_name = malloc(strlen(test_name) + 1);
    if (res->test_name == NULL)
    {
        return -1;
    }
    strcpy(res->test_name, test_name);
    res->reason = malloc(MAX_REASON_SIZE);
    if (res->reason == NULL)
    {
        return -1;
    }

    return 0;
}

void test_res_free(void *v_res)
{
    test_res *res = (test_res *)v_res;

    free(res->test_name);
    free(res->reason);
}