#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include "../include/map.h"
#include "../map/map_internal.h"
#include "../include/unittest.h"

/****************************** HELPER FUNCTIONS ******************************/
void print_int(void *el)
{
    printf("%d", *(int *)el);
}

bool validate_map_tree(map_node *node, int black_count, int *black_height)
{
    if (node == NULL)
    {
        if (*black_height == -1)
        {
            *black_height = black_count;
        }
        else if (black_count != *black_height)
        {
            printf("Black height mismatch at a leaf\n");
            return false;
        }
        return true;
    }

    if (node->color == RED)
    {
        if ((node->left_child && node->left_child->color != BLACK) || (node->right_child && node->right_child->color != BLACK))
        {
            printf("Red node has a red child\n");
            return false;
        }
    }

    if (node->color == BLACK)
    {
        black_count++;
    }

    bool left_valid = validate_map_tree(node->left_child, black_count, black_height);
    bool right_valid = validate_map_tree(node->right_child, black_count, black_height);

    return (left_valid & right_valid);
}

bool check_map_validity(map m)
{
    if (m.root == NULL || m.root->color == BLACK)
        return true;

    if (m.root && m.root->color != BLACK)
        return false;

    int black_height = -1;
    return validate_map_tree(m.root, 0, &black_height);
}

/****************************** UNITTEST FUNCTIONS ******************************/
test_res test_map_create_free_empty()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_CREATE_FREE_EMPTY";
    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc;
    bool valid_tree;

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

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

    valid_tree = check_map_validity(m);
    if (!valid_tree)
    {
        res.return_code = CS_FUNC;
        map_free(&m);
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
    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key = 6, val = 9;
    bool valid_tree;

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

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
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    valid_tree = check_map_validity(m);
    if (!valid_tree)
    {
        map_free(&m);
        res.return_code = CS_FUNC;
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
    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key, val;
    bool valid_tree;

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

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
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    valid_tree = check_map_validity(m);
    if (!valid_tree)
    {
        map_free(&m);
        res.return_code = CS_FUNC;
        return res;
    }

    map_print(m);

    map_free(&m);

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("%s time: %f\n", test_name, time_taken);
    res.return_code = CS_SUCCESS;

    return res;
}

test_res test_map_insert_five_elem()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_INSERT_FIVE_ELEM";
    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key, val, i;
    bool valid_tree;

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

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

    for (i = 0; i < 5; i++)
    {
        key = 2 * i * i - 3 * i + 5;
        val = 5;
        rc = map_insert(&m, &key, &val);
        if (rc != CS_SUCCESS)
        {
            map_free(&m);
            res.return_code = rc;
            return res;
        }
    }

    valid_tree = check_map_validity(m);
    if (!valid_tree)
    {
        map_free(&m);
        res.return_code = CS_FUNC;
        return res;
    }

    map_print(m);

    map_free(&m);
    if (m.root != NULL)
    {
        res.return_code = CS_MEM;
        return res;
    }

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("%s time: %f\n", test_name, time_taken);
    res.return_code = CS_SUCCESS;

    return res;
}

test_res test_map_insert_1000_elem()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_INSERT_1000_ELEM";
    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key, val, i;
    bool valid_tree;

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

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
        printf("WOW %d\n", rc);
        res.return_code = rc;
        return res;
    }

    for (i = 0; i < 1000; i++)
    {
        key = 2 * pow(i, 2) + 34 * pow(i, 1) + 71;
        val = 5;
        rc = map_insert(&m, &key, &val);
        if (rc != CS_SUCCESS)
        {
            printf("WTFFFFFF %d\n", rc);
            res.return_code = rc;
            map_free(&m);
            return res;
        }
    }

    valid_tree = check_map_validity(m);
    if (!valid_tree)
    {
        map_free(&m);
        res.return_code = CS_FUNC;
        return res;
    }

    map_free(&m);

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("%s time: %f\n", test_name, time_taken);
    res.return_code = CS_SUCCESS;

    return res;
}

test_res test_map_insert_fixup_case_1_1()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_INSERT_FIXUP_CASE_1_1";
    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key = 6, val = 9;
    bool valid_tree;

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

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

    key = 5;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    key = 3;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    key = 7;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    key = 4;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    valid_tree = check_map_validity(m);
    if (!valid_tree)
    {
        map_free(&m);
        res.return_code = CS_FUNC;
        return res;
    }

    map_print(m);

    map_free(&m);

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("%s time: %f\n", test_name, time_taken);
    res.return_code = CS_SUCCESS;

    return res;
}

test_res test_map_insert_fixup_case_1_2_1()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_INSERT_FIXUP_CASE_1_2_1";
    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key = 6, val = 9;
    bool valid_tree;

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

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

    key = 5;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    key = 3;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    key = 4;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    valid_tree = check_map_validity(m);
    if (!valid_tree)
    {
        map_free(&m);
        res.return_code = CS_FUNC;
        return res;
    }

    map_print(m);

    map_free(&m);

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("%s time: %f\n", test_name, time_taken);
    res.return_code = CS_SUCCESS;

    return res;
}

test_res test_map_insert_fixup_case_1_2_2()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_INSERT_FIXUP_CASE_1_2_2";
    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key = 6, val = 9;
    bool valid_tree;

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

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

    key = 5;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    key = 3;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    key = 2;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    valid_tree = check_map_validity(m);
    if (!valid_tree)
    {
        map_free(&m);
        res.return_code = CS_FUNC;
        return res;
    }

    map_print(m);

    map_free(&m);

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("%s time: %f\n", test_name, time_taken);
    res.return_code = CS_SUCCESS;

    return res;
}

test_res test_map_insert_fixup_case_2_1()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_INSERT_FIXUP_CASE_2_1";
    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key = 6, val = 9;
    bool valid_tree;

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

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

    key = 5;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    key = 3;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    key = 7;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    key = 6;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    valid_tree = check_map_validity(m);
    if (!valid_tree)
    {
        map_free(&m);
        res.return_code = CS_FUNC;
        return res;
    }

    map_print(m);

    map_free(&m);

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("%s time: %f\n", test_name, time_taken);
    res.return_code = CS_SUCCESS;

    return res;
}

test_res test_map_insert_fixup_case_2_2_1()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_INSERT_FIXUP_CASE_2_2_1";
    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key = 6, val = 9;
    bool valid_tree;

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

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

    key = 5;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    key = 7;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    key = 6;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    valid_tree = check_map_validity(m);
    if (!valid_tree)
    {
        map_free(&m);
        res.return_code = CS_FUNC;
        return res;
    }

    map_print(m);

    map_free(&m);

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("%s time: %f\n", test_name, time_taken);
    res.return_code = CS_SUCCESS;

    return res;
}

test_res test_map_insert_fixup_case_2_2_2()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_INSERT_FIXUP_CASE_2_2_2";
    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key = 6, val = 9;
    bool valid_tree;

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

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

    key = 5;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    key = 7;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    key = 8;
    val = 9;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_SUCCESS)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    valid_tree = check_map_validity(m);
    if (!valid_tree)
    {
        map_free(&m);
        res.return_code = CS_FUNC;
        return res;
    }

    map_print(m);

    map_free(&m);

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("%s time: %f\n", test_name, time_taken);
    res.return_code = CS_SUCCESS;

    return res;
}

test_res test_map_insert_same_key_twice()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_INSERT_SAME_KEY_TWICE";
    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key, val;
    bool valid_tree;

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

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

    key = 6;
    val = 69;
    rc = map_insert(&m, &key, &val);
    if (rc != CS_ELEM || m.root->left_child != NULL || m.root->right_child != NULL)
    {
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    valid_tree = check_map_validity(m);
    if (!valid_tree)
    {
        map_free(&m);
        res.return_code = CS_FUNC;
        return res;
    }

    map_print(m);

    map_free(&m);

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("%s time: %f\n", test_name, time_taken);
    res.return_code = CS_SUCCESS;

    return res;
}

test_res test_map_get()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_GET";
    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key = 6, val = 9, *read_val = malloc(sizeof(int));
    bool valid_tree;

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

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
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    valid_tree = check_map_validity(m);
    if (!valid_tree)
    {
        map_free(&m);
        res.return_code = CS_FUNC;
        return res;
    }

    rc = map_get(m, &key, read_val);
    if (rc != CS_SUCCESS || *read_val != val)
    {
        free(read_val);
        map_free(&m);
        res.return_code = CS_FUNC;
        return res;
    }
    printf("Value read: ");
    print_int(read_val);
    printf("\n");
    free(read_val);

    map_print(m);

    map_free(&m);

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("%s time: %f\n", test_name, time_taken);
    res.return_code = CS_SUCCESS;

    return res;
}

test_res test_map_get_wrong_key()
{
    clock_t start = clock();
    double time_taken;
    test_res res;
    char test_name[] = "MAP_GET_WRONG_KEY";
    map m;
    map_attr_t key_attr;
    map_attr_t val_attr;
    int rc, key = 6, val = 9, *read_val = malloc(sizeof(int));
    bool valid_tree;

    // Initialise test result
    res.test_name = malloc(sizeof(test_name) + 1);
    strcpy(res.test_name, test_name);

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
        map_free(&m);
        res.return_code = rc;
        return res;
    }

    valid_tree = check_map_validity(m);
    if (!valid_tree)
    {
        map_free(&m);
        res.return_code = CS_FUNC;
        return res;
    }

    key = 7;
    rc = map_get(m, &key, read_val);
    if (rc != CS_ELEM)
    {
        free(read_val);
        map_free(&m);
        res.return_code = CS_FUNC;
        return res;
    }
    free(read_val);

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
    test tests[] = {test_map_get_wrong_key};
    test_res res;

    for (i = 0; i < MAP_TEST_SIZE && i < (int)(sizeof(tests) / sizeof(test)); i++)
    {
        res = tests[i]();
        if (res.return_code != CS_SUCCESS)
        {
            printf("%s....................FAILED\n", res.test_name);
            free_test_res(&res);
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