#ifndef PAIR_H
#define PAIR_H

#include "universal.h"

typedef xuniv_attr_t pair_attr_t;

typedef struct pair_type {
    void *fst;              /*!< pointer with the values of the first element of the pair */
    void *snd;              /*!< pointer with the values of the second element of the pair */
    pair_attr_t fst_attr;   /*!< attribute struct with the informations about the first element of the pair */
    pair_attr_t snd_attr;   /*!< attribute struct with the informations about the second element of the pair */
} pair;

#define PAIR_SIZE sizeof ( pair )

/*!
 * Initiates the pair structure offered considering the attributes for both elements
 * @param[out] p         The pair structure initalized
 * @param[in]  fst_attr  The attributes for the first element of the pair
 * @param[in]  snd_attr  The attributes for the second element of the pair
 * @return CS_SIZE if a negative or too big size has been given or CS_SUCCESS upon successfully initializing
 */
cs_codes pair_init ( pair *p, pair_attr_t fst_attr, pair_attr_t snd_attr );

/*!
 * Sets the first values of the pair structure with the value of the element offered
 * @param[out] p  The pair whose first element will be set
 * @param[in]  el The value with which the first element will be set
 * @return        CS_MEM if a memory problem occured or CS_SUCCESS upon successfuly 
 *                copying the value
 */
cs_codes pair_set_fst_value ( pair *p, void *el );

/*!
 * Sets the second values of the pair structure with the value of the element offered
 * @param[out] p  The pair whose first element will be set
 * @param[in]  el The value with which the second element will be set
 * @return        CS_MEM if a memory problem occured or CS_SUCCESS upon successfuly 
 *                copying the value
 */
cs_codes pair_set_snd_value ( pair *p, void *el );

/*!
 * Sets both values of the pair structure with the values of the elements offered
 * @param[out] p      The pair whose first element will be set
 * @param[in]  first  The value with which the first element will be set
 * @param[in]  second The value with which the second element will be set
 * @return            CS_MEM if a memory problem occured or CS_SUCCESS upon successfuly 
 *                    copying the value
 */
cs_codes pair_set_both_value ( pair *p, void *first, void *second );

/*! 
 * Selects the free function for the values inside the pair fst value 
 * @param[in] p        The pair whose first element free fct will be replaced
 * @param[in] fst_free The new free function for the first element
 */
void pair_set_fst_free ( pair *p, freer fst_free );

/*! 
 * Selects the free function for the values inside the pair snd value 
 * @param[in] p        The pair whose second element free fct will be replaced
 * @param[in] snd_free The new free function for the second element
 */
void pair_set_snd_free ( pair *p, freer snd_free );

/*! 
 * Sets the new free functions for both elements inside the pair
 * @param[out] p        Pair whose free functions will be changed
 * @param[in]  fst_free New free function for the first element
 * @param[in]  snd_free New free function for the second element 
 */
void pair_set_both_free ( pair *p, freer fst_free, freer snd_free );

/*! 
 * Sets the new print function for the first element of the pair
 * @param[out] p         Pair whose print function for the first element will be changed
 * @param[in]  fst_print New print function for the first element
 */
void pair_set_fst_print ( pair *p, printer fst_print );

/*!
 * Sets the new print function for the second element of the pair
 * @param[out] p         Pair whose print function for the second element will be changed
 * @param[in]  fst_print New print function for the second element
 */
void pair_set_snd_print ( pair *p, printer snd_print );

/*! 
 * Sets the new print functions for both elements of the pair
 * @param[out] p         Pair whose print functions will be changed
 * @param[in]  fst_print New print function for the first element
 * @param[in]  snd_print New print function for the second element
 */
void pair_set_both_print ( pair *p, printer fst_print, printer snd_print );

/*! 
 * Swaps the two given pairs
 * @param pair1, pair2 The two pairs which will be swapped
 */
cs_codes pair_swap ( pair *pair1, pair *pair2 );

/*! 
 * Gives a reference to the first element of the pair
 * @param[in] p Pair whose first element will be accessed
 */
void *pair_get_first ( pair p );

/*! 
 * Gives a reference to the second element of the pair
 * @param[in] p Pair whose second element will be accessed
 */
void *pair_get_second ( pair p );

/*! 
 * Gives access to both element of the pair
 * @param[in]  p      Pair whose elements will be accessed
 * @param[out] first  Reference to the first element
 * @param[out] second Reference to the second element
 */
void pair_get_both ( pair p, void *first, void *second );

/*! 
 * Prints the first element of the pair
 * @param[in] v_p Void reference to the pair
 */
void pair_print_first ( void *v_p );

/*!
 * Prints the second element of the pair
 * @param[in] v_p Void reference to the pair
 */
void pair_print_second ( void *v_p );

/*!
 * Prints both elements of the pair
 * @param[in] v_p Void reference to the pair
 */
void pair_print_both ( void *v_p );

/*!
 * Frees the memory that the pair uses
 * @param[in] v_p Void reference to the pair
 */
void pair_free ( void *v_p );

/*!
 * Compares the two pairs by firstly comparing the first element and if they are equal the second element
 * will be compared
 * @param[in] pair1, pair2 The two pairs which will be compared
 * @return positive number if the first pair is bigger, a negative number is pair2 is bigger or 0 if they are
 * equal, it will also return 0 if the pairs have elements of different sizes or if the compare function have not been
 * set
 */
int pair_compare ( pair pair1, pair pair2 );

#endif