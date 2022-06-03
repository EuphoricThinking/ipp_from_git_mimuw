/** @file
 * Interface of the class storing phone numbers forwards
 *
 * @authors Marcin Peczarski <marpe@mimuw.edu.pl>,
 *          Agata Momot <a.momot4@student.uw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef __PHONE_FORWARD_H__
#define __PHONE_FORWARD_H__

#include <stdbool.h>
#include <stddef.h>

/**
 * This is the structure storing phone numbers forwards.
 */
struct PhoneForward;
typedef struct PhoneForward PhoneForward;  ///< Stores phone forwards

/**
 * This is the structure storing the sequence of phone numbers.
 */
struct PhoneNumbers;
typedef struct PhoneNumbers PhoneNumbers;  ///< Stores phone numbers

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
 *  Adds a forwarding of the all numbers beginning with the prefix @p num1
 *  to the numbers, whose given prefix has been correspondingly substituted
 *  with @p num2. Each number is its own prefix. If a redirection with the same
 *  @p num1 parameter has been added before, this redirection is replaced.
 *
 *  Phone numbers forwarding relation is not transitive.
 *
 * @param[in, out] pfd - a pointer to the structure storing number redirections;
 * @param[in] num1 - a pointer to the string representing the prefix of
 *                   the redirected numbers;
 * @param[in] num2 - a pointer to the string representing the prefix of
 *                   the numbers to whom the redirection is performed.
 * @return The value of @p true, if the redirection has been added.
 *         The value of @p false, if an error occurred, i.e. the given prefix
 *         does not represent a number, the passed numbers are identical
 *         or enough memory could not have been allocated.
 */
bool phfwdAdd(PhoneForward *pfd, char const *num1, char const *num2);

/** @brief Removes redirections.
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
 * which should be freed using the function @ref phnumDelete.
 *
 * @param[in] pf - a pointer to the structure storing number redirections.
 * @param[in] num - a pointer to the string representing the number.
 * @return A pointer to the structure storing the sequence of numbers
 *         or NULL in case of memory allocation failure.
 */
PhoneNumbers * phfwdGet(PhoneForward const *pf, char const *num);

/** @brief Assigns a redirection to the given number.
 * Assigns the following sequence of numbers to the given number: if there
 * exists a number @p x such that its prefix can be redirected to the
 * prefix of @p num, then the number @p x belongs to the set returned from
 * @ref phfwdReverse with the number @p num. @ref phfwdGet may not return @p num
 * with passed number @p x, since @ref phfwdReverse checks all possible prefixes
 * and includes @p num, not limiting the set to the longest possible.
 * The resulting numbers are sorted lexicographically and they must not repeat
 * in the returned set. If the given string does not represent a number
 * the result is an empty sequence. Allocates a structure @p PhoneNumbers,
 * which should be freed using the function @ref phnumDelete.
 *
 * @param[in] pf - a pointer to the structure storing number redirections;
 * @param[in] num - a pointer to the string representing a number.
 * @return A pointer to the structure storing the sequence of numbers
 *         or NULL in case of memory allocation failure.
 */
PhoneNumbers * phfwdReverse(PhoneForward const *pf, char const *num);

/** @brief Removes a structure.
 * Removes a structure pointed to by @p pnum. It does nothing if the pointer
 * is NULL.
 *
 * @param[in] pnum - a pointer to the stucture to be removed.
 */
void phnumDelete(PhoneNumbers *pnum);

/** @brief Provides a number.
 * Provides a pointer to the string representing a number. Strings are indexed
 * from zero.
 *
 * @param[in] pnum - a pointer to the structure storing the sequence of numbers;
 * @param[in] idx - an index of the phone number.
 * @return A pointer to the string representing a phone number.
 *         Tha value of NULL if @p pnum is NULL or the index is too large.
 */
char const * phnumGet(PhoneNumbers const *pnum, size_t idx);

#endif /* __PHONE_FORWARD_H__ */
