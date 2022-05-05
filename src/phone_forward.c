//
// Created by heheszek on 05.05.22.
//
#include <stdlib.h>
#include "phone_forward.h"
#include <stdint.h>

struct ForwardedNode;

typedef struct InitialNode {
    struct InitialNode* ancestor;
    struct ForwardedNode* forward;
    struct InitialNode* alphabet[10];
    uint64_t depth;
    uint8_t terminalForwarded;
    uint64_t indexForward;
    uint64_t howManyHavePassedThrough;
} InitialNode;

typedef struct ForwardedNode {
    struct ForwardedNode* alphabet[10];
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