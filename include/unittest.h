#define MAX_REASON_SIZE 1024
#define MAX_PRINT_SIZE 75

#define MAP_TEST_SIZE 20
#define DEQUE_TEST_SIZE 20

typedef struct test_res
{
    char *test_name;
    char *reason;
    int return_code;
} test_res;

typedef struct test_res (*test)();

int test_res_init(test_res *res, char test_name[]);
void test_res_free(void *v_res);