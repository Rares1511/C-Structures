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
    map_node *node = m->root, *father, *grandfather, *uncle;
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
            map_node_free(*m, insert_node);
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
    while (node != m->root && node->father->color == RED)
    {
        father = node->father;
        grandfather = node->father->father;

        // Case 1: Father is the left child of grandfather
        if (father == grandfather->left_child)
        {
            uncle = grandfather->right_child;
            // Case 1.1: Uncle is RED
            if (uncle != NULL && uncle->color == RED)
            {
                father->color = BLACK;
                uncle->color = BLACK;
                grandfather->color = RED;
                node = grandfather;
            }
            else
            {
                // Case 1.2: Node is the right child of father
                if (node == father->right_child)
                {
                    node = father;
                    map_rotate_left(m, node);
                    father = node->father;
                    grandfather = node->father->father;
                }
                // Case 1.3: Node is the left child of father
                father->color = BLACK;
                grandfather->color = RED;
                map_rotate_right(m, grandfather);
            }
        }
        // Case 2: Father is the right child of grandfather
        else
        {
            uncle = grandfather->left_child;
            // Case 2.1: Uncle is RED
            if (uncle != NULL && uncle->color == RED)
            {
                father->color = BLACK;
                uncle->color = BLACK;
                grandfather->color = RED;
                node = grandfather;
            }
            else
            {
                // Case 2.2: Node is the left child of father
                if (node == father->left_child)
                {
                    node = father;
                    map_rotate_right(m, node);
                    father = node->father;
                    grandfather = node->father->father;
                }
                // Case 2.3: Node is the right child of father
                father->color = BLACK;
                grandfather->color = RED;
                map_rotate_left(m, grandfather);
            }
        }
    }

    m->root->color = BLACK;

    return CS_SUCCESS;
}

cs_codes map_get(map m, void *key, void *value)
{
    map_node *node = map_node_find(m, key);
    if (node == NULL)
    {
        return CS_ELEM;
    }
    memcpy(value, node->val, m.val_attr.size);
    return CS_SUCCESS;
}

cs_codes map_delete(map *m, void *key)
{
    /************************* STANDARD BST DELETE *************************/
    map_node *delete_node = map_node_find(*m, key);
    if (delete_node->left_child != NULL && delete_node->right_child != NULL)
    {
        map_node *successor = delete_node->right_child;
        while (successor->left_child != NULL)
        {
            successor = successor->left_child;
        }
        delete_node->key = successor->key;
        delete_node->val = successor->val;
        delete_node = successor;
    }

    map_node *child = (delete_node->right_child != NULL) ? delete_node->right_child : delete_node->left_child;

    /************************* FIXUP RED BLACK TREE PROPERTIES *************************/

    // Case 1: Either delete node or his replacement child is RED
    if (delete_node->color == RED || (child != NULL && child->color == RED))
    {
        child->color = BLACK;
        if (delete_node->father != NULL)
        {
            if (delete_node->father->left_child == delete_node)
            {
                delete_node->father->left_child = child;
            }
            else
            {
                delete_node->father->right_child = child;
            }
            child->father = delete_node->father;
        }
        else
        {
            m->root = child;
        }
    }
    // Case 2: Both delete node and his replacement child are BLACK and node is not root
    else if (delete_node->father != NULL)
    {
        if (child == NULL)
        {
            child = delete_node;
        }
        map_node *father = delete_node->father;
        map_node *brother = (father != NULL && father->left_child == delete_node) ? father->right_child : father->left_child;

        // Case 2.1: Brother is BLACK and at least one of his children is RED
        if (brother != NULL && ((brother->left_child != NULL && brother->left_child->color == RED) ||
                                (brother->right_child != NULL && brother->right_child->color == RED)))
        {
            // Case 2.1.1: Brother is the left child of his father and his left child is the red one
            if (brother == father->left_child && brother->left_child != NULL && brother->left_child->color == RED)
            {
                brother->left_child->color = BLACK;
                brother->color = father->color;
                father->color = BLACK;
                map_rotate_right(m, father);
            }
            // Case 2.1.2: Brother is the left child of his father and his right child is the red one
            else if (brother == father->left_child && brother->right_child != NULL && brother->right_child->color == RED)
            {
                brother->right_child->color = BLACK;
                brother->color = RED;
                map_rotate_left(m, brother);
                map_rotate_right(m, father);
            }
            // Case 2.1.3: Brother is the right child of his father and his right child is the red one
            else if (brother == father->right_child && brother->right_child != NULL && brother->right_child->color == RED)
            {
                brother->right_child->color = BLACK;
                brother->color = father->color;
                father->color = BLACK;
                map_rotate_left(m, father);
            }
            // Case 2.1.4: Brother is the right child of his father and his left child is the red one
            else if (brother == father->right_child && brother->left_child != NULL && brother->left_child->color == RED)
            {
                brother->left_child->color = BLACK;
                brother->color = RED;
                map_rotate_right(m, brother);
                map_rotate_left(m, father);
            }
        }
        // Case 2.2: Brother is BLACK and both his children are BLACK
        else if (brother != NULL && ((brother->left_child == NULL || brother->left_child->color == BLACK) &&
                                     (brother->right_child == NULL || brother->right_child->color == BLACK)))
        {
            brother->color = RED;
            if (father->color == RED)
            {
                father->color = BLACK;
            }
            else
            {
                map_node *uncle = (father->father != NULL && father->father->left_child == father) ? father->father->right_child : father->father->left_child;
                if (uncle != NULL && uncle->color == RED)
                {
                    uncle->color = BLACK;
                    father->color = BLACK;
                    father->father->color = RED;
                    delete_node = father->father;
                }
                else
                {
                    map_node *grandfather = father->father;
                    if (grandfather != NULL)
                    {
                        if (grandfather->left_child == father)
                        {
                            grandfather->left_child = NULL;
                        }
                        else
                        {
                            grandfather->right_child = NULL;
                        }
                    }
                    map_node_free(*m, father);
                    father = NULL;
                    map_delete(m, grandfather->key);
                }
            }
        }
    }
    // Case 3: Delete node is root
    else
    {
        m->root = NULL;
    }

    // Free memory of delete node
    map_node_free(*m, delete_node);
    delete_node = NULL;

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
    if (m.root == NULL)
    {
        return;
    }
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