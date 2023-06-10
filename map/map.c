#include "map_internal.h"

/* initializes the map knowing that the size of the key and value are the ones offered */
map *map_initiate ( size_t key_dim, size_t value_dim ) {
    map *m = malloc ( MAP_SIZE );
    if ( !m ) return NULL;
    m->root = NULL;
    m->key_compare = NULL;
    m->value_compare = NULL;
    m->value_dim = value_dim;
    m->key_dim = key_dim;
    return m;
}

/* sets the key compare function to the one offered */
void map_set_key_compare ( map *m, comparer compare ) {
    m->key_compare = compare;
}

/* sets the value compare function to the one offered */
void map_set_value_compare ( map *m, comparer compare ) {
    m->value_compare = compare;
}

/* helper function for insert, basic BST insertion */
enum return_codes map_insert_node ( map *m, map_node *node ) {
    map_node *aux = m->root;
    if ( m->root == NULL ) {
        m->root = node;
        m->root->color = BLACK;
        return SUCCESSFUL_ADDITION;
    }
    while ( aux != NULL ) {
        int compare_res = map_element_compare ( m, aux->data, node->data );
        if ( compare_res < 0 ) {
            if ( aux->left_child == NULL ) {
                aux->left_child = node;
                break;
            }
            aux = aux->left_child;
        }
        else if ( compare_res > 0 ) {
            if ( aux->right_child == NULL ) {
                aux->right_child = node;
                break;
            }
            aux = aux->right_child;
        }
        else
            return WRONG_ELEMENT;
    }
    node->father = aux;
    return SUCCESSFUL_ADDITION;
}

/* fixes the RBT after the BST insertion so it keeps its properties */
void map_insert_node_fixup ( map *m, map_node *node ) {
    map_node *aux = node;
    while ( aux->father != NULL && aux->father->father != NULL && aux->father->color == RED ) {
        map_node *uncle;
        if ( aux->father == aux->father->father->left_child )
            uncle = aux->father->father->right_child;
        else
            uncle = aux->father->father->left_child;
        if ( uncle != NULL && uncle->color == RED ) {
            aux->father->color = BLACK;
            uncle->color = BLACK;
            aux->father->father->color = RED;
            aux = aux->father->father;
        }
        else {
            if ( aux == aux->father->right_child && aux->father == aux->father->father->left_child ) {
                printf ( "triangle left\n" );
                aux = aux->father; 
                map_left_rotate ( aux );
            }
            else if ( aux == aux->father->left_child && aux->father == aux->father->father->right_child ) {
                printf ( "triangle right\n" );
                aux = aux->father;
                map_right_rotate ( aux );
            }
            aux->father->color = BLACK;
            aux->father->father->color = RED;
            if ( aux == aux->father->left_child ) {
                printf ( "line left\n" );
                map_right_rotate ( aux->father->father );
            }
            else {
                printf ( "line right\n" );
                map_left_rotate ( aux->father->father );
            }
        }
    }
    printf ( "%p\n", m->root->father );
    while ( m->root->father != NULL )
        m->root = m->root->father;
    m->root->color = BLACK;
}

/* inserts the pair of key and value in the map */
enum return_codes map_insert ( map *m, void *key, void *value ) {
    map_node *node = map_node_initiate ( key, m->key_dim, value, m->value_dim );
    if ( !node ) return MEMORY_PROBLEM;
    int rc = map_insert_node ( m, node );
    if ( rc == WRONG_ELEMENT ) return WRONG_ELEMENT;
    printf ( "done inserting\n" );
    map_insert_node_fixup ( m, node );
    m->size++;
    return SUCCESSFUL_ADDITION;
}

/* updates the value stored with the offered key, and changes it with the value offered, 
returns WRONG_ELEMENT if the key offered was not found in the map */
enum return_codes map_update ( map *m, void *key, void *value ) {
    map_node *aux = map_find_node ( m, key );
    if ( aux == NULL ) return WRONG_ELEMENT;
    memcpy ( aux->data + m->key_dim, value, m->value_dim );
    return SUCCESSFUL_REPLACEMENT;
}

/* helper function which replaces the delete_node with replace_node considering that
delete_node will be deleted */
void map_erase_node ( map *m, map_node *delete_node, map_node *replace_node ) {
    if ( delete_node->father == NULL )
        m->root = replace_node;
    else if ( delete_node == delete_node->father->left_child )
        delete_node->father->left_child = replace_node;
    else
        delete_node->father->right_child = replace_node;
    if ( replace_node != NULL )
        replace_node->father = delete_node->father;
}

/* helper function for case 1 of the deletion fixup */
void map_erase_fixup_case1 ( map_node *node, map_node *brother, uint8_t type ) {
    brother->color = BLACK;
    node->father = RED;
    if ( type == 1 ) {
        map_left_rotate ( node->father );
        brother = node->father->right_child;
    }
    else {
        map_right_rotate ( node->father );
        brother = node->father->left_child;
    }
}

/* helper function for case 2 of the deletion fixup */
void map_erase_fixup_case2 ( map_node *node, map_node *brother ) {
    brother->color = RED;
    node = node->father;
}

/* helper function for case 3 of the deletion fixup */
void map_erase_fixup_case3 ( map_node *node, map_node *brother, uint8_t type ) {
    brother->color = RED;
    if ( type == 1 ) {
        brother->left_child->color = BLACK;
        map_right_rotate ( brother );
        brother = node->father->right_child;
    }
    else {
        brother->right_child->color = BLACK;
        map_left_rotate ( brother );
        brother = node->father->left_child;
    }
}

/* helper function for case 4 of the deletion fixup */
void map_erase_fixup_case4 ( map_node *node, map_node *brother, uint8_t type ) {
    brother->color = node->father->color;
    node->father->father->color = BLACK;
    if ( type == 1 ) {
        brother->right_child->color = BLACK;
        map_right_rotate ( node->father );
    }
    else {
        brother->left_child->color = BLACK;
        map_left_rotate ( node->father );
    }
}

/* helper function for the deletion which corrects the tree so it respects the RBT 
properties */
void map_erase_fixup ( map *m, map_node *node ) {
    map_node *brother;
    while ( node != m->root && node->color == BLACK ) {
        uint8_t type;
        if ( node->father->left_child == node ) {
            brother = node->father->right_child;
            type = 1;
        }
        else {
            brother = node->father->left_child;
            type = 2;
        }
        if ( brother->color == RED )
            map_erase_fixup_case1 ( node, brother, type );
        else if ( brother->left_child->color == BLACK && brother->right_child->color == BLACK )
            map_erase_fixup_case2 ( node, brother );
        else if ( ( brother->right_child->color == BLACK && type == 1 ) || ( brother->left_child->color == BLACK && type == 2 ) )
            map_erase_fixup_case3 ( node, brother, type );
        else
            map_erase_fixup_case4 ( node, brother, type );
    }
    node->color = BLACK;
}

/* deletes the element stored with the key, returns WRONG_ELEMENT if the key is not stored */
enum return_codes map_erase ( map *m, void *key ) {
    map_node *delete_node = map_find_node ( m, key ), *replace_node;
    if ( delete_node == NULL ) return WRONG_ELEMENT;
    int original_color = delete_node->color;
    if ( delete_node->left_child == NULL ) {
        map_node *replace_node = delete_node->right_child;
        map_erase_node ( m, delete_node, replace_node );
    }
    else if ( delete_node->right_child == NULL ) {
        map_node *replace_node = delete_node->left_child;
        map_erase_node ( m, delete_node, replace_node );
    }
    else {
        map_node *replace_node = map_find_min_node ( delete_node->right_child );
        original_color = replace_node->color;
        map_node *aux = replace_node->right_child;
        if ( replace_node->father == delete_node )
            aux->father = replace_node;
        else {
            map_erase_node ( m, replace_node, aux );
            replace_node->right_child = delete_node->right_child;
            replace_node->right_child->father = replace_node;
        }
        map_erase_node ( m, delete_node, replace_node );
        replace_node->left_child = delete_node->left_child;
        replace_node->left_child->father = replace_node;
        replace_node->color = delete_node->color;
        if ( aux != NULL )
            replace_node = aux;
        else {
            aux = replace_node;
            replace_node = map_node_initiate ( NULL, m->key_dim, NULL, m->value_dim );
            replace_node->father = aux;
            replace_node->color = BLACK;
        }
    }
    map_node_free ( delete_node );
    if ( original_color == BLACK )
        map_erase_fixup ( m, replace_node );
    return SUCCESSFUL_DELETION;
}

/* returns a reference to the value stored with the key offered or NULL if there is no
element with that key */
void *map_find ( map *m, void *key ) {
    map_node *aux = map_find_node ( m, key );
    if ( aux == NULL ) return NULL;
    void *ptr = malloc ( m->value_dim );
    memcpy ( ptr, aux->data + m->key_dim, m->value_dim );
    return ptr;
}

/* returns 1 if the key is found in the map, 0 otherwise */
int map_count ( map *m, void *key ) {
    map_node *aux = map_find_node ( m, key );
    if ( aux == NULL ) return 0;
    return 1;
}

/* swaps the two maps offered */
void map_swap ( map *m1, map *m2 ) {
    universal_swap ( m1, m2, MAP_SIZE );
}

/* frees the memory that map uses */
void map_free ( map *m ) {
    map_tree_free ( m->root );
    free ( m );
}

/* prints the map using the printer function offered for the individual elements */
void map_print ( map *m, printer key_print, printer value_print ) {
    printf ( "map: \n" );
    map_node_print ( m->root, key_print, value_print, m->key_dim );
}