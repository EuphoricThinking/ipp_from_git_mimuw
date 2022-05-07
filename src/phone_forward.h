/** @file
 * Interfejs klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Marcin Peczarski <marpe@mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef __PHONE_FORWARD_H__
#define __PHONE_FORWARD_H__

#include <stdbool.h>
#include <stddef.h>

/**
 * To jest struktura przechowująca przekierowania numerów telefonów.
 */
struct PhoneForward;
typedef struct PhoneForward PhoneForward;  ///< Przechowuje dane o przekierowaniach

/**
 * To jest struktura przechowująca ciąg numerów telefonów.
 */
struct PhoneNumbers;
typedef struct PhoneNumbers PhoneNumbers;  ///< Final name

/** @brief Creates a new structure.
 * Creates a new structure which does not contain any redirections.
 *
 * @return  A pointer to the created structure or NULL in case of memory
 *          allocation failure.
 */
PhoneForward * phfwdNew(void);

/** @brief Removes a structure.
 * Removes a structure pointed to by @p pf. It does nothing if this pointer
 * is NULL.
 *
 * @param[in] pf - a pointer to the structure to be deleted.
 */
void phfwdDelete(PhoneForward *pf);

/** @brief Adds a redirection.
 *  Adds a forwarding of the all numbers beginnign with the prefix @p num1
 *  to the numbers, whose given prefix has been correspondingly substituted
 *  with @p num2. Each number is its own prefix. If a redirection with the same
 *  @p num1 parameter has been added before, this redirection is replaced.
 *
 *  Forwarding relation is not transitive.
 *
 * @param[in, out] pfd - a pointer to the structure storing number redirections;
 * @param[in] num1 - a pointer to the string representing the prefix of
 *                   the redirected numbers;
 * @param[in] num2 - a pointer to the string representing the prefix of
 *                   the numbers to whom the redirection is performed.
 * @return The value @p true, if the redirection has been added.
 *         The value @p false, if an error ocurred, i.e. the given prefix
 *         does not represent a number, both passed numbers are identical
 *         or the memory could not have been allocated.
 */
bool phfwdAdd(PhoneForward *pfd, char const *num1, char const *num2);

/** @brief Removes a redirection.
 * Removes all redirections, in which the parameter @p num is a prefix
 * of the parameter @p num1 used in redirection inclusion (phfwdAdd). If there
 * are no such redirections or the string does not represent a number,
 * the function does nothing.
 *
 * @param[in, out] pf - a pointer to a structure storing number redirections;
 * @param[in] num - a pointer to the string representing the prefix of numbers.
 */
void phfwdRemove(PhoneForward *pf, char const *num);

/** @brief Assigns the number redirection.
 * Assigns the redirection to the given number. Looks for the longest common
 * prefix. The result is the sequence containing at most one number.
 * If the given number has not been redirected, the result is the sequence
 * containing this one number. If the given string does not represent a number,
 * the result is an empty sequence. Allocates the structure @p PhoneNumbers,
 * which should be freed with the function @ref phnumDelete.
 *
 * @param[in] pf - a pointer to the structure storing number redirections.
 * @param[in] num - a pointer to the string representing the number.
 * @return A pointer to the structure storing the sequence of numbers
 *         or NULL in case of memory allocation failure.
 */
PhoneNumbers * phfwdGet(PhoneForward const *pf, char const *num);

/** @brief Wyznacza przekierowania na dany numer.
 * Wyznacza następujący ciąg numerów: jeśli istnieje numer @p x, taki że wynik
 * wywołania @p phfwdGet z numerem @p x zawiera numer @p num, to numer @p x
 * należy do wyniku wywołania @ref phfwdReverse z numerem @p num. Dodatkowo ciąg
 * wynikowy zawsze zawiera też numer @p num. Wynikowe numery są posortowane
 * leksykograficznie i nie mogą się powtarzać. Jeśli podany napis nie
 * reprezentuje numeru, wynikiem jest pusty ciąg. Alokuje strukturę
 * @p PhoneNumbers, która musi być zwolniona za pomocą funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci.
 */
PhoneNumbers * phfwdReverse(PhoneForward const *pf, char const *num);

/** @brief Removes a structure.
 * Removes a structure pointed to by @p num. It does nothing if the pointer
 * is NULL.
 *
 * @param[in] pnum - a pointer to the stucture to be removed.
 */
void phnumDelete(PhoneNumbers *pnum);

/** @brief Provides a number.
 * Provides a pointer to the string representing a number. Strings (char
 * arrays) are indexed from zero.
 *
 * @param[in] pnum - a pointer to the structure storing a sequence of numbers;
 * @param[in] idx - an index of the phone number.
 * @return A pointer to the string representing a phone number.
 *         Tha value of NULL if @p pnum is NULL or the index is too large.
 */
char const * phnumGet(PhoneNumbers const *pnum, size_t idx);

#endif /* __PHONE_FORWARD_H__ */
