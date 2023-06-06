#ifndef MAP_H
#define MAP_H

#include "universal.h"

typedef struct pair {
    void *first, *second;
    size_t dim1, dim2;
} pair;

typedef struct map {
    pair *vec;
    size_t cap, size, used_cap;
} map;

#define MAP_SIZE sizeof ( map )
#define PAIR_SIZE sizeof ( pair )

map *map_initiate ( );
enum return_codes map_pair_insert ( map *m, pair *p );
enum return_codes map_clear ( map *m );
enum return_codes map_erase ( map *m, void *key, size_t dim );
pair *map_get_value ( map *m, void *key, size_t dim );
void map_print ( map *m, printer print );
void map_free ( map *m );

#endif