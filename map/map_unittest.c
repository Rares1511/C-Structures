#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/map.h"
#include "../include/unittest.h"

void print_int(void *el)
{
    printf("%d", *(int *)el);
}

test_res test_map_create_free_empty()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_CREATE_FREE_EMPTY";

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc;

    key_attr.comp = NULL;
    key_attr.fr = NULL;
    key_attr.print = print_int;
    key_attr.copy = NULL;
    key_attr.size = sizeof(int);

    val_attr.comp = NULL;
    val_attr.fr = NULL;
    val_attr.print = print_int;
    val_attr.copy = NULL;
    val_attr.size = sizeof(int);

    rc = map_init(&m, key_attr, val_attr);
    if (rc != CS_SUCCESS)
    {
        res.return_code = rc;
        return res;
    }

    map_free(&m);

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("%s time: %f\n", test_name, time_taken);
    res.return_code = CS_SUCCESS;

    return res;
}

test_res test_map_insert_one_elem()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_INSERT_ONE_ELEM";

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key = 6, val = 9;

    key_attr.comp = NULL;
    key_attr.fr = NULL;
    key_attr.print = print_int;
    key_attr.copy = NULL;
    key_attr.size = sizeof(int);

    val_attr.comp = NULL;
    val_attr.fr = NULL;
    val_attr.print = print_int;
    val_attr.copy = NULL;
    val_attr.size = sizeof(int);

    rc = map_init(&m, key_attr, val_attr);
    if (rc != CS_SUCCESS)
    {
        res.return_code = rc;
        return res;
    }

    key = 6;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        res.return_code = rc;
        return res;
    }

    map_print(m);

    map_free(&m);

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("%s time: %f\n", test_name, time_taken);
    res.return_code = CS_SUCCESS;

    return res;
}

test_res test_map_insert_two_elem()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_INSERT_TWO_ELEM";

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key, val;

    key_attr.comp = NULL;
    key_attr.fr = NULL;
    key_attr.print = print_int;
    key_attr.copy = NULL;
    key_attr.size = sizeof(int);

    val_attr.comp = NULL;
    val_attr.fr = NULL;
    val_attr.print = print_int;
    val_attr.copy = NULL;
    val_attr.size = sizeof(int);

    rc = map_init(&m, key_attr, val_attr);
    if (rc != CS_SUCCESS)
    {
        res.return_code = rc;
        return res;
    }

    key = 6;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        res.return_code = rc;
        return res;
    }

    key = 5;
    val = 69;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        res.return_code = rc;
        return res;
    }

    map_print(m);

    map_free(&m);

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("%s time: %f\n", test_name, time_taken);
    res.return_code = CS_SUCCESS;

    return res;
}

int main()
{
    int i;
    test tests[] = {test_map_insert_two_elem};
    test_res res;

    for (i = 0; i < MAP_TEST_SIZE && i < (int)(sizeof(tests) / sizeof(test)); i++)
    {
        res = tests[i]();
        if (res.return_code != CS_SUCCESS)
        {
            printf("%s....................FAILED\n", res.test_name);
            exit(-(int)(sizeof(tests) / sizeof(test) - i));
        }
        else
        {
            printf("%s....................SUCCESS: %d/%d\n", res.test_name, i + 1, (int)(sizeof(tests) / sizeof(test)));
        }
        free_test_res(&res);
    }

    return 0;
}