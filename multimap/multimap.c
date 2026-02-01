#include <cs/multimap.h>
#include <cs/rbt.h>
#include <cs/pair.h>
#include <cs/vector.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


/*!
 * Compare function for multimap nodes (pairs).
 * @param[in] a  First pair to compare
 * @param[in] b  Second pair to compare
 * @return    Negative if a < b, zero if a == b, positive if a > b
 */
int multimap_node_comp(const void *a, const void *b) {
    const pair *pa = (const pair *)a;
    const pair *pb = (const pair *)b;

    if (pa->first_attr->comp) {
        return pa->first_attr->comp(pa->first, pb->first);
    } else {
        return memcmp(pa->first, pb->first, pa->first_attr->size);
    }
}

/*!
 * Copy function for multimap nodes (pairs).
 * @param[out] dest  Destination pair to copy into
 * @param[in]  src   Source pair to copy from
 */
void multimap_node_copy(void *dest, const void *src) {
    pair* d = (pair*)dest;
    const pair* s = (const pair*)src;
    if (d == NULL || s == NULL) {
        return;
    }
    d->first_attr = s->first_attr;
    d->second_attr = s->second_attr;
    d->first = s->first;
    d->second = s->second;
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

cs_codes multimap_init(multimap *mm, multimap_attr_t key_attr,
                           multimap_attr_t value_attr) {
    CS_RETURN_IF(NULL == mm, CS_NULL);
    CS_RETURN_IF(key_attr.size <= 0 || key_attr.size > SIZE_TH, CS_SIZE);
    CS_RETURN_IF(value_attr.size <= 0 || value_attr.size > SIZE_TH, CS_SIZE);

    rbt_attr_t rbt_attr = {
        .size = sizeof(pair),
        .fr = pair_free,
        .print = pair_print,
        .comp = multimap_node_comp,
        .copy = multimap_node_copy,
    };

    multimap_attr_t vec_attr = {
        .size = sizeof(vector),
        .fr = vector_free,
        .print = vector_print,
        .comp = NULL,
        .copy = NULL
    };

    mm->key_attr = malloc(sizeof(multimap_attr_t));
    mm->value_attr = malloc(sizeof(multimap_attr_t));
    mm->vec_attr = malloc(sizeof(multimap_attr_t));
    CS_RETURN_IF(NULL == mm->key_attr || NULL == mm->value_attr || NULL == mm->vec_attr, CS_MEM);
    memcpy(mm->key_attr, &key_attr, sizeof(multimap_attr_t));
    memcpy(mm->value_attr, &value_attr, sizeof(multimap_attr_t));
    memcpy(mm->vec_attr, &vec_attr, sizeof(multimap_attr_t));

    mm->t = malloc(sizeof(rbt));
    CS_RETURN_IF(NULL == mm->t, CS_MEM);
    return rbt_init(mm->t, rbt_attr);
}

cs_codes multimap_insert(multimap *mm, const void *key, const void *value) {
    CS_RETURN_IF(mm == NULL || key == NULL || value == NULL, CS_NULL);
    pair data;
    int rc;

    pair_init(&data, mm->key_attr, mm->vec_attr);
    pair_set(&data, key, NULL);

    pair *p = (pair *)rbt_find(*(mm->t), &data);
    if (p != NULL) {
        pair_free(&data);
        vector *vec = (vector *)p->second;
        return vector_push_back(vec, value);
    }
    else {
        vector vec;
        rc = vector_init(&vec, *(mm->value_attr));
        if (CS_SUCCESS != rc) {
            pair_free(&data);
            return rc;
        }
        vector_push_back(&vec, value);
        pair_set(&data, NULL, &vec);
        return rbt_insert(mm->t, &data);
    }
}

cs_codes multimap_delete(multimap *mm, const void *key) {
    CS_RETURN_IF(mm == NULL || key == NULL, CS_NULL);
    pair data;
    int rc;
    pair_init(&data, mm->key_attr, mm->vec_attr);
    pair_set(&data, key, NULL);

    pair *p = (pair *)rbt_find(*(mm->t), &data);
    if (p != NULL) {
        vector *vec = (vector *)p->second;
        int size = vector_size(*vec);
        if (size == 1) {
            rc = rbt_delete(mm->t, &data);
        } 
        else {
            rc = vector_pop_back(vec);
        }
    } else {
        rc = CS_ELEM;
    }
    pair_free(&data);
    return rc;
}

vector* multimap_get(multimap *mm, const void *key) {
    CS_RETURN_IF(NULL == mm || NULL == key, NULL);
    pair data;
    pair_init(&data, mm->key_attr, mm->vec_attr);
    pair_set(&data, key, NULL);
    
    pair *p = (pair *)rbt_find(*(mm->t), &data);
    pair_free(&data);
        
    if (p != NULL) {
        return (vector *)p->second;
    }
    return NULL;
}

void multimap_clear(multimap *mm) {
    CS_RETURN_IF(mm == NULL);
    rbt_clear(mm->t);
}

void multimap_print(FILE *stream, void *v_mm) {
    CS_RETURN_IF(v_mm == NULL);
    multimap *mm = (multimap *)v_mm;
    rbt_print(stream, mm->t);
}

void multimap_free(void *v_mm) {
    CS_RETURN_IF(v_mm == NULL);
    multimap *mm = (multimap *)v_mm;
    rbt_free(mm->t);
    free(mm->key_attr);
    free(mm->value_attr);
    free(mm->vec_attr);
    free(mm->t);
}