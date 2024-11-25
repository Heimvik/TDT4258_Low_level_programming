#include "generics.h"

/*
Raw
*/
void setVaraible(void* structPtr,size_t offset, uint8_t target){
    *(uint8_t*)((uint8_t*)structPtr + offset) = target;
}

void copyMemory(void* src, void* dst,size_t bytes){
    for(size_t i = 0;i<bytes;i++){
        ((uint8_t*)dst)[i] = ((uint8_t*)src)[i];
    }
}

void swapMemory(void* a, void* b, size_t size){
    uint8_t tmpA[size];
    copyMemory(a,(void*)tmpA,size);

    copyMemory(b,a,size);
    copyMemory(tmpA,b,size); 
}

uint8_t compareGeneric(void* a,void* b,size_t size){
    uint8_t* castedA = (uint8_t*)a;
    uint8_t* castedB = (uint8_t*)b;

    for(size_t i = 0; i<size; i++){
        if(castedA[i] != castedB[i]){
            return (uint8_t)0;
        }
    }
    return (uint8_t)1;
}
void serializeStructure(void* structure,size_t size,uint8_t* byteArr){
    for(size_t i = 0; i<size;i++){
        byteArr[i] = ((uint8_t*)structure)[i];
    }
}

void deserializeStructure(uint8_t* byteArr, void* structure, size_t size){
    for(size_t i = 0; i<size;i++){
        ((uint8_t*)structure)[i] = byteArr[i];    
    }
}

void printUint8Array(uint8_t* array, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("0x%02X ", array[i]);
    }
    printf("\n");
}


void printMemory(void* structure,size_t size){
    uint8_t* castedStructure = (uint8_t*)structure;
    printf("Memory printout:\n\r");
    for(size_t i = 0;i<size;i++){
        printf(" 0x%x ",castedStructure[i]);
    }
    printf("\n\r");
}


int main(){
    Motor motor0 = (Motor){
        .id = 12,
        .speed = 1
    };
    LED led = (LED){
        .id = 12,
        .brightness = 0
    };
    printMemory(&motor0,sizeof(Motor));
    printMemory(&led,sizeof(Motor));
    swapMemory(&motor0,&led,sizeof(Motor));
    printMemory(&led,sizeof(Motor));
    printMemory(&motor0,sizeof(Motor));
    exit(0);
    
    uint8_t serializedData[sizeof(Motor)];
    Motor motor1;
    serializeStructure(&motor0,sizeof(Motor),serializedData);
    printUint8Array(serializedData,sizeof(Motor));
    deserializeStructure(serializedData,&motor1,sizeof(Motor));

    printMemory(&motor1,sizeof(Motor));
}