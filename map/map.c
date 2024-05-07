#include "../include/map.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED 0
#define BLACK 1

map_node *map_node_init(void *key, int key_dim, void *val, int val_dim)
{
    map_node *node = malloc(sizeof(map_node));
    if (!node)
        return NULL;
    node->father = NULL;
    node->left_child = NULL;
    node->right_child = NULL;
    node->color = RED;
    node->key = malloc(key_dim);
    if (!node->key)
    {
        free(node);
        return NULL;
    }
    node->val = malloc(val_dim);
    if (!node->val)
    {
        free(node->key);
        free(node);
        return NULL;
    }
    memcpy(node->key, key, key_dim);
    memcpy(node->val, val, val_dim);
    return node;
}

map_node *map_node_find(map m, void *key)
{
    map_node *aux = m.root;
    while (aux != NULL)
    {
        int res = m.key_attr.comp(key, aux->key);
        if (res > 0)
            aux = aux->right_child;
        else if (res < 0)
            aux = aux->left_child;
        else
            return aux;
    }
    return NULL;
}

cs_codes map_insert_node(map *m, map_node *node)
{
    if (m->root == NULL)
    {
        m->root = node;
        m->root->color = BLACK;
        return CS_SUCCESS;
    }
    map_node *aux = m->root;
    while (aux != NULL)
    {
        int res = m->key_attr.comp(node->key, aux->key);
        if (res < 0)
        {
            if (aux->left_child == NULL)
            {
                aux->left_child = node;
                break;
            }
            aux = aux->left_child;
        }
        else if (res > 0)
        {
            if (aux->right_child == NULL)
            {
                aux->right_child = node;
                break;
            }
            aux = aux->right_child;
        }
        else
            return CS_ELEM;
    }
    node->father = aux;
    return CS_SUCCESS;
}

void map_change_father(map_node *node, map_node *son)
{
    map_node *father = node->father;
    node->father = son;
    son->father = father;
    if (father == NULL)
        return;
    if (father->left_child == node)
        father->left_child = son;
    else
        father->right_child = son;
}

void map_left_rotate(map_node *node)
{
    if (node->right_child == NULL)
        return;
    map_node *right = node->right_child;
    node->right_child = right->left_child;
    right->left_child = node;
    map_change_father(node, right);
}

void map_right_rotate(map_node *node)
{
    if (node->left_child == NULL)
        return;
    map_node *left = node->left_child;
    node->left_child = left->right_child;
    left->right_child = node;
    map_change_father(node, left);
}

void map_insert_node_fixup(map *m, map_node *node)
{
    map_node *aux = node;
    while (aux->father != NULL && aux->father->father != NULL && aux->father->color == RED)
    {
        map_node *uncle;
        if (aux->father == aux->father->father->left_child)
            uncle = aux->father->father->right_child;
        else
            uncle = aux->father->father->left_child;
        if (uncle != NULL && uncle->color == RED)
        {
            aux->father->color = BLACK;
            uncle->color = BLACK;
            aux->father->father->color = RED;
            aux = aux->father->father;
        }
        else
        {
            if (aux == aux->father->right_child && aux->father == aux->father->father->left_child)
            {
                aux = aux->father;
                map_left_rotate(aux);
            }
            else if (aux == aux->father->left_child && aux->father == aux->father->father->right_child)
            {
                aux = aux->father;
                map_right_rotate(aux);
            }
            aux->father->color = BLACK;
            aux->father->father->color = RED;
            if (aux == aux->father->left_child)
            {
                map_right_rotate(aux->father->father);
            }
            else
            {
                map_left_rotate(aux->father->father);
            }
        }
    }
    while (m->root->father != NULL)
        m->root = m->root->father;
    m->root->color = BLACK;
}

cs_codes map_erase_node(map *m, map_node *delete_node)
{
    map_node *replace_node = delete_node->right_child;
}

void map_node_free(map_node *node, freer key_fr, freer val_fr)
{
    if (key_fr)
        key_fr(node->key);
    if (val_fr)
        val_fr(node->val);
    free(node->key);
    free(node->val);
    free(node);
}

cs_codes map_init(map *m, map_attr_t key_attr, map_attr_t val_attr)
{
    if (key_attr.size < 0 || key_attr.size > SIZE_TH || val_attr.size < 0 || val_attr.size > SIZE_TH)
        return CS_SIZE;
    m->key_attr = key_attr;
    m->val_attr = val_attr;
    m->size = 0;
    m->root = NULL;
    return CS_SUCCESS;
}

cs_codes map_insert(map *m, void *key, void *val)
{
    map_node *node = map_node_init(key, m->key_attr.size, val, m->val_attr.size);
    if (!node)
        return CS_MEM;
    int rc = map_insert_node(m, node);
    if (rc != CS_SUCCESS)
        return rc;
    map_insert_node_fixup(m, node);
    m->size++;
    return CS_SUCCESS;
}

cs_codes map_update(map *m, void *key, void *val)
{
    void *ptr = map_find(*m, key);
    if (!ptr)
        return CS_ELEM;
    memcpy(ptr, val, m->val_attr.size);
    return CS_SUCCESS;
}

cs_codes map_erase(map *m, void *key)
{
    map_node *delete_node = map_node_find(*m, key);
    int rc = map_erase_node(m, delete_node);
    if (rc != CS_SUCCESS)
        return rc;
    return CS_SUCCESS;
}

void *map_find(map m, void *key)
{
    map_node *aux = map_node_find(m, key);
    if (!aux)
        return NULL;
    return aux->val;
}

// /* helper function which replaces the delete_node with replace_node considering that
// delete_node will be deleted */
// void map_erase_node ( map *m, map_node *delete_node, map_node *replace_node ) {
//     if ( delete_node->father == NULL )
//         m->root = replace_node;
//     else if ( delete_node == delete_node->father->left_child )
//         delete_node->father->left_child = replace_node;
//     else
//         delete_node->father->right_child = replace_node;
//     if ( replace_node != NULL )
//         replace_node->father = delete_node->father;
// }

// /* helper function for case 1 of the deletion fixup */
// void map_erase_fixup_case1 ( map_node *node, map_node *brother, char type ) {
//     brother->color = BLACK;
//     node->father = RED;
//     if ( type == 1 ) {
//         map_left_rotate ( node->father );
//         brother = node->father->right_child;
//     }
//     else {
//         map_right_rotate ( node->father );
//         brother = node->father->left_child;
//     }
// }

// /* helper function for case 2 of the deletion fixup */
// void map_erase_fixup_case2 ( map_node *node, map_node *brother ) {
//     brother->color = RED;
//     node = node->father;
// }

// /* helper function for case 3 of the deletion fixup */
// void map_erase_fixup_case3 ( map_node *node, map_node *brother, char type ) {
//     brother->color = RED;
//     if ( type == 1 ) {
//         brother->left_child->color = BLACK;
//         map_right_rotate ( brother );
//         brother = node->father->right_child;
//     }
//     else {
//         brother->right_child->color = BLACK;
//         map_left_rotate ( brother );
//         brother = node->father->left_child;
//     }
// }

// /* helper function for case 4 of the deletion fixup */
// void map_erase_fixup_case4 ( map_node *node, map_node *brother, char type ) {
//     brother->color = node->father->color;
//     node->father->father->color = BLACK;
//     if ( type == 1 ) {
//         brother->right_child->color = BLACK;
//         map_right_rotate ( node->father );
//     }
//     else {
//         brother->left_child->color = BLACK;
//         map_left_rotate ( node->father );
//     }
// }

// /* helper function for the deletion which corrects the tree so it respects the RBT
// properties */
// void map_erase_fixup ( map *m, map_node *node ) {
//     map_node *brother;
//     while ( node != m->root && node->color == BLACK ) {
//         char type;
//         if ( node->father->left_child == node ) {
//             brother = node->father->right_child;
//             type = 1;
//         }
//         else {
//             brother = node->father->left_child;
//             type = 2;
//         }
//         if ( brother->color == RED )
//             map_erase_fixup_case1 ( node, brother, type );
//         else if ( brother->left_child->color == BLACK && brother->right_child->color == BLACK )
//             map_erase_fixup_case2 ( node, brother );
//         else if ( ( brother->right_child->color == BLACK && type == 1 ) || ( brother->left_child->color == BLACK && type == 2 ) )
//             map_erase_fixup_case3 ( node, brother, type );
//         else
//             map_erase_fixup_case4 ( node, brother, type );
//     }
//     node->color = BLACK;
// }

// /* deletes the element stored with the key, returns WRONG_ELEMENT if the key is not stored */
// cs_codes map_erase ( map *m, void *key ) {
//     map_node *delete_node = map_node_find ( m, key ), *replace_node;
//     if ( delete_node == NULL ) return CS_ELEM;
//     int original_color = delete_node->color;
//     if ( delete_node->left_child == NULL ) {
//         map_node *replace_node = delete_node->right_child;
//         map_erase_node ( m, delete_node, replace_node );
//     }
//     else if ( delete_node->right_child == NULL ) {
//         map_node *replace_node = delete_node->left_child;
//         map_erase_node ( m, delete_node, replace_node );
//     }
//     else {
//         map_node *replace_node = map_find_min_node ( delete_node->right_child );
//         original_color = replace_node->color;
//         map_node *aux = replace_node->right_child;
//         if ( replace_node->father == delete_node )
//             aux->father = replace_node;
//         else {
//             map_erase_node ( m, replace_node, aux );
//             replace_node->right_child = delete_node->right_child;
//             replace_node->right_child->father = replace_node;
//         }
//         map_erase_node ( m, delete_node, replace_node );
//         replace_node->left_child = delete_node->left_child;
//         replace_node->left_child->father = replace_node;
//         replace_node->color = delete_node->color;
//         if ( aux != NULL )
//             replace_node = aux;
//         else {
//             aux = replace_node;
//             replace_node = map_node_initiate ( NULL, m->key_attr.size, NULL, m->val_attr.size );
//             replace_node->father = aux;
//             replace_node->color = BLACK;
//         }
//     }
//     map_node_free ( delete_node );
//     if ( original_color == BLACK )
//         map_erase_fixup ( m, replace_node );
//     return CS_SUCCESS;
// }

void map_swap(map *m1, map *m2)
{
    map_attr_t key_attr = m1->key_attr;
    map_node *root = m1->root;
    int size = m1->size;
    map_attr_t val_attr = m1->val_attr;

    m1->key_attr = m2->key_attr;
    m1->root = m2->root;
    m1->size = m2->size;
    m1->val_attr = m2->val_attr;

    m2->key_attr = key_attr;
    m2->root = root;
    m2->size = size;
    m2->val_attr = val_attr;
}

void map_free(map *m)
{
    map_node *node = m->root;
    while (node != NULL)
    {
        if (node->left_child != NULL)
        {
            node = node->left_child;
            node->father->left_child = NULL;
            continue;
        }
        if (node->right_child != NULL)
        {
            node = node->right_child;
            node->father->right_child = NULL;
            continue;
        }
        map_node *aux = node;
        node = node->father;
        map_node_free(aux, m->key_attr.fr, m->val_attr.fr);
    }
}

void map_print(map *m)
{
}

void map_print_debug_helper(map_node *node, int tab, printer key_print, printer val_print)
{
    if (!node)
        return;
    for (int i = 0; i < abs(tab); i++)
        printf(" ");
    if (tab > 0)
        printf("r: ");
    else if (tab < 0)
        printf("l: ");
    printf("(");
    key_print(node->key);
    printf(", ");
    val_print(node->val);
    printf(", ");
    if (node->color == BLACK)
        printf("BLACK");
    else
        printf("RED");
    printf(")");
    printf("\n");
    map_print_debug_helper(node->left_child, -abs(tab) - 2, key_print, val_print);
    map_print_debug_helper(node->right_child, abs(tab) + 2, key_print, val_print);
}

void map_print_debug(map *m)
{
    map_print_debug_helper(m->root, 0, m->key_attr.print, m->val_attr.print);
}