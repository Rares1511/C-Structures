#include "map.h"

size_t pair_size ( pair* p ) {
    return 2 * sizeof ( size_t ) + p->dim1 + p->dim2;
}

pair *get_pair ( map *m, size_t pos ) {
    pair *p = malloc ( PAIR_SIZE );
    memcpy ( &p->dim1, m->vec + pos, sizeof ( size_t ) );
    memcpy ( &p->dim2, m->vec + pos + sizeof ( size_t ), sizeof ( size_t ) );

    p->first = malloc ( p->dim1 );
    p->second = malloc ( p->dim2 );

    memcpy ( p->first, m->vec + pos + 2 * sizeof ( size_t ), p->dim1 );
    memcpy ( p->second, m->vec + pos + 2 * sizeof ( size_t ) + p->dim1, p->dim2 );

    return p;
}

size_t get_starting_point ( map *m ) {
    size_t pos = 0, aux1, aux2;
    for ( size_t i = 0; i < m->size; i++ ) {
        memcpy ( &aux1, m->vec + pos, sizeof ( size_t ) );
        memcpy ( &aux2, m->vec + pos + sizeof ( size_t ), sizeof ( size_t ) );
        pos += 2 * sizeof ( size_t ) + aux1 + aux2;
    }
    return pos;
}

/* initializes the map knowing that the sizeof the key is key_dim and of the values
is value_dim */
map *map_initiate ( ) {
    map *m = malloc ( MAP_SIZE );
    if ( !m ) return NULL;
    m->size = 0;
    m->cap = INIT_CAPACITY * PAIR_SIZE;
    m->used_cap = 0;
    m->vec = malloc ( m->cap );
    return m;
}

/* inserts the given pair in the map or returns an error if the key already exists */
enum return_codes map_pair_insert ( map *m, pair *p ) {
    if ( m->cap + pair_size ( p ) > m->cap ) {
        m->vec = realloc ( m->vec, ( m->cap + INIT_CAPACITY ) );
        if ( !m->vec ) {
            free ( m );
            return MEMORY_PROBLEM;
        }
        m->cap += INIT_CAPACITY;
    }
    size_t pos = get_starting_point ( m );
    memcpy ( m->vec + pos, &p->dim1, sizeof ( size_t ) );
    memcpy ( m->vec + pos + sizeof ( size_t ), &p->dim2, sizeof ( size_t ) );
    memcpy ( m->vec + pos + 2 * sizeof ( size_t ), p->first, p->dim1 );
    memcpy ( m->vec + pos + 2 * sizeof ( size_t ) + p->dim1, p->second, p->dim2 );
    m->size++;
    m->used_cap += pair_size ( p );
    return SUCCESSFUL_ADDITION;
}

/* returns the pair in which the key is the one given, or NULL if there is no value with the key */
pair *map_get_value ( map *m, void *key, size_t dim ) {
    size_t pos = 0;
    pair *p;
    for ( size_t i = 0; i < m->size; i++ ) {
        p = get_pair ( m, pos );
        pos += pair_size ( p );
        if ( p->dim1 != dim ) continue;
        if ( universal_compare ( p->first, key, dim ) == 0 )
            return p;
    }
    return NULL;
}

/* deletes the entry in the map that has the given key */
enum return_codes map_erase ( map *m, void *key, size_t dim ) {
    size_t pos = 0;
    pair *p;
    for ( size_t i = 0; i < m->size; i++ ) {
        p = get_pair ( m, pos );
        if ( p->dim1 == dim && universal_compare ( p->first, key, dim ) == 0 ) {
            memcpy ( m->vec + pos, m->vec + pos + pair_size ( p ), m->used_cap - pos - pair_size ( p ) );
            return SUCCESSFUL_DELETION;
        }
        pos += pair_size ( p );
    }
    return WRONG_ELEMENT;
}

/* updates the value of the given key with the new value */
enum return_codes map_update ( map *m, void *key, size_t key_dim, void *val, size_t val_dim ) {
    size_t pos = 0;
    pair *p;
    for ( size_t i = 0; i < m->size; i++ ) {
        p = get_pair ( m, pos );
        if ( p->dim1 == key_dim && universal_compare ( p->first, key, key_dim ) == 0 ) {
            if ( m->used_cap - p->dim2 + val_dim > m->cap ) {
                m->vec = realloc ( m->vec, ( m->cap + INIT_CAPACITY ) );
                if ( !m->vec ) {
                    free ( m );
                    return MEMORY_PROBLEM;
                }
                m->cap += INIT_CAPACITY;
            }
            memcpy ( m->vec + pos + pair_size ( p ) - p->dim2 + val_dim, m->vec + pos + pair_size ( p ), m->used_cap - pos );
            memcpy ( m->vec + pos + pair_size ( p ) - p->dim2, val, val_dim );
            return SUCCESSFUL_REPLACEMENT;
        }
        pos += pair_size ( p );
    }
    return WRONG_ELEMENT;
}

/* empty the map */
enum return_codes map_clear ( map *m ) {
    size_t pos = 0, aux1, aux2;
    for ( size_t i = 0; i < m->size; i++ ) {
        memcpy ( &aux1, m->vec + pos, sizeof ( size_t ) );
        memcpy ( &aux2, m->vec + pos + sizeof ( size_t ), sizeof ( size_t ) );
        memset ( m->vec + pos, 0, aux1 + aux2 + 2 * sizeof ( size_t ) );
        pos += aux1 + aux2 + 2 * sizeof ( size_t );
    }
    m->size = 0;
    m->used_cap = 0;
}

/* frees the memory the map uses */
void map_free ( map *m ) {
    free ( m->vec );
    free ( m );
}

/* prints the map with the given printer function for individual elements */
void map_print ( map *m, printer print ) {
    pair *p;
    size_t pos = 0;
    for ( size_t i = 0; i < m->size; i++ ) {
        p = get_pair ( m, pos );
        pos += pair_size ( p );
        print ( p );
    }
    printf ( "\n" );
}