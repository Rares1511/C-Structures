#ifndef __CS_VECTOR_H__
#define __CS_VECTOR_H__

#include <cs/universal.h>

typedef univ_attr_t vector_attr_t;

typedef struct vector {
    void *vec;          /*!< size of the datatype */
    int cap;            /*!< current maximum capacity of the vector */
    metadata_t *meta;   /*!< metadata of the vector */
    vector_attr_t attr; /*!< attributes of the elements inside the vector */
} vector;

#define VECTOR_SIZE sizeof(vector)

/*!
 * Checks if the vector is empty
 * @param[in] vec  The vector to be checked
 * @return 1 if the vector is empty, 0 otherwise
 */
static inline int vector_empty(vector vec) { return vec.meta->size == 0; };

/*!
 * Returns the current size of the vector
 * @param[in] vec  The vector whose size will be returned
 * @return The size of the vector
 */
static inline int vector_size(vector vec) { return vec.meta->size; };

/*!
 * Initializes the given variable with the correct vector structure datatype
 * @param[in]  attr       Attributes decribing the elements that the vector will contain
 * @param[out] v          Pointer to the vector structure that will be initialized
 * @return CS_MEM if a memory problem ocurred or CS_SUCCESS upon a successful initalization
 */
cs_codes vector_init(vector *v, vector_attr_t attr);

/*!
 * Inserts the element at the given position in the offered vector
 * @param[out] vec  Vector in which the element will be inserted
 * @param[in]  el   The value of the element which will be inserted
 * @param[in]  pos  The position at which the element will be inserted at
 * @return CS_MEM if a memory problem ocurred or CS_SUCCESS upon a successful initalization
 */
cs_codes vector_insert_at(vector *vec, const void *el, int pos);

/*!
 * Pushes the element at the back of the vector
 * @param[out] vec  Vector in which the element will be inserted
 * @param[in]  el   The value of the element which will be inserted
 * @return CS_MEM if a memory problem ocurred or CS_SUCCESS upon a successful initalization
 */
static inline cs_codes vector_push_back(vector *vec, const void *el) { return vector_insert_at(vec, el, vector_size(*vec)); }

/*!
 * Erase the element at the position offered
 * @param[out] vec  Vector in which the element will be deleted
 * @param[in]  pos  Position at which the element will be deleted
 * @return CS_EMPTY if the vector is empty, CS_POS if given an incorrect position or
 * CS_SUCCESS upon a successful deletion
 */
cs_codes vector_erase(vector *vec, int pos);

/*!
 * Pops the last element of the vector
 * @param[out] vec  Vector from which the last element will be deleted
 * @return CS_EMPTY if the vector is empty or CS_SUCCESS upon a successful deletion
 */
static inline cs_codes vector_pop_back(vector *vec) { return vector_erase(vec, vector_size(*vec) - 1); }

/*!
 * Replaces the value at the position offered with the new value given
 * @param[out] vec  Vector in which the element will be replaced
 * @param[in]  el   The new value for the element
 * @param[in]  pos  The position for the soon to be changed element
 * @return CS_EMPTY if the vector is empty, CS_POS if given an incorrect position or
 * CS_SUCCESS upon a successful deletion
 */
cs_codes vector_replace(vector *vec, const void *el, int pos);

/*!
 * Find the element given in the vector
 * @param[in] vec  The vector in which the search will be done
 * @param[in] el   The element searched
 * @return The position of the element, CS_COMP if no compare function has been assigned
 * or CS_ELEM if it's not in the vector
 */
int vector_find(vector vec, const void *el);

/*!
 * Returns the reference at the position given
 * @param[in] vec  Vector used to be given the reference
 * @param[in] pos  Position for the reference
 */
void *vector_at(vector vec, int pos);

/*!
 * Counts how many times the given element appears in the vector
 * @param[in] vec  The vector in which the count will be done
 * @param[in] el   The element to be counted
 * @return The number of times the element appears in the vector, CS_COMP if no compare function
 * has been assigned
 */
int vector_count(vector vec, const void *el);

/*!
 * Sets the new attributes for the vector
 * @param[out] vec  Vector whose attributes will be changed
 * @param[in]  attr The new attributes
 */
static inline void vector_set_attr(vector *vec, vector_attr_t attr) { 
    CS_RETURN_IF(NULL == vec);
    vec->attr = attr; 
}

/*!
 * Sets the new function to free the elements inside the vector
 * @param[out] vec Vector which will have its free function changed
 * @param[in]  fr  The new free function
 */
static inline void vector_set_free(vector *vec, freer fr) { 
    CS_RETURN_IF(NULL == vec);
    vec->attr.fr = fr; 
}

/*!
 * Sets the new function to print the elements inside the vector
 * @param[out] vec    Vector which will have its print function changed
 * @param[in]  print  The new print function
 */
static inline void vector_set_print(vector *vec, printer print) { 
    CS_RETURN_IF(NULL == vec);
    vec->attr.print = print; 
}

/*!
 * Sets a new copy function used in copying the elements in the vector
 * @param[out] vec Vector whose copy function will be set or changed
 * @param[in]  cp  The new copy function
 */
static inline void vector_set_copy(vector *vec, deepcopy cp) { 
    CS_RETURN_IF(NULL == vec);
    vec->attr.copy = cp; 
}

/*!
 * Sets the new function to compare the elements inside the vector
 * @param[out] vec   Vector which will have its compare function changed
 * @param[in]  comp  The new compare function
 */
static inline void vector_set_comp(vector *vec, comparer comp) { 
    CS_RETURN_IF(NULL == vec);
    vec->attr.comp = comp; 
}

/*!
 * Empties the vector and frees any memory that was used in any of its elements
 * @param[out] vec Vector which will have its contet emptied
 */
void vector_clear(vector *vec);

/*!
 * Swaps the two given vector structures.
 * @param vec1,vec2 Vector which will be swapped.
 */
void vector_swap(vector *vec1, vector *vec2);

/*!
 * Sorts the vector if the compare function of the attribute has been set
 * @param[out] vec Vector which will be sorted
 */
void vector_sort(vector *vec);

/*!
 * Prints the content of the vector if the print function of the attribute has been set
 * @param[in] v_vec Void pointer to the vector structure
 */
void vector_print(FILE *stream, const void *v_vec);

/*!
 * Frees the memory that the vector uses and the memory of the elements if the free
 * function of the attribute has been set
 * @param[in] v_vec Void pointer to the vector structure
 */
void vector_free(void *v_vec);

#endif
