#include <cs/multiset.h>
#include <cs/pair.h>
#include <cs/rbt.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


/*!
 * Compares two multiset nodes (pairs) based on their first element.
 * @param[in] a  Pointer to the first multiset node (pair)
 * @param[in] b  Pointer to the second multiset node (pair)
 * @return        Negative value if a < b, positive if a > b, zero if equal
 */
int multiset_node_comp(const void *a, const void *b) {
    const pair *pa = (pair*) a;
    const pair *pb = (pair*) b;
    if (pa->first_attr->comp != NULL) {
        return pa->first_attr->comp(pa->first, pb->first);
    }
    return memcmp(pa->first, pb->first, pa->first_attr->size);
}

/*!
 * Copies a multiset node (pair) from src to dest.
 * @param[out] dest  Pointer to the destination multiset node (pair)
 * @param[in]  src   Pointer to the source multiset node (pair)
 */
void multiset_node_copy(void *dest, const void *src) {
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

/*!
 * Prints the elements of a multiset node (pair) to the given stream.
 * @param[in] stream  The output stream
 * @param[in] node    Pointer to the multiset node (pair)
 */
void multiset_node_print(FILE *stream, const void *node) {
    CS_RETURN_IF(node == NULL || stream == NULL);
    const pair *p = (const pair *)node;
    CS_RETURN_IF(p->first == NULL || p->second == NULL);
    int count = *((int *)(p->second));
    CS_RETURN_IF(p->first_attr->print == NULL);
    for (int i = 0; i < count; i++) {
        p->first_attr->print(stream, p->first);
        if (i < count - 1) {
            fprintf(stream, ", ");
        }
    }
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

multiset *multiset_init(multiset_attr_t attr) {
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, NULL);
    multiset *ms = (multiset *)malloc(sizeof(multiset));
    CS_RETURN_IF(ms == NULL, NULL);

    rbt_attr_t rbt_attr = {
        .comp = multiset_node_comp,
        .copy = multiset_node_copy,
        .fr = pair_free,
        .print = multiset_node_print,
        .size = sizeof(pair)
    };

    multiset_attr_t count_attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = NULL,
        .comp = NULL
    };

    ms->t = rbt_init(rbt_attr);
    CS_RETURN_IF(ms->t == NULL, NULL);

    ms->el_attr = malloc(sizeof(multiset_attr_t));
    ms->count_attr = malloc(sizeof(multiset_attr_t));
    CS_RETURN_IF(NULL == ms->el_attr || NULL == ms->count_attr, NULL);

    memcpy(ms->el_attr, &attr, sizeof(multiset_attr_t));
    memcpy(ms->count_attr, &count_attr, sizeof(multiset_attr_t));
    return ms;
}

cs_codes multiset_insert(multiset *ms, const void *elem) {
    CS_RETURN_IF(ms == NULL || elem == NULL, CS_NULL);
    pair data;
    int inital_count = 1;
    pair_init(&data, ms->el_attr, ms->count_attr);
    pair_set(&data, elem, &inital_count);

    void *node = rbt_find(*(ms->t), &data);
    if (node != NULL) {
        pair_free(&data);
        pair *p = (pair *)node;
        int *count = (int *)(p->second);
        (*count)++;
        return CS_SUCCESS;
    }
    else {
        return rbt_insert(ms->t, &data);
    }
}

cs_codes multiset_delete(multiset *ms, const void *elem) {
    CS_RETURN_IF(ms == NULL || elem == NULL, CS_NULL);
    pair data;
    int rc;
    pair_init(&data, ms->el_attr, ms->count_attr);
    pair_set(&data, elem, NULL);

    void *node = rbt_find(*(ms->t), &data);
    if (node != NULL) {
        pair *p = (pair *)node;
        int *count = (int *)(p->second);
        if (*count > 1) {
            (*count)--;
            rc = CS_SUCCESS;
        } else {
            rc = rbt_delete(ms->t, &data);
        }
    } else {
        rc = CS_ELEM;
    }
    pair_free(&data);
    return rc;
}

int multiset_count(multiset *ms, const void *elem) {
    CS_RETURN_IF(ms == NULL || elem == NULL, 0);
    pair data;
    pair_init(&data, ms->el_attr, ms->count_attr);
    pair_set(&data, elem, NULL);

    void *node = rbt_find(*(ms->t), &data);
    pair_free(&data);
    if (node != NULL) {
        pair *p = (pair *)node;
        return *((int *)(p->second));
    } else {
        return 0;
    }
}

void multiset_clear(multiset *ms) {
    CS_RETURN_IF(ms == NULL);
    rbt_clear(ms->t);
}

void multiset_print(FILE *stream, void *v_ms) {
    CS_RETURN_IF(v_ms == NULL || stream == NULL);
    multiset *ms = (multiset *)v_ms;
    rbt_print(stream, ms->t);
}

void multiset_free(void *v_ms) {
    CS_RETURN_IF(v_ms == NULL);
    multiset *ms = (multiset *)v_ms;
    rbt_free(ms->t);
    free(ms->el_attr);
    free(ms->count_attr);
    free(ms);
}