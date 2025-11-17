#ifndef CS_GLOBAL_H
#define CS_GLOBAL_H

#include <pthread.h>
#include <cs/universal.h>

#define SHARED_MEMORY_NAME "/cs_global"

/************************************* DATA STRUCTURES *************************************/

typedef enum {
    CS_DEQUE,
    CS_HASH_TABLE,
    CS_HEAP,
    CS_LARGE_NUMBER,
    CS_LIST,
    CS_MAP,
    CS_MATRIX,
    CS_PAIR,
    CS_QUEUE,
    CS_STACK,
    CS_STRING,
    CS_VECTOR
} cs_data_structure;

typedef struct cs_global_entry_t {
    cs_data_structure data_structure;
    int pid;
    void *data;
} cs_global_entry_t;

typedef struct cs_global_t {
    pthread_mutex_t lock;
    cs_global_entry_t *entries;
    int size;
    int cap;
    int is_initialized;
} cs_global_t;

/************************************* FUNCTIONS *************************************/

/*!
 * Adds a new entry to the global shared memory object
 * @param[in] data_structure - the data structure of the entry
 * @param[in] data - the pointer to the data of the entry
 * @return CS_SUCCESS if the entry was added successfully, some error code otherwise such as
 * CS_MEM - if there was an error allocating memory for the new entry
 */
cs_codes cs_global_add_entry(cs_data_structure data_structure, void *data);

int cs_global_size();

#endif // CS_GLOBAL_H