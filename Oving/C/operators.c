#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void pBin(uint8_t num) {  // Helper function to print binary representation of a 8 bit number
    char binary[9]; // 8 bits + null terminator
    binary[8] = '\0'; // null terminator
    for (int i = 0; i < 8; i++) { 
        binary[i] = (num & 1) ? '1' : '0'; // compact if-else
        num >>= 1;
    }
    printf("%s ", binary);
}

void increment(){
	int varPre=10,varPost=10; 
    int tempPre,tempPost;
	tempPre = ++varPre;
    tempPost = varPost++;
    printf("Using ++varPre: %d\n",tempPre);
    printf("Using varPost++: %d\n",tempPost);
    printf("Initial values: %d %d\n",varPre,varPost);
}

void bwAnd(){
    //Set a bit
    uint8_t dst = 0x12;
    dst |= 0x80;
    printf("Should be 0x92: 0x%x\n",dst);
}
void bwReversal(void* src, void* dst, size_t size){
    for(int i = 0; i<size; i++){
        uint8_t currByte = ((uint8_t*)src)[i];
        uint8_t targetByte = 0x00;
        uint8_t mask = 0x80;
        for(int j = 0; j<8;j++){
            if (mask&currByte){
                targetByte |= (1<<j);
            }
            mask = mask>>1;
        }
        ((uint8_t*)dst)[(size-1)-i] = targetByte;
    }
}

int main(){
    //preIncrement();
    //bwOperators();
    uint16_t src = (uint16_t)0x0001;
    uint16_t dst;
    uint16_t mask = 0x0001;
    int8_t shiftDir = 0;
    while(1){
        
        bwReversal(&src,&dst,sizeof(uint16_t));
        pBin((uint8_t)src);
        pBin((uint8_t)(src>>8));
        printf("\n");
        pBin((uint8_t)dst);
        pBin((uint8_t)(dst>>8));

        if(mask == 0x8000){
            shiftDir = -1;
        } else if (mask == 0x1){
            shiftDir = 1;
        }
        if (shiftDir == 1){
            mask = mask << 1;
        } else if (shiftDir == -1){
            mask = mask >> 1;
        }
        src = mask;
        usleep(50000);
        printf("\n");
    }
    return 0;
}