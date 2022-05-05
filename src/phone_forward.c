//
// Created by heheszek on 05.05.22.
//
#include "phone_forward.h"
#include <stdint.h>

typedef struct InitialNode {
    struct InitialNode* alphabet[10];
    uint16_t depth;
    uint8_t terminalForwarded;
} InitialNode;
typedef struct PhoneForward {

} PhoneForward;