#include "map_internal.h"

size_t map_element_dim ( map *m ) {
    return m->key_dim + m->value_dim;
}

int map_element_compare ( map *m, void *key1, void *key2 ) {
    if ( m->key_compare == NULL )
        return universal_compare ( key1, key2, m->key_dim );
    else
        return m->key_compare ( key1, key2 );
}

map_node *map_find_node ( map *m, void *key ) {
    map_node *aux = m->root;
    while ( aux != NULL ) {
        int compare_res = map_element_compare ( m, aux->data, key );
        if ( compare_res == 0 )
            break;
        if ( compare_res < 0 )
            aux = aux->left_child;
        else
            aux = aux->right_child;
    }
    return aux;
}

map_node *map_node_initiate ( void *key, size_t key_dim, void *value, size_t value_dim ) {
    map_node *node = malloc ( MAP_NODE_SIZE );
    if ( !node ) return NULL;
    node->father = NULL;
    node->left_child = NULL;
    node->right_child = NULL;
    node->color = RED;
    node->data = malloc ( key_dim + value_dim );
    if ( !node->data ) {
        free ( node );
        return NULL;
    }
    if ( key != NULL )
        memcpy ( node->data, key, key_dim );
    else
        memset ( node->data, 0, key_dim );
    if ( value != NULL )
        memcpy ( node->data + key_dim, value, value_dim );
    else
        memset ( node->data + key_dim, 0, value_dim );
    return node;
}

map_node *map_find_min_node ( map_node *node ) {
    while ( node->left_child != NULL )
        node = node->left_child;
    return node;
}

void map_change_father ( map_node *node, map_node *son ) {
    map_node *father = node->father;
    node->father = son;
    son->father = father;
    if ( father == NULL ) return;
    if ( father->left_child == node )
        father->left_child = son;
    else
        father->right_child = son;
}

void map_left_rotate ( map_node *node ) {
    if ( node->right_child == NULL ) return;
    map_node *right = node->right_child;
    node->right_child = right->left_child;
    right->left_child = node;
    map_change_father ( node, right );
}

void map_right_rotate ( map_node *node ) {
    if ( node->left_child == NULL ) return;
    map_node *left = node->left_child;
    node->left_child = left->right_child;
    left->right_child = node;
    map_change_father ( node, left );
}

void map_node_print ( map_node *node, printer key_print, printer value_print, size_t key_dim ) {
    key_print ( node->data );
    value_print ( node->data + key_dim );
    if ( node->left_child != NULL )
        map_node_print ( node->left_child, key_print, value_print, key_dim );
    if ( node->right_child != NULL )
        map_node_print ( node->right_child, key_print, value_print, key_dim );
}

void map_node_free ( map_node *node ) {
    free ( node->data );
    free ( node );
}

void map_tree_free ( map_node *node ) {
    if ( node->left_child != NULL )
        map_tree_free ( node->left_child );
    if ( node->right_child != NULL )
        map_tree_free ( node->right_child );
    map_node_free ( node );
}