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

void map_rotate_left(map *m, map_node *node)
{
    map_node *right_child = node->right_child;

    node->right_child = node->right_child->left_child;
    if (node->right_child)
    {
        node->right_child->father = node;
    }
    right_child->left_child = NULL;

    if (!node->father)
    {
        m->root = right_child;
    }
    else
    {
        if (node->father->left_child == node)
        {
            node->father->left_child = right_child;
        }
        else
        {
            node->father->right_child = right_child;
        }
    }
    right_child->left_child = node;
    right_child->father = node->father;
    node->father = right_child;
}

void map_rotate_right(map *m, map_node *node)
{
    map_node *left_child = node->left_child;

    node->left_child = node->left_child->right_child;
    if (node->left_child)
    {
        node->left_child->father = node;
    }
    left_child->right_child = NULL;

    if (!node->father)
    {
        m->root = left_child;
    }
    else
    {
        if (node->father->left_child == node)
        {
            node->father->left_child = left_child;
        }
        else
        {
            node->father->right_child = left_child;
        }
    }
    left_child->right_child = node;
    left_child->father = node->father;
    node->father = left_child;
}