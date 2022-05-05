//
// Created by heheszek on 05.05.22.
//
#include <stdlib.h>
#include <string.h>
#include "phone_forward.h"
#include <stdint.h>
#include <stdbool.h>

#define ALPHABET_SIZE   10

#define TERMINAL_BIT    0
#define FORWARD_BIT   1
struct ForwardedNode;

typedef struct InitialNode {
    struct InitialNode* ancestor;
    struct ForwardedNode* forward;
    struct InitialNode* alphabet[ALPHABET_SIZE];
    uint64_t depth;
    uint8_t terminalForwarded;
    uint64_t indexForward;
    uint64_t howManyHavePassedThrough;
} InitialNode;

typedef struct ForwardedNode {
    struct ForwardedNode* ancestor;
    struct ForwardedNode* alphabet[ALPHABET_SIZE];
    uint8_t terminalForwarding;
    uint64_t numForwardedNodes;
    uint64_t numSlotsForNodes;
    InitialNode** forwardedNodes;
    char* forwardedPrefix;
    uint64_t howManyHavePassedThrough;
} ForwardedNode;

typedef struct PhoneForward {
    ForwardedNode* forwardedRoot;
    InitialNode* initialRoot;
} PhoneForward;

PhoneForward * phfwdNew(void) {
    PhoneForward * result = malloc(sizeof(PhoneForward));
    if (!result) {
        return NULL;
    }

    result->forwardedRoot = malloc(sizeof (ForwardedNode));
    if (!result->forwardedRoot) {
        free(result);
        return NULL;
    }

    result->initialRoot = malloc(sizeof (InitialNode));
    if (!result->initialRoot) {
        free(result->forwardedRoot);
        free(result);

        return NULL;
    }

    return result;
}

void setBit(uint8_t* flag, int numBitsToShift) {
    *flag |= ((uint8_t) 1 << numBitsToShift);
}

void clearBit(uint8_t* flag, int numBitsToShift) {
    *flag &= ~((uint8_t)1 << numBitsToShift);
}

InitialNode * initInitialNode(InitialNode* ancestor, uint64_t depth,
                              bool isTerminal) {
    InitialNode * result = malloc(sizeof (InitialNode));
    if (!result) {
        return NULL;
    }

    result->ancestor = ancestor;
    result->forward = NULL;
    result->depth = depth;
    result->terminalForwarded = 0;
    result->indexForward = 0;
    result->howManyHavePassedThrough = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        result->alphabet[i] = NULL;
    }

    if (isTerminal) {
        setBit(&(result->terminalForwarded), TERMINAL_BIT);
    }

    return result;
}

ForwardedNode * initForwardedNode(ForwardedNode* ancestor, bool isTerminal) {
//                                  bool isForwarding) {
    ForwardedNode * result = malloc(sizeof (ForwardedNode));
    if (!result) {
        return NULL;
    }

    result->terminalForwarding = 0;
    if (isTerminal) {
        setBit(&(result->terminalForwarding), TERMINAL_BIT);
    }

//    if (isForwarding) {
//        setBit(&(result->terminalForwarding), FORWARD_BIT);
//    }

    result->ancestor = ancestor;
    result->forwardedPrefix = NULL;
    result->howManyHavePassedThrough = 0;

    result->numForwardedNodes = 0;
    result->numSlotsForNodes = 0;
    result->forwardedNodes = NULL;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        result->alphabet[i] = NULL;
    }

    return result;
};

bool setAsFinalForward(ForwardedNode* forwarding, const char* prefix) {
    forwarding->forwardedPrefix = strdup(prefix);

    if (!(forwarding->forwardedPrefix)) {
        return false;
    }

    setBit(&(forwarding->terminalForwarding), FORWARD_BIT);

    return true;
}

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
}