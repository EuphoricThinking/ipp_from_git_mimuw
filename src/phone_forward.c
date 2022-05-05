//
// Created by heheszek on 05.05.22.
//
#include <stdlib.h>
#include "phone_forward.h"
#include <stdint.h>

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
    struct ForwardedNode* alphabet[ALPHABET_SIZE];
    uint8_t terminalForwarding;
    uint64_t numForwardedNodes;
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