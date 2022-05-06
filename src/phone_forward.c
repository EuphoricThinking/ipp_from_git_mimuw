//
// Created by heheszek on 05.05.22.
//
#include <stdlib.h>
#include <string.h>
#include "phone_forward.h"
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#define ALPHABET_SIZE   10

//#define TERMINAL_BIT    0
#define FORWARD_BIT   0

struct ForwardedNode;

typedef struct InitialNode {
    struct InitialNode* ancestor;
    struct ForwardedNode* forwardingNode;
    struct InitialNode* alphabet[ALPHABET_SIZE];
    uint64_t depth;
    uint8_t isForwarded;
    uint64_t indexForward;
    uint64_t howManyHavePassedThrough;
    char* initialPrefix;
} InitialNode;

typedef struct ForwardedNode {
    struct ForwardedNode* ancestor;
    struct ForwardedNode* alphabet[ALPHABET_SIZE];
    uint8_t isForwarding;
    uint64_t numForwardedNodes;
    uint64_t depth;
    uint64_t numSlotsForNodes;
    InitialNode** forwardedNodes;
    char* forwardedPrefix;
    uint64_t howManyHavePassedThrough;
} ForwardedNode;

typedef struct PhoneForward {
    ForwardedNode* forwardedRoot;
    InitialNode* initialRoot;
} PhoneForward;


//void setBit(uint8_t* flag, int numBitsToShift) {
//    *flag |= ((uint8_t) 1 << numBitsToShift);
//}
//
//void clearBit(uint8_t* flag, int numBitsToShift) {
//    *flag &= ~((uint8_t)1 << numBitsToShift);
//}
void setBitForward(uint8_t * flag) {
    *flag |= (uint8_t) 1;
}

void clearBitForward(uint8_t * flag) {
    *flag &= ~(uint8_t) 1;
}

InitialNode * initInitialNode(InitialNode* ancestor, uint64_t depth) {
 //                             bool isTerminal) {
    InitialNode * result = malloc(sizeof (InitialNode));
    if (!result) {
        return NULL;
    }

    result->ancestor = ancestor;
    result->forwardingNode = NULL;
    result->depth = depth;
    result->isForwarded = 0;
    result->indexForward = 0;
    result->howManyHavePassedThrough = 0;
    result->initialPrefix = NULL;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        result->alphabet[i] = NULL;
    }

//    if (isTerminal) {
//        setBit(&(result->forwarded), TERMINAL_BIT);
//    }

    return result;
}

ForwardedNode * initForwardedNode(ForwardedNode* ancestor, uint64_t depth) {
//                                  bool isForwarding) {
    ForwardedNode * result = malloc(sizeof (ForwardedNode));
    if (!result) {
        return NULL;
    }

    result->isForwarding = 0;
//    if (isTerminal) {
//        setBit(&(result->forwarding), TERMINAL_BIT);
//    }

//    if (isForwarding) {
//        setBit(&(result->forwarding), FORWARD_BIT);
//    }

    result->ancestor = ancestor;
    result->forwardedPrefix = NULL;
    result->howManyHavePassedThrough = 0;
    result->depth = depth;

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

    result->forwardedRoot = initForwardedNode(NULL, 0);
    if (!result->forwardedRoot) {
        return NULL;
    }

    result->initialRoot = initInitialNode(NULL, 0);
    if (!result->initialRoot) {
        return NULL;
    }

    return result;
}

//bool setAsFinalForward(ForwardedNode* forwardingNode, const char* prefix) {
//    forwardingNode->forwardedPrefix = strdup(prefix);
//
//    if (!(forwardingNode->forwardedPrefix)) {
//        return false;
//    }
//
//    setBit(&(forwarding->forwarding), FORWARD_BIT);
//    setBitForward(&(forwardingNode->isForwarding));
//
//    return true;
//}

bool addForwardedNode(InitialNode* toBeForwarded, ForwardedNode* finalForward) {
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

    setBitForward(&(toBeForwarded->isForwarded));
    setBitForward(&(finalForward->isForwarding));

    return true;
}

bool addPrefixForward(ForwardedNode* finalForward, const char* prefix) {
    forwardingNode->forwardedPrefix = strdup(prefix);

    if (!(forwardingNode->forwardedPrefix)) {
        return false;
    }

    return true;
}

bool addPrefixInitial(InitialNode * init, const char* prefix) {
    init->initialPrefix = strdup(prefix);

    if (!(init->initialPrefix)) {
        return false;
    }

    return true;
}

int64_t checkLength(const char * number) {
    int index = 0;

    while (isdigit(number[index]) && number[index] != '\0') index++;

    if (number[index] == '\0') {
        return index;
    }
    else {
        return -1;
    }
}

//bool isTerminal(int flag) {
//    return (flag && ((uint8_t)1 << TERMINAL_BIT)) != 0;
//}

//bool isForwardSet(int flag) {
//    return (flag && ((uint8_t)1 << FORWARD_BIT)) != 0;
//}
bool isForwarded(uint8_t flag) {
    return (flag && (uint8_t) 1) != 0;
}

int getIndex(char c) {
    return c - '0';
}

bool phfwdAdd(PhoneForward *pfd, char const *num1, char const *num2) {
    int64_t len1 = checkLength(num1);
    if (len1 == -1) {
        return false;
    }

    int64_t len2 = checkLength(num2);
    if (len2 == -1) {
        return false;
    }

    if (strcmp(num1, num2) == 0) {
        return false;
    }

    uint64_t depth = 0;
    InitialNode * currentInitial = pfd->initialRoot;
    int digit;
    while (depth < len1) {
        digit = getIndex(num1[depth]); //TODO check if it works
        currentInitial->howManyHavePassedThrough++;
        
        if (!(currentInitial->alphabet[digit])) { //TODO check shortened version
            InitialNode * newNode = initInitialNode(currentInitial, ++depth);
            if (!newNode) {
                return false;
            }

            currentInitial->alphabet[digit] = newNode;
            currentInitial = newNode;
        }
        else {
            currentInitial = currentInitial->alphabet[digit];
        }
    }

//    if (currentInitial->howManyHavePassedThrough == 0) {
//        setBit(&(currentInitial->forwarded), TERMINAL_BIT);
//    }

//    setBit(&(currentInitial->forwarded), FORWARD_BIT);
//    setBitForward(&(currentInitial->isForwarded));

    depth = 0;
    ForwardedNode * currentForward = pfd->forwardedRoot;
    
    while (depth < len2) {
        digit = getIndex(num2[depth]);
        currentForward->howManyHavePassedThrough++;
        
        if (!(currentForward->alphabet[digit])) {
            ForwardedNode * newNode = initForwardedNode(currentForward, ++depth);
            if (!newNode) {
                return false;
            }
            
            currentForward->alphabet[digit] = newNode;
            currentForward = newNode;
        }
        else {
            currentForward = currentForward->alphabet[digit];
        }
    }

//    setBitForward(&(currentForward->ISforwarding));

    if (!addForwardedNode(currentInitial, currentForward)) {
        return false;
    }

    if (!(addPrefixForward(currentForward, num2))
        || !(addPrefixInitial(currentInitial, num1))) {
        return false;
    }

    return true;
}