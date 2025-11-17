#include <cs/cs_global.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

cs_global_t *cs_global = NULL;

void cs_global_destroy() {
    printf("Cleaning up\n");
    if (cs_global) {
        // Unmap the shared memory object
        if (munmap(cs_global, sizeof(cs_global_t)) == -1) {
            perror("munmap");
        }
    }
    pthread_mutex_destroy(&cs_global->lock);
}

void cs_global_destroy_handler(int signal_id) {
    printf("Received signal %d\n", signal_id);
    cs_global_destroy();
}

cs_codes cs_global_init() {
    char already_exists = 0;

    // Try to open the shared memory without O_CREAT to check if it exists
    int fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0666);
    if (fd == -1) {
        if (errno == ENOENT) {
            // Shared memory does not exist, create it
            fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
            if (fd == -1) {
                perror("shm_open");
                return CS_FILE;
            }
        } else {
            // Some other error occurred
            perror("shm_open");
            return CS_FILE;
        }
    } else {
        already_exists = 1; // Shared memory already exists
    }

    // Resize the shared memory object if it was just created
    if (!already_exists) {
        if (ftruncate(fd, sizeof(cs_global_t)) == -1) {
            perror("ftruncate");
            close(fd); // Cleanup file descriptor
            return CS_MEM;
        }
    }

    // Map the shared memory object into the process's address space
    cs_global = mmap(NULL, sizeof(cs_global_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (cs_global == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return CS_MEM;
    }

    close(fd);

    // Check the validity marker
    if (!already_exists || cs_global->is_initialized != 1) {
        // Shared memory is uninitialized; initialize it
        printf("Shared memory object created and initialized.\n");
        memset(cs_global, 0, sizeof(cs_global_t)); // Zero out the shared memory
        cs_global->is_initialized = 1;             // Mark it as initialized
        if (pthread_mutex_init(&cs_global->lock, NULL) != 0) {
            perror("pthread_mutex_init");
            return CS_UNKNOWN;
        }
        cs_global->size = 0;
        cs_global->cap = INIT_CAPACITY;
        cs_global->entries = malloc(cs_global->cap * sizeof(cs_global_entry_t));
        if (!cs_global->entries) {
            perror("malloc");
            return CS_MEM;
        }
    } else {
        printf("Shared memory object already contains valid data.\n");
    }

    atexit(cs_global_destroy);
    for (int signal_id = 0; signal_id < 32; signal_id++) {
        signal(signal_id, cs_global_destroy);
    }

    return CS_SUCCESS;
}

int cs_global_size() {
    if (!cs_global || cs_global->is_initialized != 1) {
        return 0;
    }

    return cs_global->size;
}

cs_codes cs_global_add_entry(cs_data_structure data_structure, void *data) {
    cs_codes rc;

    if (!cs_global || cs_global->is_initialized != 1) {
        rc = cs_global_init();
        if (rc != CS_SUCCESS) {
            return rc;
        }
    }

    printf("Adding entry to shared memory object.\n");

    // Lock the shared memory object
    if (pthread_mutex_lock(&cs_global->lock) != 0) {
        perror("pthread_mutex_lock");
        return CS_UNKNOWN;
    }

    printf("Current size: %d, capacity: %d\n", cs_global->size, cs_global->cap);

    // Resize the entries array
    if (cs_global->size == cs_global->cap) {
        cs_global->cap = cs_global->cap == 0 ? 1 : cs_global->cap * 2;
        cs_global->entries =
            realloc(cs_global->entries, cs_global->cap * sizeof(cs_global_entry_t));
        if (!cs_global->entries) {
            perror("realloc");
            return CS_MEM;
        }
    }

    printf("Resized entries array to capacity: %d\n", cs_global->cap);

    // Add the new entry
    cs_global->entries[cs_global->size].data_structure = data_structure;
    cs_global->entries[cs_global->size].pid = getpid();
    cs_global->entries[cs_global->size].data = data;
    cs_global->size++;

    printf("Added entry. New size: %d\n", cs_global->size);

    // Unlock the shared memory object
    if (pthread_mutex_unlock(&cs_global->lock) != 0) {
        perror("pthread_mutex_unlock");
        return CS_UNKNOWN;
    }

    return CS_SUCCESS;
}