/** @file
 * Implementation of the class storing the number forwardings
 *
 * @author Agata Momot <a.momot4@student.uw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

/**
 * A macro which enables usage of strdup(char * s), which isn't included
 * in C standard but is a POSIX extension.
 */
#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <string.h>
#include "phone_forward.h"
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

/**
 * The size of the alphabet of the telephone numbers, counting
 * ten characters from 0 to 9 inclusively
 */
#define ALPHABET_SIZE   10

struct ForwardedNode;

/** @struct InitialNode
 * @brief A struct describing nodes storing information for prefixes
 *      of the numbers which are supposed to be redirected.
 * @var InitialNode::ancestor
 *      A parent node of the current node.
 * @var InitialNode::forwardingNode
 *      A pointer to the node which stores the information about redirection
 * @var InitialNode::alphabet
 *      An array storing pointers to children nodes which are sorted according
 *      to the label of the edge leading to the subsequent node. The value
 *      of an index in range from 0 to 9 unambiguously labels the edge.
 * @var InitialNode::depth
 *      Depth of the node in a tree, related to the accurate length
 *      of the redirected prefix
 * @var InitialNode::isForwarded
 *      A flag indicating whether the given node is a terminal node for
 *      a prefix which should be redirected, therefore it is the first node
 *      after the last edge labeled with the last digit of a number
 *      and contains a pointer to corresponding node, representing the final
 *      redirection.
 * @var InitialNode::indexForward
 *      If the given node is a terminal node for a redirected prefix,
 *      indexForward stores the index in the array of terminal nodes
 *      for forwarded prefixes contained in a node, which is responsible
 *      for the final redirection
 * @var InitialNode::filledEdges
 *      The number of edges leaving the node, equivalent of the number
 *      of the children and the number of indices in
 *      \link InitialNode::alphabet alphabet \endlink which do not indicate NULL.
 * @var InitialNode::edgeLeadingTo
 *      The label of the edge leading to the current node, equivalent to
 *      the index value in \link InitialNode::alphabet alphabet array \endlink
 *      of the parent.
 * @var InitialNode::lastChecked
 *      The index of the last checked element in \link InitialNode::alphabet
 *      alphabet array \endlink, used in iterative tree traversal.
 * @var InitialNode::initialPrefix
 *      The string associated with the prefix terminating in the current node,
 *      saved for speed-up of redirection lookup and for the future
 *      implementation of the ReverseFunction
 */
typedef struct InitialNode {
    struct InitialNode* ancestor;
    struct ForwardedNode* forwardingNode;
    struct InitialNode* alphabet[ALPHABET_SIZE];
    uint64_t depth;
    uint8_t isForwarded;
    uint64_t indexForward;
    int filledEdges;
    int edgeLeadingTo;
    int lastChecked;
    char* initialPrefix;
} InitialNode;  ///< Compound struct for storing data about redirected prefixes

/** @struct ForwardedNode
 *  @brief  A struct representing a node storing information about the final
 *          redirection.
 *  @var ForwardedNode::ancestor
 *          A parental node of the current node.
 *  @var ForwardedNode::alphabet
 *          An array storing pointers to children nodes which are sorted
 *          according to the label of the edge leading to the subsequent node.
 *          The value of an index in range from 0 to 9 unambiguously labels
 *          the edge.
 *  @var ForwardedNode::isForwarding
 *          A flag indicating whether the given node is a terminal node for
 *          a prefix describing the final substitution, therefore it is
 *          the first node after the last edge labeled with the last digit of
 *          a number and contains an array of pointers to the nodes which
 *          are terminal nodes for the prefixes redirected to the given
 *          forwarding prefix.
 *  @var ForwardedNode::numForwardedNodes
 *          An index of the last slot available for storing a redirected node.
 *  @var ForwardedNode::sumForwarded
 *          The sum of the nodes currently redirected to the given node.
 *          Enables exclusion of the NULL slots, freed after redirection removal.
 *  @var ForwardedNode::depth
 *          Depth of the node in a tree, related to the accurate length
 *          of the redirected prefix
 *  @var ForwardedNode::numSlotsForNodes
 *          The number of available slots for storing the pointers to
 *          the redirected nodes, regarding the size of the memory allocated
 *          for \link ForwardedNode::forwardedNodes redirected nodes array
 *          \endlink - includes both non-occupied and assigned slots.
 *  @var ForwardedNode::edgeLeadingTo
 *          The label of the edge leading to the current node, equivalent to
 *          the index value in \link ForwardedNode::alphabet alphabet array
 *          \endlink of the parent.
 *  @var ForwardedNode::lastChecked
 *          The index of the last checked element in \link
 *          ForwardedNode::alphabet alphabet array \endlink, used in iterative
 *          tree traversal.
 *  @var ForwardedNode::forwardedNodes
 *          If the given node is terminal for the given final prefix,
 *          it stores the array of pointers to InitialNode nodes,
 *          which are terminal for the prefixes supposed to be redirected.
 *          If the redirection is removed, NULL is placed at the index
 *          indicating the pointer to a given node.
 *  @var ForwardedNode::forwardedPrefix
 *          The string associated with the prefix terminating in the current
 *          node, saved for speed-up of redirection lookup and for the future
 *          implementation of the ReverseFunction
 *  @var ForwardedNode::filledEdges
 *          The number of edges leaving the node, equivalent of the number
 *          of the children and the number of indices in
 *          \link ForwardedNode::alphabet alphabet \endlink which do not
 *          indicate NULL.
 */
typedef struct ForwardedNode {
    struct ForwardedNode* ancestor;
    struct ForwardedNode* alphabet[ALPHABET_SIZE];
    uint8_t isForwarding;
    uint64_t numForwardedNodes;
    uint64_t sumForwarded;
    uint64_t depth;
    uint64_t numSlotsForNodes;
    int edgeLeadingTo;
    int lastChecked;
    InitialNode** forwardedNodes;
    char* forwardedPrefix;
    uint64_t filledEdges;
} ForwardedNode; ///< Compound struct for storing data about forwarding prefixes

/** @struct PhoneForward
 *  @brief A storage for root nodes for trees responsible for storing
 *  information about forwarded and forwarding prefixes.
 *  @var PhoneForward::forwardedRoot
 *      A root of the tree responsible for storing data related to the final
 *      prefixes.
 *  @var PhoneForward::initialRoot
 *      A root of the tree responsible for storing data related to
 *      the redirected prefixes.
 */
typedef struct PhoneForward {
    ForwardedNode* forwardedRoot;
    InitialNode* initialRoot;
} PhoneForward;  ///< Final struct for storing data about forwarding

/** @struct PhoneNumbers
 * @brief Struct for storing the full phone numbers retrieved from PhoneForward.
 * @var PhoneNumbers::numbers
 *      An array storing full phone numbers
 * @var PhoneNumbers::slots
 *      Number of available slots in \link PhoneNumbers::numbers number array
 *      \endlink, resulting from memory allocation.
 * @var PhoneNumbers::lastAvailableIndex
 *      The last non-occupied index in \link PhoneNumbers::numbers number array
 *      \endlink
 */
typedef struct PhoneNumbers {
    char** numbers;
    uint64_t slots;
    uint64_t lastAvailableIndex;
} PhoneNumbers;  ///< Final struct for storing full numbers

/** @brief Sets a flag bit.
 *  Sets a flag bit in order to indicate that the given node participates
 *  in forwarding.
 *
 *  Bitwise is used instead of simple value assigned in order to prepare
 *  the function for probable future extensions (incorporation of the new flags,
 *  accordingly to the needs of the future project steps; number of shifted
 *  bits, defined for a particular flag, should be added then to the arguments).
 *
 * @param[in, out] flag - a pointer to isForwarding or isForwarded struct field.
 */
static void setBitForward(uint8_t * flag) {
    *flag |= (uint8_t) 1;
}

/** @brief Clears a flag bit.
 *  Clears a flag bit in order to indicate that the node which has participated
 *  in forwarding is not participating in redirection at the moment of changing
 *  the flag.
 *
 *  Bitwise is used instead of simple value assigned in order to prepare
 *  the function for probable future extensions (incorporation of the new flags,
 *  accordingly to the needs of the future project steps; number of shifted
 *  bits, defined for a particular flag, should be added then to the arguments).
 *
 * @param[in, out] flag - a pointer to isForwarding or isForwarded struct field.
 */
static void clearBitForward(uint8_t * flag) {
    *flag &= ~(uint8_t) 1;
}

/** @brief Checks flag bits.
 * Used for testing whether the given node participates in forwarding.
 *
 *  Bitwise is used instead of simple value assigned in order to prepare
 *  the function for probable future extensions (incorporation of the new flags,
 *  accordingly to the needs of the future project steps; number of shifted
 *  bits, defined for a particular flag, should be added then to the arguments).
 *
 * @param[in] flag  - the value of a isForwarding or isForwarded struct field.
 *
 * @return True if the node participates in forwarding, false otherwise.
 */
static bool isForwardSet(uint8_t flag) {
    return (flag & (uint8_t) 1) != 0;
}

/** @brief Creates and initializes a node.
 *  Creates and initializes the node responsible for storing the information
 *  about the prefixes supposed to be redirected.
 *
 * @param[in] ancestor - a parental node of the initialized node.
 * @param[in] depth - depth of the level at which the node is supposed to be
 *                    assigned to the tree
 * @param edgeLeadingTo - the label of the edge leading to the initialized node
 * @return
 */
static InitialNode * initInitialNode(InitialNode* ancestor, uint64_t depth,
                              int edgeLeadingTo) {
    InitialNode * result = malloc(sizeof (InitialNode));
    if (!result) {
        return NULL;
    }

    result->ancestor = ancestor;
    result->forwardingNode = NULL;
    result->depth = depth;
    result->isForwarded = 0;
    result->indexForward = 0;
    result->filledEdges = 0;
    result->lastChecked = 0;
    result->initialPrefix = NULL;
    result->edgeLeadingTo = edgeLeadingTo;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        result->alphabet[i] = NULL;
    }

    return result;
}

/** @brief Creates and initializes a node.
 *  Creates and initializes the node responsible for storing the information
 *  about the prefixes supposed to represent the final redirection.
 *
 * @param[in] ancestor - a parental node of the initialized node.
 * @param[in] depth - depth of the level at which the node is supposed to be
 *                    assigned to the tree
 * @param edgeLeadingTo - the label of the edge leading to the initialized node
 * @return
 */
static ForwardedNode * initForwardedNode(ForwardedNode* ancestor, uint64_t depth,
                                  int edgeLeadingTo) {
    ForwardedNode * result = malloc(sizeof (ForwardedNode));
    if (!result) {
        return NULL;
    }

    result->isForwarding = 0;

    result->ancestor = ancestor;
    result->forwardedPrefix = NULL;
    result->filledEdges = 0;
    result->depth = depth;
    result->sumForwarded = 0;
    result->edgeLeadingTo = edgeLeadingTo;
    result->lastChecked = 0;

    result->numForwardedNodes = 0;
    result->numSlotsForNodes = 0;
    result->forwardedNodes = NULL;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        result->alphabet[i] = NULL;
    }

    return result;
};

PhoneForward * phfwdNew(void) {
    PhoneForward * result = malloc(sizeof(PhoneForward));
    if (!result) {
        return NULL;
    }

    result->forwardedRoot = initForwardedNode(NULL, 0, -1);
    if (!result->forwardedRoot) {
        return NULL;
    }

    result->initialRoot = initInitialNode(NULL, 0, -1);
    if (!result->initialRoot) {
        return NULL;
    }

    return result;
}

/** @brief Adds a node to an array.
 *  Adds a node storing information about a redirected prefix to the array
 *  of the nodes indicating terminal nodes for redirected prefixes,
 *  which is assigned to the specific node to whom the prefixes are redirected.
 *
 * @param[in, out] toBeForwarded - a terminal node for a redirected prefix
 * @param[in, out] finalForward - a terminal node for the final redirection.
 *
 * @return False in case of memory allocation failure, true otherwise.
 */
static bool addForwardedNode(InitialNode* toBeForwarded, ForwardedNode* finalForward) {
    uint64_t * slots = &(finalForward->numSlotsForNodes);
    uint64_t * numNodes = &(finalForward->numForwardedNodes);

    if (*slots <= *numNodes) {
        uint64_t newSlots = (*slots)*2 + 1;
        InitialNode ** newNodeArray = realloc(finalForward->forwardedNodes,
                                              newSlots * sizeof (InitialNode*));

        if (!newNodeArray) {
            return false;
        }

        finalForward->forwardedNodes = newNodeArray;
        *slots = newSlots;
    }

    finalForward->forwardedNodes[(*numNodes)] = toBeForwarded;
    toBeForwarded->indexForward = (*numNodes)++;
    toBeForwarded->forwardingNode = finalForward;
    (finalForward->sumForwarded)++;

    return true;
}

/** @brief Adds prefix to a node and sets a flag.
 * Adds a final prefix to the terminal node corresponding to the given prefix
 * and set isForwarding flag in order to indicate that the passed node
 * participates in redirection
 *
 * @param[in, out] finalForward - the terminal node corresponding to the final
 *                                prefix
 * @param[in] prefix - the final prefix associated with the given node.
 *
 * @return False in case of memory allocation failure, true otherwise.
 */
static bool addPrefixForwardAndSetForward(ForwardedNode* finalForward, const char* prefix) {
    // If it hasn't been previously set - the value of the string is the same
    if (!isForwardSet(finalForward->isForwarding)) {
        finalForward->forwardedPrefix = strdup(prefix);

        if (!(finalForward->forwardedPrefix)) {
            return false;
        }

        setBitForward(&(finalForward->isForwarding));
    }

    return true;
}

/** @brief Adds prefix to a node and sets a flag.
 *  Adds a redirected prefix to the terminal node corresponding to the given
 *  prefix and set isForwarded flag in order to indicate that the passed node
 *  participates in redirection.
 *
 *  @param[in, out] init - the terminal node corresponding to the redirected
 *                         prefix
 *
 *  @param[in] prefix - the final prefix associated with the given node.
 *
 *  @return False in case of memory allocation failure, true otherwise.
 */
static bool addPrefixInitialAndSetForward(InitialNode * init, const char* prefix) {
    // Temporary array enables saving the previous content
    char* copiedPrefix = strdup(prefix);

    if (!(copiedPrefix)) {
        return false;
    }

    if (isForwardSet(init->isForwarded)) {
        free(init->initialPrefix);
    }
    else {
        setBitForward(&(init->isForwarded));
    }

    init->initialPrefix = copiedPrefix;

    return true;
}

/** @brief Checks the length of a string.
 *  Checks the length of a string and validates the correctness of the passed
 *  argument.
 *
 * @param[in] number - char * array containing the phone number
 *
 * @return  The number of digit characters in a string or 0 if the string
 *          is empty, is NULL or contains characters which are not digits.
 */
static size_t checkLength(const char * number) {
    if (!number) {
        return 0;
    }

    size_t index = 0;

    while (isdigit(number[index]) && number[index] != '\0') index++;

    if (number[index] != '\0') {
        return 0;
    }
    else {
        return index;
    }
}

/** @brief Converts char to int
 * Converts char to its integer value of the number it represents.
 *
 * @param[in] c - char to convert
 * @return Integer value of the number the passed char represents.
 */
static int getIndex(char c) {
    return c - '0';
}

bool phfwdAdd(PhoneForward *pfd, char const *num1, char const *num2) {
    if (!pfd) {
        return false;
    }

    size_t len1 = checkLength(num1);
    if (len1 == 0) {
        return false;
    }

    size_t len2 = checkLength(num2);
    if (len2 == 0) {
        return false;
    }

    if (strcmp(num1, num2) == 0) {
        return false;
    }

    uint64_t depth = 0;
    InitialNode * currentInitial = pfd->initialRoot;
    int digit;
    // Extending the path for redirected prefix
    while (depth < len1) {
        digit = getIndex(num1[depth]);
        
        if (!(currentInitial->alphabet[digit])) {
            InitialNode * newNode = initInitialNode(currentInitial, ++depth, digit);
            if (!newNode) {
                return false;
            }

            currentInitial->alphabet[digit] = newNode;
            currentInitial->filledEdges++;

            currentInitial = newNode;
        }
        else {
            currentInitial = currentInitial->alphabet[digit];
            depth++;
        }
    }

    depth = 0;
    ForwardedNode * currentForward = pfd->forwardedRoot;

    //Extending the path for the final prefix
    while (depth < len2) {
        digit = getIndex(num2[depth]);
        
        if (!(currentForward->alphabet[digit])) {
            ForwardedNode * newNode = initForwardedNode(currentForward, ++depth, digit);
            if (!newNode) {
                return false;
            }

            currentForward->alphabet[digit] = newNode;
            currentForward->filledEdges++;
            
            currentForward = newNode;
        }
        else {
            currentForward = currentForward->alphabet[digit];
            depth++;
        }
    }

    if (!addForwardedNode(currentInitial, currentForward)) {
        return false;
    }

    if (!(addPrefixForwardAndSetForward(currentForward, num2))
        || !(addPrefixInitialAndSetForward(currentInitial, num1))) {
            return false;
    }

    return true;
}

/** @brief Removes a node.
 *  Removes a node responsible for storing information about the final prefix
 *  and updates information about the children in the parental node.
 *
 * @param[in, out] toDelete - a node to be removed from a tree.
 */
static void removeForwardedNode(ForwardedNode * toDelete) {
    if (toDelete) {
        ForwardedNode * ancestor = toDelete->ancestor;

        if (ancestor) {
            ancestor->alphabet[toDelete->edgeLeadingTo] = NULL;
            (ancestor->filledEdges)--;
        }

        free(toDelete->forwardedPrefix);
        free(toDelete->forwardedNodes);
        free(toDelete);    
    }
}

/** @brief Removes unnecessary nodes.
 *  Removes unnecessary nodes from a tree: nodes which are not on the path
 *  ending with a node regarded as terminal for the given prefix.
 *
 * @param currentForward - a node responsible for storing data about
 *                         the final prefix, which starts the chain of nodes
 *                         removal.
 */
static void removeStumpsForwardedNode(ForwardedNode * currentForward) {
    while (currentForward && currentForward->filledEdges == 0
            && currentForward->sumForwarded == 0) {
                ForwardedNode * currentAncestor = currentForward->ancestor;

                if (currentAncestor) {
                    removeForwardedNode(currentForward);
                }

                currentForward = currentAncestor;
    }
}

/** @brief Removes a redirection.
 * Removes a redirection, which includes dereference the forwarding node
 * in the redirected node, exclusion of the redirected node from the array
 * of the redirected nodes, clearing the flags and removal of the potentially
 * unnecessary nodes in the final redirection tree.
 *
 * @param[in, out] toDeforward - a node storing information about the redirected
 *                               prefix
 */
static void removeForwardedNodeFromInitialAndRemoveInitialFromForward(
                                                InitialNode* toDeforward) {
    ForwardedNode * finalForward = toDeforward->forwardingNode;
    uint64_t index = toDeforward->indexForward;

    finalForward->forwardedNodes[index] = NULL;
    (finalForward->sumForwarded)--;
    toDeforward->forwardingNode = NULL;
    toDeforward->indexForward = 0;

    clearBitForward(&(toDeforward->isForwarded));
    if (finalForward->sumForwarded == 0) {
        clearBitForward(&(finalForward->isForwarding));
        free(finalForward->forwardedPrefix);
        finalForward->forwardedPrefix = NULL;
    }

    removeStumpsForwardedNode(finalForward);
}

/** @brief Removes a node.
 * Removes a node responsible for storing information about the redirected
 * prefix and updates information about the children in the parental node.
 *
 * @param[in, out] init - a node to be removed from a tree.
 */
static void removeInitialNode(InitialNode* init) {
    if (init) {
        InitialNode * ancestor = init->ancestor;
        if (ancestor) {
            ancestor->alphabet[init->edgeLeadingTo] = NULL;
            (ancestor->filledEdges)--;
        }

        free(init->initialPrefix);
        free(init);
    }
}

/** @brief Removes unnecessary nodes.
 *  Removes unnecessary nodes from a tree: nodes which are not on the path
 *  ending with a node regarded as terminal for the given prefix.
 *
 * @param currentInitial - a node responsible for storing data about
 *                         the redirected prefix, which starts the chain of
 *                         nodes removal.
 */
static void removeStumpsInitialNode(InitialNode * currentInitial) {
    while (currentInitial && currentInitial->filledEdges == 0 &&
           !(isForwardSet(currentInitial->isForwarded))) {
                InitialNode * currentAncestor = currentInitial->ancestor;
                if (currentAncestor) {
                    removeInitialNode(currentInitial);
                }
                currentInitial = currentAncestor;
    }
}

void phfwdRemove(PhoneForward * pf, char const * num) {
    if (pf) {
        size_t len = checkLength(num);

        if (len == 0) {
            return;
        }

        uint64_t depth = 0;
        bool possibleToPass = true;
        InitialNode *currentInitialCore = pf->initialRoot;
        int digit;
        while (depth < len && possibleToPass) {
            digit = getIndex(num[depth]);

            if (currentInitialCore->alphabet[digit]) {
                currentInitialCore = currentInitialCore->alphabet[digit];
                depth++;
            } else {
                possibleToPass = false;
            }
        }
        if (!possibleToPass) {
            return;
        }

        InitialNode *currentInitial = currentInitialCore;
        InitialNode *coreAncestor = currentInitialCore->ancestor;
        InitialNode *currentAncestor;

        while (currentInitial != coreAncestor) {
            if (isForwardSet(currentInitial->isForwarded)) {
                removeForwardedNodeFromInitialAndRemoveInitialFromForward(
                        currentInitial);
            }

            if (currentInitial->filledEdges == 0) {
                currentAncestor = currentInitial->ancestor;
                removeInitialNode(currentInitial);
                currentInitial = currentAncestor;
            } else {
                int *index = &(currentInitial->lastChecked);

                while (!currentInitial->alphabet[*index]) {
                    (*index)++;
                }

                currentInitial = currentInitial->alphabet[*index];
            }
        }

        removeStumpsInitialNode(currentInitial);
    }
}

void phfwdDelete(PhoneForward * pf) {
    if (pf) {
        InitialNode * currentInitial = pf->initialRoot;
        InitialNode * currentInitAncestor;

        //NULL is an ancestor
        while (currentInitial) {
            if (currentInitial->filledEdges == 0) {
                currentInitAncestor = currentInitial->ancestor;
                removeInitialNode(currentInitial);
                currentInitial = currentInitAncestor;
            }
            else {
                int * index = &(currentInitial->lastChecked);

                while (!currentInitial->alphabet[*index]) {
                    (*index)++;
                }

                currentInitial = currentInitial->alphabet[*index];
            }
        }

        ForwardedNode * currentForward = pf->forwardedRoot;
        ForwardedNode * currentForwardAncestor;

        while (currentForward) {
            if (currentForward->filledEdges == 0) {
                currentForwardAncestor = currentForward->ancestor;
                removeForwardedNode(currentForward);
                currentForward = currentForwardAncestor;
            }
            else {
                int *index = &(currentForward->lastChecked);

                while(!currentForward->alphabet[*index]) {
                    (*index)++;
                }

                currentForward = currentForward->alphabet[*index];
            }
        }

        free(pf);
    }
}

/** @brief Creates and initializes a structure.
 * Creates and initializes a PhoneNumbers structure. The resulting structure
 * has allocated memory with an empty slot.
 *
 * @return A pointer to initialized @p PhoneNumbers structure.
 */
static PhoneNumbers * createNewPhoneNumbers() {
    PhoneNumbers * result = malloc(sizeof (PhoneNumbers));
    if (!result) {
        return NULL;
    }
    
    result->numbers = malloc(sizeof (char*));
    if (!result->numbers) {
        return NULL;
    }

    // The reallocation should occur analogously to adding nodes
    result->numbers[0] = NULL;
    result->slots = 1;
    result->lastAvailableIndex = 1;
    
    return result;
}

PhoneNumbers * phfwdGet(PhoneForward const *pf, char const* num) {
    size_t len = checkLength(num);
    PhoneNumbers * result = createNewPhoneNumbers();
    if (!pf) {
        return NULL;
    }

    if (!result) {
        return NULL;
    }

    if (len == 0) {
        return result;
    }

    InitialNode * lastForwardedNode = NULL;
    InitialNode * currentInitial = pf->initialRoot;
    bool isPossibleToPass = true;
    size_t depth = 0;
    int digit;
    while (depth < len && isPossibleToPass && currentInitial) {
        digit = getIndex(num[depth]);
        if (isForwardSet(currentInitial->isForwarded)) {
            lastForwardedNode = currentInitial;
        }

        if (currentInitial->alphabet[digit]) {
            currentInitial = currentInitial->alphabet[digit];
            depth++;
        }
        else {
            isPossibleToPass = false;
        }
    }

    //Check the last one
    if (isForwardSet(currentInitial->isForwarded)) {
        lastForwardedNode = currentInitial;
    }

    if (!lastForwardedNode) {
        result->numbers[0] = strdup(num);
        if (!result->numbers[0]) {
            return NULL;
        }

        return result;
    }

    ForwardedNode * forwardedPrefixNode = lastForwardedNode->forwardingNode;
    char* finalPrefix = forwardedPrefixNode->forwardedPrefix;

    size_t finalPrefixLength = forwardedPrefixNode->depth;
    size_t nonForwardedPrefixLength = lastForwardedNode->depth;
    size_t finalSuffixLength = len - nonForwardedPrefixLength;
    size_t finalLength = finalSuffixLength + finalPrefixLength + 1;

    char* resultingForward = malloc(finalLength);
    if (!resultingForward) {
        return NULL;
    }

    memmove(resultingForward, finalPrefix, finalPrefixLength);
    memmove(resultingForward + finalPrefixLength, num + nonForwardedPrefixLength,
            finalSuffixLength);
    resultingForward[finalLength - 1] = '\0';

    result->numbers[0] = resultingForward;

    return result;
}

void phnumDelete(PhoneNumbers *pnum) {
    if (pnum) {
        for (uint64_t i = 0; i < pnum->lastAvailableIndex; i++) {
            free(pnum->numbers[i]);
        }

        free(pnum->numbers);
        free(pnum);
    }
}

char const * phnumGet(PhoneNumbers const *pnum, size_t idx) {
    if (!pnum || idx >= pnum->lastAvailableIndex) {
        return NULL;
    }
    else {
        return (char const*)pnum->numbers[idx];
    }
}

PhoneNumbers * phfwdReverse(__attribute__((unused)) PhoneForward const * pf,
                            __attribute__((unused)) char const *num) {
    return NULL;
}