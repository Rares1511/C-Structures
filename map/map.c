#include "map_internal.h"

#include <string.h>
#include <stdio.h>

cs_codes map_init(map *m, map_attr_t key_attr, map_attr_t val_attr)
{
    m->key_attr = key_attr;
    m->val_attr = val_attr;
    m->root = NULL;
    m->size = 0;

    return CS_SUCCESS;
}

cs_codes map_insert(map *m, void *key, void *val)
{
    /************************* STANDARD BST INSERT *************************/
    map_node *node = m->root, *uncle;
    map_node *insert_node = map_node_init(key, m->key_attr.size, val, m->val_attr.size);
    int comp_res;

    if (!insert_node)
    {
        return CS_MEM;
    }

    // Check for first node to be inserted in map
    if (!m->root)
    {
        m->root = insert_node;
        insert_node->color = BLACK;
        return CS_SUCCESS;
    }

    // Find location to insert new node
    while (1 > 0)
    {
        // Compare the node key and the inserted node key and save result
        comp_res = 0;
        if (!m->key_attr.comp)
        {
            comp_res = universal_compare(node->key, insert_node->key, m->key_attr.size);
        }
        else
        {
            comp_res = m->key_attr.comp(node->key, insert_node->key);
        }

        // Check if an item with the same key exists
        if (comp_res == 0)
        {
            map_node_free(m, insert_node);
            return CS_ELEM;
        }

        // Check correct direction to go in the binary tree
        if (comp_res > 0)
        {
            // Location found for insertion
            if (!node->left_child)
                break;
            node = node->left_child;
        }
        else
        {
            // Location found for insertion
            if (!node->right_child)
                break;
            node = node->right_child;
        }
    }

    // Insert the node in the correct location
    if (comp_res > 0)
    {
        node->left_child = insert_node;
    }
    else if (comp_res < 0)
    {
        node->right_child = insert_node;
    }
    else
        return CS_ELEM;

    insert_node->father = node;

    /************************* FIX RED-BLACK TREE PROPERTIES *************************/

    node = insert_node;
    while (node->father && node->father->color == RED)
    {
        // Case 1: Father is the left child of the grandfather
        if (node->father->father && node->father == node->father->father->left_child)
        {
            uncle = node->father->father->right_child;

            // Case 1.1: Uncle is RED (recoloring)
            if (uncle && uncle->color == RED)
            {
                node->father->color = BLACK;
                uncle->color = BLACK;
                node->father->father->color = RED;
                node = node->father->father;
            }
            // Case 1.2: Uncle is BLACK
            else
            {
                // Case 1.2.1: Node is the right child (left rotation)
                if (node->father->right_child == node)
                {
                    node = node->father;
                    map_rotate_left(m, node);
                }
                // Case 1.2.2: Node is the left child (right rotation)
                else
                {
                    node->father->color = BLACK;
                    node->father->father->color = RED;
                    map_rotate_right(m, node->father->father);
                }
            }
        }
        // Case 2: Father is the right child of the grandfather
        else
        {
            if (node->father->father)
                uncle = node->father->father->left_child;
            else
                uncle = NULL;

            // Case 2.1: Uncle is RED (recoloring)
            if (uncle && uncle->color == RED)
            {
                node->father->color = BLACK;
                uncle->color = BLACK;
                node->father->father->color = RED;
                node = node->father->father;
            }
            // Case 2.2: Uncle is BLACK
            else
            {
                // Case 2.2.1: Node is the left child (right rotation)
                if (node->father->left_child == node)
                {
                    node = node->father;
                    map_rotate_right(m, node);
                }
                // Case 2.2.2: Node is the right child (left rotation)
                else
                {
                    node->father->color = BLACK;
                    node->father->father->color = RED;
                    map_rotate_left(m, node->father->father);
                }
            }
        }
    }

    m->root->color = BLACK;

    return CS_SUCCESS;
}

cs_codes map_get(map m, void *key, void *value)
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
            memcpy(value, node->val, m.val_attr.size);
            return CS_SUCCESS;
        }
        else if (comp_res > 0)
        {
            node = node->left_child;
        }
        else
            node = node->right_child;
    }

    return CS_ELEM;
}

cs_codes map_delete(map *m, void *key)
{
    return CS_SUCCESS;
}

void map_print_helper(map_node node, char tab[], printer key_print, printer val_print)
{
    printf("(");
    key_print(node.key);
    printf(", ");
    val_print(node.val);
    printf(", ");
    if (node.color == RED)
        printf("R)\n");
    else
        printf("B)\n");
    strcat(tab, "  ");
    if (node.left_child)
    {
        printf("%sl: ", tab);
        map_print_helper(*node.left_child, tab, key_print, val_print);
        tab[strlen(tab) - 2] = 0;
    }
    if (node.right_child)
    {
        printf("%sr: ", tab);
        map_print_helper(*node.right_child, tab, key_print, val_print);
        tab[strlen(tab) - 2] = 0;
    }
}

void map_print(map m)
{
    map_node node = *m.root;
    char tab[1000] = "";
    map_print_helper(node, tab, m.key_attr.print, m.val_attr.print);
}

void map_free(void *v_m)
{
    map *m = (map *)v_m;
    map_node *node = m->root, *aux;

    while (node != NULL)
    {
        if (node->left_child == NULL && node->right_child == NULL)
        {
            aux = node;
            node = node->father;
            if (m->key_attr.fr)
            {
                m->key_attr.fr(aux->key);
            }
            if (m->val_attr.fr)
            {
                m->val_attr.fr(aux->val);
            }
            free(aux->val);
            free(aux->key);
            free(aux);
        }
        else if (node->left_child == NULL)
        {
            node = node->right_child;
            node->father->right_child = NULL;
        }
        else
        {
            node = node->left_child;
            node->father->left_child = NULL;
        }
    }

    m->root = NULL;
}