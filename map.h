#ifndef MAP_H
#define MAP_H

#include "universal.h"

typedef struct pair {
    void *first, *second;
    size_t dim1, dim2;
} pair;

typedef struct map {
    void *vec;
    size_t cap, size, used_cap;
} map;

#define MAP_SIZE sizeof ( map )
#define PAIR_SIZE sizeof ( pair )

#define pair_size(p) 2 * sizeof(size_t) + p->dim1 + p->dim2

map *map_initiate ( );
enum return_codes map_pair_insert ( map *m, pair *p );
enum return_codes map_clear ( map *m );
enum return_codes map_erase ( map *m, void *key, size_t dim );
enum return_codes map_update ( map *m, void *key, size_t key_dim, void *val, size_t val_dim );
pair *map_get_value ( map *m, void *key, size_t dim );
pair *map_make_pair ( void *key, size_t key_dim, void *value, size_t val_dim );
void map_print ( map *m, printer print );
void map_free ( map *m );

#endif