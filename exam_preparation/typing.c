/*
This file is for practicing:
- Types and bit converions
- Memory organizations
- How types are stored in memory
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

void pBin(uint8_t num) {  // Helper function to print binary representation of a byte(uint8_t)
    char binary[9]; // 8 bits + null terminator
    binary[8] = '\0'; // null terminator
    for (int i = 7; i >= 0; i--) { 
        binary[i] = (num & 1) ? '1' : '0'; // compact if-else
        num >>= 1;
    }
    printf("%s", binary);
}

/*
Write a C function that takes an unsigned 32-bit integer and returns the number of set bits (1s) in its binary representation.
*/
uint8_t countBits(uint32_t operand){
    uint32_t mask = 0b1;
    uint8_t oneCount = 0;
    for(int i = 0; i<32;i++){
        if(mask & operand){
            oneCount++;
        }
        mask<<=1;
    }
    return oneCount;
}

/*
Write a function in C that takes an unsigned 32-bit integer and returns the largest power of 2 less than or equal to the given number.

Solve the problem using bitwise operations only (no loops or multiplication/division).
Ensure the function is optimized for execution time.
*/
uint8_t recursiveFindFirstOne(uint32_t number, uint8_t currentIndex){
    if(number & 1<<currentIndex){
        return currentIndex;
    } else if (currentIndex>=1) {
        return recursiveFindFirstOne(number,currentIndex-1);
    } else {
        return 0;
    }
}
uint32_t largestLessOrEqual(uint32_t number){
    uint8_t firstOnePos = recursiveFindFirstOne(number,(uint8_t)(sizeof(uint32_t)*4)-1);
    return ((uint32_t)(1<<firstOnePos));
}

/*
Write a C program to find the length of the longest substring of a given string without repeating characters.
*/
uint8_t longestSubString(char* input){
    char* lsPtr = (char*)calloc(100,sizeof(char));
    char* lsPtrMax = (char*)calloc(100,sizeof(char));

    uint8_t currentLsIndex = 0;
    uint8_t currentInputIndex = 0;
    char currentInputChar = 0xFF;

    while(currentInputChar != '\0'){
        currentInputChar = input[currentInputIndex];

        uint8_t isPresent = 0;
        for(int i = 0; i<100; i++){
            if(currentInputChar == lsPtr[i]){
                isPresent = 1;
            }
        }
        if(!isPresent){
            lsPtr[currentLsIndex] = currentInputChar;
            currentLsIndex++;
        } else {
            //We now have a series off chars that are different -> check if what we have save up is larger than the max
            uint8_t currentLsmIndex = 0;
            while(lsPtrMax[currentLsmIndex] != '\0'){
                currentLsmIndex++;
            }
            if(currentLsIndex>currentLsmIndex){
                memcpy(lsPtr,lsPtrMax,100);
            } else {
                memset(lsPtr,0,100);
            }
        }
        currentInputIndex++;
    }
    uint8_t finalLsmIndex = 0;
    while(lsPtrMax[finalLsmIndex] != '\0'){
        finalLsmIndex++;
    }
    free(lsPtr);
    free(lsPtrMax);
    return finalLsmIndex+1;
}




int main(){
    printf("Starting executing...\n");
    char string[10] = "Helloe";
    printf("%d",longestSubString(string));
    return 0;
}