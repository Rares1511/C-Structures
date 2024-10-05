#include "map_internal.h"

#include <stdio.h>

map_node *map_node_init(void *key, int key_size, void *val, int val_size)
{
    map_node *node = malloc(sizeof(map_node));
    if (!node)
        return NULL;

    node->father = NULL;
    node->left_child = NULL;
    node->right_child = NULL;
    node->color = RED;
    node->key = malloc(key_size);
    if (!node->key)
    {
        free(node);
        return NULL;
    }
    node->val = malloc(val_size);
    if (!node->val)
    {
        free(node->key);
        free(node);
        return NULL;
    }

    memcpy(node->key, key, key_size);
    memcpy(node->val, val, val_size);

    return node;
}

map_node *map_node_find(map m, void *key)
{
    map_node *node = m.root;
    int comp_res;

    while (node != NULL)
    {
        if (m.key_attr.comp)
        {
            comp_res = m.key_attr.comp(node->key, key);
        }
        else
        {
            comp_res = universal_compare(node->key, key, m.key_attr.size);
        }

        if (comp_res == 0)
        {
            return node;
        }
        else if (comp_res > 0)
        {
            node = node->left_child;
        }
        else
            node = node->right_child;
    }

    return NULL;
}

void map_rotate_left(map *m, map_node *node)
{
    map_node *right_child = node->right_child;
    node->right_child = right_child->left_child;
    if (right_child->left_child != NULL)
    {
        right_child->left_child->father = node;
    }
    right_child->father = node->father;
    if (node->father == NULL)
    {
        m->root = right_child;
    }
    else if (node == node->father->left_child)
    {
        node->father->left_child = right_child;
    }
    else
    {
        node->father->right_child = right_child;
    }
    right_child->left_child = node;
    node->father = right_child;
}

void map_rotate_right(map *m, map_node *node)
{
    map_node *left_child = node->left_child;
    node->left_child = left_child->right_child;
    if (left_child->right_child != NULL)
    {
        left_child->right_child->father = node;
    }
    left_child->father = node->father;
    if (node->father == NULL)
    {
        m->root = left_child;
    }
    else if (node == node->father->right_child)
    {
        node->father->right_child = left_child;
    }
    else
    {
        node->father->left_child = left_child;
    }
    left_child->right_child = node;
    node->father = left_child;
}