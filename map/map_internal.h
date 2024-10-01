#include <string.h>
#include <stdlib.h>

#include "../include/map.h"

#define BLACK 0
#define RED 1

map_node *map_node_init(void *key, int key_size, void *val, int val_size);
void map_rotate_left(map_node *node);
void map_rotate_right(map_node *node);