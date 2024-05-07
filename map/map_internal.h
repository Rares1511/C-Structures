#include "../include/map.h"

#define RED 0
#define BLACK 1

map_node *map_find_node ( map *m, void *key );
map_node *map_node_initiate ( void *key, int key_dim, void *value, int value_dim );
map_node *map_find_min_node ( map_node *node );
void map_change_father ( map_node *node, map_node *son );
void map_left_rotate ( map_node *node );
void map_right_rotate ( map_node *node );