#include "map.h"

#define RED 0
#define BLACK 1

size_t map_element_dim ( map *m );
int map_element_compare ( map *m, void *key1, void *key2 );
void map_node_free ( map_node *node );
map_node *map_find_node ( map *m, void *key );
map_node *map_node_initiate ( void *key, size_t key_dim, void *value, size_t value_dim );
map_node *map_find_min_node ( map_node *node );
void map_change_father ( map_node *node, map_node *son );
void map_left_rotate ( map_node *node );
void map_right_rotate ( map_node *node );
void map_node_print ( map_node *node, printer key_print, printer value_print, size_t key_dim );
void map_tree_free ( map_node *node );