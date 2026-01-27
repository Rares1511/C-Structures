#include <cs/flat_set.h>
#include <cs/deque.h>
#include <cs/vector.h>

#include <stdlib.h>
#include <string.h>

flat_set* flat_set_init(flat_set_attr_t attr, flat_set_type type) {
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, NULL);
    flat_set *fs = (flat_set *)malloc(sizeof(flat_set));
    CS_RETURN_IF(NULL == fs, NULL);

    fs->type = type;
    fs->container = NULL;
    switch (type) {
        case CS_FLAT_SET_DEQUE:
            fs->container = deque_init(attr);
            break;
        case CS_FLAT_SET_VECTOR:
            fs->container = vector_init(attr);
            break;
        default:
            break;
    }
    if (NULL == fs->container) {
        free(fs);
        return NULL;
    }
    return fs;
}

cs_codes flat_set_insert(flat_set *fs, void *data) {
    CS_RETURN_IF(NULL == fs || NULL == data, CS_NULL);
    comparer comp;
    int size = 0;
    switch (fs->type) {
        case CS_FLAT_SET_DEQUE:
            comp = ((deque*)fs->container)->attr.comp;
            size = ((deque*)fs->container)->attr.size;
            break;
        case CS_FLAT_SET_VECTOR:
            comp = ((vector*)fs->container)->attr.comp;
            size = ((vector*)fs->container)->attr.size;
            break;
        default:
            return CS_FUNC;
    }

    int left = 0, right = flat_set_size(*fs) - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        void *mid_data = flat_set_at(*fs, mid);
        int cmp_res = comp ? comp(data, mid_data) : memcmp(data, mid_data, size);
        if (cmp_res == 0) {
            return CS_ELEM;
        } else if (cmp_res < 0) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    switch (fs->type) {
        case CS_FLAT_SET_DEQUE:
            return deque_insert_at(fs->container, data, left);
        case CS_FLAT_SET_VECTOR:
            return vector_insert_at(fs->container, data, left);
        default:
            return CS_FUNC;
    }
}

cs_codes flat_set_erase(flat_set *fs, void *data) {
    CS_RETURN_IF(NULL == fs || NULL == data, CS_NULL);
    int index = flat_set_find(*fs, data);
    CS_RETURN_IF(index == -1, CS_ELEM);

    switch (fs->type) {
        case CS_FLAT_SET_DEQUE:
            return deque_erase(fs->container, index);
        case CS_FLAT_SET_VECTOR:
            return vector_erase(fs->container, index);
        default:
            return CS_FUNC;
    }
}

void* flat_set_at(flat_set fs, int index) {
    CS_RETURN_IF(NULL == fs.container, NULL);
    switch (fs.type) {
        case CS_FLAT_SET_DEQUE:
            return deque_at(*((deque*)fs.container), index);
        case CS_FLAT_SET_VECTOR:
            return vector_at(*((vector*)fs.container), index);
        default:
            return NULL;
    }
}

int flat_set_find(flat_set fs, void *data) {
    CS_RETURN_IF(NULL == fs.container || NULL == data, -1);
    comparer comp;
    int size = 0;
    switch (fs.type) {
        case CS_FLAT_SET_DEQUE:
            comp = ((deque*)fs.container)->attr.comp;
            size = ((deque*)fs.container)->attr.size;
            break;
        case CS_FLAT_SET_VECTOR:
            comp = ((vector*)fs.container)->attr.comp;
            size = ((vector*)fs.container)->attr.size;
            break;
        default:
            return -1;
    }

    int left = 0, right = flat_set_size(fs) - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        void *mid_data = flat_set_at(fs, mid);
        int cmp_res = comp ? comp(data, mid_data) : memcmp(data, mid_data, size);
        if (cmp_res == 0) {
            return mid;
        } else if (cmp_res < 0) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return -1;
}

int flat_set_empty(flat_set fs) {
    CS_RETURN_IF(NULL == fs.container, 1);
    switch (fs.type) {
        case CS_FLAT_SET_DEQUE:
            return deque_empty(*((deque*)fs.container));
        case CS_FLAT_SET_VECTOR:
            return vector_empty(*((vector*)fs.container));
        default:
            return 1;
    }
}

int flat_set_size(flat_set fs) {
    CS_RETURN_IF(NULL == fs.container, 0);
    switch (fs.type) {
        case CS_FLAT_SET_DEQUE:
            return deque_size(*((deque*)fs.container));
        case CS_FLAT_SET_VECTOR:
            return vector_size(*((vector*)fs.container));
        default:
            return 0;
    }
}

void flat_set_clear(flat_set *fs) {
    CS_RETURN_IF(NULL == fs || NULL == fs->container);
    switch (fs->type) {
        case CS_FLAT_SET_DEQUE:
            deque_clear(fs->container);
            break;
        case CS_FLAT_SET_VECTOR:
            vector_clear(fs->container);
            break;
        default:
            break;
    }
}

void flat_set_swap(flat_set *fs1, flat_set *fs2) {
    CS_RETURN_IF(NULL == fs1 || NULL == fs2);
    
    void *temp_container = fs1->container;
    flat_set_type temp_type = fs1->type;

    fs1->container = fs2->container;
    fs1->type = fs2->type;

    fs2->container = temp_container;
    fs2->type = temp_type;
}

void flat_set_print(FILE *stream, void *v_fs) {
    CS_RETURN_IF(NULL == v_fs);
    flat_set *fs = (flat_set *)v_fs;
    switch (fs->type) {
        case CS_FLAT_SET_DEQUE:
            deque_print(stream, fs->container);
            break;
        case CS_FLAT_SET_VECTOR:
            vector_print(stream, fs->container);
            break;
        default:
            break;
    }
}

void flat_set_free(void *v_fs) {
    CS_RETURN_IF(NULL == v_fs);
    flat_set *fs = (flat_set *)v_fs;
    switch (fs->type) {
        case CS_FLAT_SET_DEQUE:
            deque_free(fs->container);
            break;
        case CS_FLAT_SET_VECTOR:
            vector_free(fs->container);
            break;
        default:
            break;
    }
    free(fs);
}