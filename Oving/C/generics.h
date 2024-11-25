#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Motor{
    uint16_t id;
    uint8_t speed;
} Motor;

typedef struct LED{
    uint16_t id;
    uint8_t brightness;
} LED;

void setVaraible(void* structPtr,size_t offset, uint8_t target);
