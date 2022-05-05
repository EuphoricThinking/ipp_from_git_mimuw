//
// Created by heheszek on 05.05.22.
//
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