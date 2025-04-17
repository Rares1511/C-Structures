#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/matrix.h"
#include "../include/unittest.h"

test_res test_matrix_init() {
    matrix_t m;
    matrix_attr_t attr = {sizeof(int), NULL, NULL, NULL, NULL};

    cs_codes rc = matrix_init(&m, 2, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_matrix_init", .reason = "matrix_init failed", .return_code = rc};
    }

    matrix_free(&m);
    return (test_res){.test_name = "test_matrix_init", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_matrix_init_neg_size() {
    matrix_t m;
    matrix_attr_t attr = {-1, NULL, NULL, NULL, NULL};

    cs_codes rc = matrix_init(&m, 2, attr);
    if (rc != CS_SIZE) {
        return (test_res){.test_name = "test_matrix_init_neg_size",
                          .reason = "matrix_init failed",
                          .return_code = rc};
    }

    return (test_res){
        .test_name = "test_matrix_init_neg_size", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_matrix_init_max_size() {
    matrix_t m;
    matrix_attr_t attr = {SIZE_TH + 1, NULL, NULL, NULL, NULL};

    cs_codes rc = matrix_init(&m, 2, attr);
    if (rc != CS_SIZE) {
        return (test_res){.test_name = "test_matrix_init_max_size",
                          .reason = "matrix_init failed",
                          .return_code = rc};
    }

    return (test_res){
        .test_name = "test_matrix_init_max_size", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_matrix_init_neg_dim() {
    matrix_t m;
    matrix_attr_t attr = {sizeof(int), NULL, NULL, NULL, NULL};

    cs_codes rc = matrix_init(&m, -1, attr);
    if (rc != CS_SIZE) {
        return (test_res){.test_name = "test_matrix_init_neg_dim",
                          .reason = "matrix_init failed",
                          .return_code = rc};
    }

    return (test_res){
        .test_name = "test_matrix_init_neg_dim", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_matrix_init_max_dim() {
    matrix_t m;
    matrix_attr_t attr = {sizeof(int), NULL, NULL, NULL, NULL};

    cs_codes rc = matrix_init(&m, MAX_MATRIX_DIM + 1, attr);
    if (rc != CS_SIZE) {
        return (test_res){.test_name = "test_matrix_init_max_dim",
                          .reason = "matrix_init failed",
                          .return_code = rc};
    }

    return (test_res){
        .test_name = "test_matrix_init_max_dim", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_matrix_push_back_1() {
    matrix_t m;
    matrix_attr_t attr = {sizeof(int), NULL, print_int, NULL, NULL};
    int OUTPUT_BUFFER_SIZE = 1024, dimensions, num_elements;
    char input_file_name[] = "matrix/matrix_tests/input/test_matrix_push_back_1.in";
    char output_file_name[] = "matrix/matrix_tests/output/test_matrix_push_back_1.out";
    char correct_output[OUTPUT_BUFFER_SIZE], buffer[OUTPUT_BUFFER_SIZE];
    FILE *input_file = fopen(input_file_name, "r");
    FILE *output_file = fopen(output_file_name, "r");

    if (input_file == NULL) {
        return (test_res){.test_name = "test_matrix_push_back_1",
                          .reason = "input file not found",
                          .return_code = CS_FILE};
    }

    if (output_file == NULL) {
        fclose(input_file);
        return (test_res){.test_name = "test_matrix_push_back_1",
                          .reason = "output file not found",
                          .return_code = CS_FILE};
    }

    correct_output[0] = '\0';
    while (fgets(buffer, OUTPUT_BUFFER_SIZE, output_file)) {
        strcat(correct_output, buffer);
    }

    fscanf(input_file, "%d %d", &dimensions, &num_elements);
    int position[dimensions - 1];

    cs_codes rc = matrix_init(&m, dimensions, attr);
    if (rc != CS_SUCCESS) {
        fclose(input_file);
        fclose(output_file);
        return (test_res){.test_name = "test_matrix_push_back_1",
                          .reason = "matrix_init failed",
                          .return_code = rc};
    }

    for (int i = 0; i < num_elements; i++) {
        for (int j = 0; j < dimensions - 1; j++) {
            fscanf(input_file, "%d", &position[j]);
        }
        int el;
        fscanf(input_file, "%d", &el);
        rc = matrix_push_back(&m, position, &el);
        if (rc != CS_SUCCESS) {
            matrix_free(&m);
            fclose(input_file);
            fclose(output_file);
            return (test_res){.test_name = "test_matrix_push_back_1",
                              .reason = "matrix_push_back failed",
                              .return_code = rc};
        }
    }

    int stdout_bk; // is fd for stdout backup
    stdout_bk = dup(fileno(stdout));

    int pipefd[2];
    pipe2(pipefd, 0); // O_NONBLOCK);

    // What used to be stdout will now go to the pipe.
    dup2(pipefd[1], fileno(stdout));

    matrix_print(&m);
    fflush(stdout);          // flushall();
    write(pipefd[1], "", 1); // null-terminated string!
    close(pipefd[1]);

    dup2(stdout_bk, fileno(stdout)); // restore

    char buf[OUTPUT_BUFFER_SIZE];
    read(pipefd[0], buf, OUTPUT_BUFFER_SIZE - 1);
    printf("%s", buf);

    if (strcmp(buf, correct_output) != 0) {
        matrix_free(&m);
        fclose(input_file);
        fclose(output_file);
        return (test_res){.test_name = "test_matrix_push_back_1",
                          .reason = "elements were pushed back but not correctly failed",
                          .return_code = CS_UNKNOWN};
    }

    matrix_free(&m);
    if (m.vec != NULL) {
        return (test_res){.test_name = "test_matrix_push_back_1",
                          .reason = "matrix_free failed",
                          .return_code = CS_MEM};
    }

    fclose(input_file);
    fclose(output_file);
    return (test_res){
        .test_name = "test_matrix_push_back_1", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_matrix_push_back_2() {
    matrix_t m;
    matrix_attr_t attr = {sizeof(int), NULL, print_int, NULL, NULL};
    int OUTPUT_BUFFER_SIZE = 1024, dimensions, num_elements;
    char input_file_name[] = "matrix/matrix_tests/input/test_matrix_push_back_2.in";
    char output_file_name[] = "matrix/matrix_tests/output/test_matrix_push_back_2.out";
    char correct_output[OUTPUT_BUFFER_SIZE], buffer[OUTPUT_BUFFER_SIZE];
    FILE *input_file = fopen(input_file_name, "r");
    FILE *output_file = fopen(output_file_name, "r");

    if (input_file == NULL) {
        return (test_res){.test_name = "test_matrix_push_back_2",
                          .reason = "input file not found",
                          .return_code = CS_FILE};
    }

    if (output_file == NULL) {
        fclose(input_file);
        return (test_res){.test_name = "test_matrix_push_back_2",
                          .reason = "output file not found",
                          .return_code = CS_FILE};
    }

    correct_output[0] = '\0';
    while (fgets(buffer, OUTPUT_BUFFER_SIZE, output_file)) {
        strcat(correct_output, buffer);
    }

    fscanf(input_file, "%d %d", &dimensions, &num_elements);
    int position[dimensions - 1];

    cs_codes rc = matrix_init(&m, dimensions, attr);
    if (rc != CS_SUCCESS) {
        fclose(input_file);
        fclose(output_file);
        return (test_res){.test_name = "test_matrix_push_back_2",
                          .reason = "matrix_init failed",
                          .return_code = rc};
    }

    for (int i = 0; i < num_elements; i++) {
        for (int j = 0; j < dimensions - 1; j++) {
            fscanf(input_file, "%d", &position[j]);
        }
        int el;
        fscanf(input_file, "%d", &el);
        rc = matrix_push_back(&m, position, &el);
        if (rc != CS_SUCCESS) {
            matrix_free(&m);
            fclose(input_file);
            fclose(output_file);
            return (test_res){.test_name = "test_matrix_push_back_2",
                              .reason = "matrix_push_back failed",
                              .return_code = rc};
        }
    }

    int stdout_bk; // is fd for stdout backup
    stdout_bk = dup(fileno(stdout));

    int pipefd[2];
    pipe2(pipefd, 0); // O_NONBLOCK);

    // What used to be stdout will now go to the pipe.
    dup2(pipefd[1], fileno(stdout));

    matrix_print(&m);
    fflush(stdout);          // flushall();
    write(pipefd[1], "", 1); // null-terminated string!
    close(pipefd[1]);

    dup2(stdout_bk, fileno(stdout)); // restore

    char buf[OUTPUT_BUFFER_SIZE];
    read(pipefd[0], buf, OUTPUT_BUFFER_SIZE - 1);
    printf("%s", buf);

    if (strcmp(buf, correct_output) != 0) {
        matrix_free(&m);
        fclose(input_file);
        fclose(output_file);
        return (test_res){.test_name = "test_matrix_push_back_1",
                          .reason = "elements were pushed back but not correctly failed",
                          .return_code = CS_UNKNOWN};
    }

    matrix_free(&m);
    if (m.vec != NULL) {
        return (test_res){.test_name = "test_matrix_push_back_1",
                          .reason = "matrix_free failed",
                          .return_code = CS_MEM};
    }

    fclose(input_file);
    fclose(output_file);
    return (test_res){
        .test_name = "test_matrix_push_back_1", .reason = "none", .return_code = CS_SUCCESS};
}

int main() {
    int i;
    test tests[] = {
        test_matrix_init,         test_matrix_init_neg_size, test_matrix_init_max_size,
        test_matrix_init_neg_dim, test_matrix_init_max_dim,  test_matrix_push_back_1,
        test_matrix_push_back_2,
    };
    test_res res;

    for (i = 0; i < MATRIX_TEST_SIZE && i < (int)(sizeof(tests) / sizeof(test)); i++) {
        res = tests[i]();
        char buffer[1024];
        strcpy(buffer, res.test_name);
        strncat(buffer,
                "..........................................................................",
                MAX_PRINT_SIZE - strlen(res.test_name));
        if (res.return_code != CS_SUCCESS) {
            printf("%sFAILED: %s\n", buffer, res.reason);
            exit(-(int)(sizeof(tests) / sizeof(test) - i));
        } else {
            printf("%sSUCCESS: %d/%d\n", buffer, i + 1, (int)(sizeof(tests) / sizeof(test)));
        }
    }
    return 0;
}