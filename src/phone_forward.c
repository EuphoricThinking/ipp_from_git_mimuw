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
    uint64_t filledEdges;
    int edgeLeadingTo;
    int lastChecked;
    char* initialPrefix;
} InitialNode;

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
} ForwardedNode;

typedef struct PhoneForward {
    ForwardedNode* forwardedRoot;
    InitialNode* initialRoot;
} PhoneForward;

typedef struct PhoneNumbers {
    char** numbers;
    uint64_t slots;
    uint64_t lastIndex;
} PhoneNumbers;

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

InitialNode * initInitialNode(InitialNode* ancestor, uint64_t depth,
                              int edgeLeadingTo) {
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
    result->filledEdges = 0;
    result->lastChecked = 0;
    result->initialPrefix = NULL;
    result->edgeLeadingTo = edgeLeadingTo;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        result->alphabet[i] = NULL;
    }

//    if (isTerminal) {
//        setBit(&(result->forwarded), TERMINAL_BIT);
//    }

    return result;
}

ForwardedNode * initForwardedNode(ForwardedNode* ancestor, uint64_t depth,
                                  int edgeLeadingTo;) {
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
    (finalForward->sumForwarded)++;

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
    if (!number) {
        return -1;
    }

    int index = 0;

    while (isdigit(number[index]) && number[index] != '\0') index++;

    if (number[index] != '\0' || index == 0) {
        return -1;
    }
    else {
        return index;
    }
}

//bool isTerminal(int flag) {
//    return (flag && ((uint8_t)1 << TERMINAL_BIT)) != 0;
//}

//bool isForwardSet(int flag) {
//    return (flag && ((uint8_t)1 << FORWARD_BIT)) != 0;
//}
bool isForwardSet(uint8_t flag) {
    return (flag && (uint8_t) 1) != 0;
}

int getIndex(char c) {
    return c - '0';
}

bool phfwdAdd(PhoneForward *pfd, char const *num1, char const *num2) {
    size_t len1 = checkLength(num1);
    if (len1 == -1) {
        return false;
    }

    size_t len2 = checkLength(num2);
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
        
        if (!(currentInitial->alphabet[digit])) { //TODO check shortened version
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
        }
    }

//    if (currentInitial->filledEdges == 0) {
//        setBit(&(currentInitial->forwarded), TERMINAL_BIT);
//    }

//    setBit(&(currentInitial->forwarded), FORWARD_BIT);
//    setBitForward(&(currentInitial->isForwarded));

    depth = 0;
    ForwardedNode * currentForward = pfd->forwardedRoot;
    
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

void removeForwardedNode(ForwardedNode * toDelete) {
    ForwardedNode * ancestor = toDelete->ancestor;
    if (ancestor) {
        ancestor->alphabet[toDelete->edgeLeadingTo] = NULL;
        (ancestor->filledEdges)--;
    }

    free(toDelete->forwardedPrefix);
    free(toDelete->forwardedNodes);
    free(toDelete);
}

void removeStumpsForwardedNode(ForwardedNode * currentForward) {
    while (currentForward && currentForward->filledEdges == 0
            && currentForward->sumForwarded == 0) {
                ForwardedNode * currentAncestor = currentForward->ancestor;
                removeForwardedNode(currentForward);
                currentForward = currentAncestor;
    }
}

void removeForwardedNodeFromInitial(InitialNode* toDeforward) {
    ForwardedNode * finalForward = toDeforward->forwardingNode;
    uint64_t index = toDeforward->indexForward;

    finalForward->forwardedNodes[index] = NULL;
    (finalForward->sumForwarded)--;
    toDeforward->forwardingNode = NULL;
    toDeforward->indexForward = 0;

    clearBitForward(&(toDeforward->isForwarded));
    if (finalForward->sumForwarded == 0) {
        clearBitForward(&(finalForward->isForwarding));
    }

    removeStumpsForwardedNode(finalForward);
}

void removeInitialNode(InitialNode* init) {
    InitialNode * ancestor = init->ancestor;
    if (ancestor) {
        ancestor->alphabet[init->edgeLeadingTo] = NULL;
        (ancestor->filledEdges)--;
    }

    free(init->initialPrefix);
    free(init);
}

void removeStumpsInitialNode(InitialNode * currentInitial) {
    while (currentInitial && currentInitial->filledEdges == 0 &&
           !(isForwardSet(currentInitial->isForwarded))) {
                InitialNode * currentAncestor = currentInitial->ancestor;
                removeInitialNode(currentInitial);
                currentInitial = currentAncestor;
    }
}

void phfwdRemove(PhoneForward * pf, char const * num) {
    size_t len = checkLength(num);

    if (len == -1) {
        return;
    }

    uint64_t depth = 0;
    bool possibleToPass = true;
    InitialNode * currentInitialCore = pf->initialRoot;
    int digit;
    while (depth < len && possibleToPass) {
        digit = getIndex(num[depth]);
        if (currentInitialCore->alphabet[digit]) {
            currentInitialCore = currentInitialCore->alphabet[digit];
            depth++;
        }
        else {
            possibleToPass = false;
        }
    }

    if (!possibleToPass) {
        return;
    }

//    uint64_t leftPathsToCheck = currentInitialCore->filledEdges;
    InitialNode * currentInitial = currentInitialCore;
    InitialNode * coreAncestor = currentInitialCore->ancestor;
    InitialNode * currentAncestor;

    while (currentInitial != coreAncestor) {
        if (isForwardSet(currentInitial->isForwarded)) {
            removeForwardedNodeFromInitial(currentInitial);
        }
        
        if (currentInitial->filledEdges == 0) {
            currentAncestor = currentInitial;
            removeInitialNode(currentInitial);
            currentInitial = currentAncestor;
        }
        else {
            int * index = &(currentInitial->lastChecked);
            while (*index < ALPHABET_SIZE && !currentInitial->alphabet[*index]) {
                (*index)++;
            }
            // TODO check in tests if can be reduced
            if (*index < ALPHABET_SIZE) {
                currentInitial = currentInitial->alphabet[*index];
            }
            else {
                *index = 0;
                currentInitial = currentInitial->ancestor;
            }
        }
    }

    removeStumpsInitialNode(currentInitial);
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
//                while (*index < ALPHABET_SIZE && !currentInitial->alphabet[*index]) {
//                    (*index)++;
//                }
                while (!currentInitial->alphabet[*index]) {
                    (*index)++;
                }


                currentInitial = currentInitial->alphabet[*index];
//                if (*index < ALPHABET_SIZE) {
//                    currentInitial = currentInitial->alphabet[*index];
//                }
//                else {
//                    *index = 0;
//                    currentInitial =
//                }
            }
        }

        ForwardedNode * currentForward = pf->forwardedRoot;
        ForwardedNode * currentForAncestor;

        while (currentForward) {
            if (currentForward->filledEdges == 0) {
                currentForAncestor = currentForward->ancestor;
                removeForwardedNode(currentForward);
                currentForward = currentForAncestor;
            }
            else {
                int *index = &(currentForward->lastChecked);

                while(!currentForward->alphabet[*index]) {
                    (*index)++;
                }

                currentForward = currentForward->alphabet[*index];
            }
        }
    }
}

PhoneNumbers * createNewPhoneNumbers() {
    PhoneNumbers * result = malloc(sizeof (PhoneNumbers));
    if (!result) {
        return NULL;
    }
    
    result->numbers = malloc(sizeof (char*));
    if (!result->numbers) {
        return NULL;
    }

    result->slots = 1;
    result->lastIndex = 0;
    
    return result;
}

PhoneNumbers * phfwdGet(PhoneForward const *pf, char const* num) {
    size_t len = checkLength(num);
    PhoneNumbers * result = createNewPhoneNumbers();
    if (!result) {
        return NULL;
    }

    if (len == -1) {
        result->numbers[0] = NULL;

        return result;
    }

    InitialNode * lastForwardedNode = NULL;
    InitialNode * currentInitial = pf->initialRoot;
    bool isPossible = true;
    size_t depth = 0;
    int digit;
    while (depth < len && isPossible) {
        digit = getIndex(num[depth]);
        if (isForwardSet(currentInitial->isForwarded)) {
            lastForwardedNode = currentInitial;
        }
        
        if (currentInitial->alphabet[digit]) {
            currentInitial = currentInitial->alphabet[digit];
            depth++;
        }
        else {
            isPossible = false;
        }
    }

    if (!isPossible) {
        result->numbers[0] = strdup(num);
        if (!result->numbers[0]) {
            return NULL;
        }

        return result;
    }


}