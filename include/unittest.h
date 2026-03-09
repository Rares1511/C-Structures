#pragma once
#define __MAX_REASON_SIZE 1024
#define __MAX_PRINT_SIZE 75

#define __TEST_SIZE 1000
#define __VALUE_RANGE 10000
#define __STRESS_TEST_SIZE 10000000

#define __UNITTEST_DEBUG_FILE_ARG_NAME "--debug-file"
#define __UNITTEST_DEBUG_FILE_DEFAULT_VALUE "logs/unittest_log.ansi"

#define __UNITTEST_RESULTS_FILE_NAME "--results-file"
#define __UNITTEST_RESULTS_FILE_NAME_VALUE "logs/unittest_results.ansi"

#define __UNITTEST_SEED_ARG_NAME "--seed"
#define __UNITTEST_SEED_DEFAULT_VALUE 42

#define __UNITTEST_MODULE_ARG_NAME "--module"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cs/universal.h>
#include <cs/cargs.h>
#include <cs/clogger.h>
#include <cs/rbt.h>

#define __GREEN_UNITTEST  "\033[32m"
#define __RED_UNITTEST    "\033[31m"
#define __RESET_UNITTEST  "\033[0m"
#define __BOLD_UNITTEST   "\033[1m"

#pragma region RBT VALIDATION HELPERS
// ============================================================================
// RBT VALIDATION HELPERS
// Used by set, map, multiset, multimap unittests to verify RBT integrity
// ============================================================================

static inline int rbt_check_black_height(rbt_node *node) {
    if (node == NULL) {
        return 1;
    }

    int left_bh  = rbt_check_black_height(node->left);
    int right_bh = rbt_check_black_height(node->right);

    if (left_bh == -1 || right_bh == -1)
        return -1;

    if (left_bh != right_bh)
        return -1;

    if (node->color == __RBT_NODE_RED_COLOR) {
        if ((node->left && node->left->color == __RBT_NODE_RED_COLOR) ||
            (node->right && node->right->color == __RBT_NODE_RED_COLOR)) {
            return -1;
        }
    }

    return left_bh + (node->color == __RBT_NODE_BLACK_COLOR ? 1 : 0);
}

static inline int rbt_check_bst(rbt_node *node, rbt_attr_t attr, void *min_key, void *max_key) {
    if (!node) return 1;

    if (min_key) {
        if (attr.comp && attr.comp(node->data, min_key) <= 0)
            return 0;
    }

    if (max_key) {
        if (attr.comp && attr.comp(node->data, max_key) >= 0)
            return 0;
    }

    if (!rbt_check_bst(node->left, attr, min_key, node->data))
        return 0;

    if (!rbt_check_bst(node->right, attr, node->data, max_key))
        return 0;

    return 1;
}

static inline int rbt_is_valid(rbt *t) {
    if (t->root && t->root->color != __RBT_NODE_BLACK_COLOR)
        return 0;

    if (!rbt_check_bst(t->root, t->attr, NULL, NULL))
        return 0;

    int bh = rbt_check_black_height(t->root);
    if (bh == -1)
        return 0;

    return 1;
}
#pragma endregion

#pragma region Hash Functions
// Simple hash function for integers
size_t hash_int(const void *key) {
    return *(int *)key;
}

// Simple hash function for strings
size_t hash_string(const void *key) {
    const char *str = *(const char **)key;
    size_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}
#pragma endregion

typedef struct test_res {
    char *test_name;
    char *reason;
    int return_code;
} test_res;

typedef struct test_arg {
    clogger *logger;
} test_arg;

typedef struct test_res (*test)(test_arg *arg);

// ============================================================================
// COMPLEX TEST STRUCT
// A complex struct with multiple dynamic allocations to stress-test
// data structure implementations with proper copy/free semantics
// ============================================================================

// Nested struct representing an address
typedef struct test_address {
    char *street;           // Dynamically allocated string
    char *city;             // Dynamically allocated string
    int zip_code;
} test_address;

// Nested struct representing contact info
typedef struct test_contact {
    char *email;            // Dynamically allocated string
    char *phone;            // Dynamically allocated string
    int priority;           // For sorting contacts
} test_contact;

// Main complex test struct with nested structs and dynamic arrays
typedef struct test_struct {
    int id;                     // Primary key for comparison
    char *name;                 // Dynamically allocated string
    double score;               // Floating point data
    test_address *address;      // Pointer to nested struct (dynamically allocated)
    test_contact *contacts;     // Dynamic array of contacts
    int contact_count;          // Number of contacts in array
    int *tags;                  // Dynamic array of integers
    int tag_count;              // Number of tags
    unsigned char flags;        // Bit flags for boolean properties
} test_struct;

// Flag definitions for test_struct.flags
#define TEST_FLAG_ACTIVE    0x01
#define TEST_FLAG_VERIFIED  0x02
#define TEST_FLAG_PREMIUM   0x04
#define TEST_FLAG_ADMIN     0x08

// Helper to create a test_address
static inline test_address *create_test_address(const char *street, const char *city, int zip) {
    test_address *addr = (test_address *)malloc(sizeof(test_address));
    if (!addr) return NULL;
    
    addr->street = street ? strdup(street) : NULL;
    addr->city = city ? strdup(city) : NULL;
    addr->zip_code = zip;
    return addr;
}

// Helper to free a test_address
static inline void free_test_address(test_address *addr) {
    if (!addr) return;
    if (addr->street) free(addr->street);
    if (addr->city) free(addr->city);
    free(addr);
}

// Helper to deep copy a test_address
static inline test_address *copy_test_address(const test_address *src) {
    if (!src) return NULL;
    return create_test_address(src->street, src->city, src->zip_code);
}

// Helper to create a test_contact
static inline test_contact create_test_contact(const char *email, const char *phone, int priority) {
    test_contact c;
    c.email = email ? strdup(email) : NULL;
    c.phone = phone ? strdup(phone) : NULL;
    c.priority = priority;
    return c;
}

// Helper to free a test_contact's internal memory
static inline void free_test_contact_internal(test_contact *c) {
    if (!c) return;
    if (c->email) { free(c->email); c->email = NULL; }
    if (c->phone) { free(c->phone); c->phone = NULL; }
}

// Helper to deep copy a test_contact
static inline test_contact copy_test_contact(const test_contact *src) {
    test_contact c;
    c.email = src->email ? strdup(src->email) : NULL;
    c.phone = src->phone ? strdup(src->phone) : NULL;
    c.priority = src->priority;
    return c;
}

// Create a complex test_struct with all fields populated
static inline test_struct create_test_struct(int id, const char *name, double score) {
    test_struct ts;
    memset(&ts, 0, sizeof(test_struct));
    
    ts.id = id;
    ts.name = name ? strdup(name) : NULL;
    ts.score = score;
    ts.flags = TEST_FLAG_ACTIVE;
    
    // Create address
    char street[64], city[32];
    snprintf(street, sizeof(street), "%d Main Street", id * 100);
    snprintf(city, sizeof(city), "City%d", id % 10);
    ts.address = create_test_address(street, city, 10000 + id);
    
    // Create contacts array (2-4 contacts based on id)
    ts.contact_count = 2 + (id % 3);
    ts.contacts = (test_contact *)malloc(sizeof(test_contact) * ts.contact_count);
    for (int i = 0; i < ts.contact_count; i++) {
        char email[64], phone[20];
        snprintf(email, sizeof(email), "contact%d_%d@test.com", id, i);
        snprintf(phone, sizeof(phone), "+1-555-%04d", id * 10 + i);
        ts.contacts[i] = create_test_contact(email, phone, i);
    }
    
    // Create tags array (3-5 tags based on id)
    ts.tag_count = 3 + (id % 3);
    ts.tags = (int *)malloc(sizeof(int) * ts.tag_count);
    for (int i = 0; i < ts.tag_count; i++) {
        ts.tags[i] = id * 100 + i;
    }
    
    // Set some flags based on id
    if (id % 2 == 0) ts.flags |= TEST_FLAG_VERIFIED;
    if (id % 5 == 0) ts.flags |= TEST_FLAG_PREMIUM;
    if (id % 10 == 0) ts.flags |= TEST_FLAG_ADMIN;
    
    return ts;
}

// Create a minimal test_struct (for simpler tests)
static inline test_struct create_test_struct_simple(int id) {
    test_struct ts;
    memset(&ts, 0, sizeof(test_struct));
    
    ts.id = id;
    char name[32];
    snprintf(name, sizeof(name), "TestItem%d", id);
    ts.name = strdup(name);
    ts.score = (double)id * 1.5;
    ts.flags = TEST_FLAG_ACTIVE;
    ts.address = NULL;
    ts.contacts = NULL;
    ts.contact_count = 0;
    ts.tags = NULL;
    ts.tag_count = 0;
    
    return ts;
}

// Deep copy function for test_struct (used by data structures)
static inline void copy_test_struct(void *dest, const void *src) {
    test_struct *d = (test_struct *)dest;
    const test_struct *s = (const test_struct *)src;
    
    // Copy simple fields
    d->id = s->id;
    d->score = s->score;
    d->flags = s->flags;
    d->contact_count = s->contact_count;
    d->tag_count = s->tag_count;
    
    // Deep copy name
    d->name = s->name ? strdup(s->name) : NULL;
    
    // Deep copy address
    d->address = copy_test_address(s->address);
    
    // Deep copy contacts array
    if (s->contacts && s->contact_count > 0) {
        d->contacts = (test_contact *)malloc(sizeof(test_contact) * s->contact_count);
        for (int i = 0; i < s->contact_count; i++) {
            d->contacts[i] = copy_test_contact(&s->contacts[i]);
        }
    } else {
        d->contacts = NULL;
    }
    
    // Deep copy tags array
    if (s->tags && s->tag_count > 0) {
        d->tags = (int *)malloc(sizeof(int) * s->tag_count);
        memcpy(d->tags, s->tags, sizeof(int) * s->tag_count);
    } else {
        d->tags = NULL;
    }
}

// Free function for test_struct (used by data structures)
static inline void free_test_struct(void *el) {
    test_struct *e = (test_struct *)el;
    
    // Free name
    if (e->name) {
        free(e->name);
        e->name = NULL;
    }
    
    // Free address
    if (e->address) {
        free_test_address(e->address);
        e->address = NULL;
    }
    
    // Free contacts array
    if (e->contacts) {
        for (int i = 0; i < e->contact_count; i++) {
            free_test_contact_internal(&e->contacts[i]);
        }
        free(e->contacts);
        e->contacts = NULL;
    }
    e->contact_count = 0;
    
    // Free tags array
    if (e->tags) {
        free(e->tags);
        e->tags = NULL;
    }
    e->tag_count = 0;
}

// Print function for test_struct
static inline void print_test_struct(FILE *stream, const void *el) {
    const test_struct *e = (const test_struct *)el;
    
    fprintf(stream, "{\n");
    fprintf(stream, "  id: %d,\n", e->id);
    fprintf(stream, "  name: \"%s\",\n", e->name ? e->name : "NULL");
    fprintf(stream, "  score: %.2f,\n", e->score);
    fprintf(stream, "  flags: 0x%02x,\n", e->flags);
    
    if (e->address) {
        fprintf(stream, "  address: {street: \"%s\", city: \"%s\", zip: %d},\n",
                e->address->street ? e->address->street : "NULL",
                e->address->city ? e->address->city : "NULL",
                e->address->zip_code);
    } else {
        fprintf(stream, "  address: NULL,\n");
    }
    
    fprintf(stream, "  contacts: [");
    for (int i = 0; i < e->contact_count; i++) {
        fprintf(stream, "{email: \"%s\", phone: \"%s\", priority: %d}",
                e->contacts[i].email ? e->contacts[i].email : "NULL",
                e->contacts[i].phone ? e->contacts[i].phone : "NULL",
                e->contacts[i].priority);
        if (i < e->contact_count - 1) fprintf(stream, ", ");
    }
    fprintf(stream, "],\n");
    
    fprintf(stream, "  tags: [");
    for (int i = 0; i < e->tag_count; i++) {
        fprintf(stream, "%d", e->tags[i]);
        if (i < e->tag_count - 1) fprintf(stream, ", ");
    }
    fprintf(stream, "]\n");
    
    fprintf(stream, "}");
}

// Compact print for inline display
static inline void print_test_struct_compact(FILE *stream, const void *el) {
    const test_struct *e = (const test_struct *)el;
    fprintf(stream, "{id:%d, name:\"%s\", score:%.1f}", 
            e->id, e->name ? e->name : "NULL", e->score);
}

// Compare by id (ascending - for min heap / ascending sort)
static inline int comp_test_struct_min(const void *a, const void *b) {
    const test_struct *ea = (const test_struct *)a;
    const test_struct *eb = (const test_struct *)b;
    return eb->id - ea->id;
}

// Compare by id (descending - for max heap / descending sort)
static inline int comp_test_struct_max(const void *a, const void *b) {
    const test_struct *ea = (const test_struct *)a;
    const test_struct *eb = (const test_struct *)b;
    return ea->id - eb->id;
}

// Compare by score (ascending)
static inline int comp_test_struct_by_score(const void *a, const void *b) {
    const test_struct *ea = (const test_struct *)a;
    const test_struct *eb = (const test_struct *)b;
    if (ea->score < eb->score) return -1;
    if (ea->score > eb->score) return 1;
    return 0;
}

// Compare by name (lexicographic)
static inline int comp_test_struct_by_name(const void *a, const void *b) {
    const test_struct *ea = (const test_struct *)a;
    const test_struct *eb = (const test_struct *)b;
    if (!ea->name && !eb->name) return 0;
    if (!ea->name) return -1;
    if (!eb->name) return 1;
    return strcmp(ea->name, eb->name);
}

// Equality check for test_struct (deep comparison)
static inline int test_struct_equals(const test_struct *a, const test_struct *b) {
    if (a->id != b->id) return 0;
    if (a->score != b->score) return 0;
    if (a->flags != b->flags) return 0;
    
    // Compare names
    if ((a->name == NULL) != (b->name == NULL)) return 0;
    if (a->name && strcmp(a->name, b->name) != 0) return 0;
    
    // Compare addresses
    if ((a->address == NULL) != (b->address == NULL)) return 0;
    if (a->address) {
        if (a->address->zip_code != b->address->zip_code) return 0;
        if ((a->address->street == NULL) != (b->address->street == NULL)) return 0;
        if (a->address->street && strcmp(a->address->street, b->address->street) != 0) return 0;
        if ((a->address->city == NULL) != (b->address->city == NULL)) return 0;
        if (a->address->city && strcmp(a->address->city, b->address->city) != 0) return 0;
    }
    
    // Compare contacts
    if (a->contact_count != b->contact_count) return 0;
    for (int i = 0; i < a->contact_count; i++) {
        if (a->contacts[i].priority != b->contacts[i].priority) return 0;
        if ((a->contacts[i].email == NULL) != (b->contacts[i].email == NULL)) return 0;
        if (a->contacts[i].email && strcmp(a->contacts[i].email, b->contacts[i].email) != 0) return 0;
    }
    
    // Compare tags
    if (a->tag_count != b->tag_count) return 0;
    for (int i = 0; i < a->tag_count; i++) {
        if (a->tags[i] != b->tags[i]) return 0;
    }
    
    return 1;
}

// Hash function for test_struct (for hash-based containers)
static inline size_t hash_test_struct(const void *el) {
    const test_struct *e = (const test_struct *)el;
    size_t hash = 17;
    hash = hash * 31 + (size_t)e->id;
    if (e->name) {
        for (const char *c = e->name; *c; c++) {
            hash = hash * 31 + (size_t)*c;
        }
    }
    return hash;
}

static inline void print_int(FILE *stream, const void *el) { fprintf(stream, "%d", *(int *)el); }

static inline int comp_int_min(const void *a, const void *b) { return *(int *)b - *(int *)a; }
static inline int comp_int_max(const void *a, const void *b) { return *(int *)a - *(int *)b; }

static inline void copy_string(void *dest, const void *src) {
    char *d = (char*)dest;
    const char *s = (const char*)src;
    strcpy(d, s);
}

// Helper to get standard int attributes
static inline univ_attr_t get_int_attr() {
    return (univ_attr_t){
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .comp = comp_int_max
    };
}

static inline univ_attr_t get_test_struct_attr() {
    return (univ_attr_t){
        .size = sizeof(test_struct),
        .fr = free_test_struct,
        .copy = copy_test_struct,
        .print = print_test_struct_compact,
        .comp = comp_test_struct_max
    };
}

// Get test_struct attr with custom comparator
static inline univ_attr_t get_test_struct_attr_by_score() {
    return (univ_attr_t){
        .size = sizeof(test_struct),
        .fr = free_test_struct,
        .copy = copy_test_struct,
        .print = print_test_struct_compact,
        .comp = comp_test_struct_by_score
    };
}

// Get test_struct attr with name comparator
static inline univ_attr_t get_test_struct_attr_by_name() {
    return (univ_attr_t){
        .size = sizeof(test_struct),
        .fr = free_test_struct,
        .copy = copy_test_struct,
        .print = print_test_struct_compact,
        .comp = comp_test_struct_by_name
    };
}

// Get test_struct attr for min-heap ordering
static inline univ_attr_t get_test_struct_attr_min() {
    return (univ_attr_t){
        .size = sizeof(test_struct),
        .fr = free_test_struct,
        .copy = copy_test_struct,
        .print = print_test_struct_compact,
        .comp = comp_test_struct_min
    };
}