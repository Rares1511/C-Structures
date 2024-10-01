#define MAP_TEST_SIZE 10

typedef struct test_res
{
    char *test_name;
    int return_code;
} test_res;

typedef struct test_res (*test)();

void free_test_res(void *v_res);