#include <stdint.h>
#include <stdlib.h>

void preIncrement(){
	int varPre=10,varPost=10; 
    int tempPre,tempPost;
	tempPre = ++varPre;
    tempPost = varPost++;
    printf("Using ++varPre: %d\n",tempPre);
    printf("Using varPost++: %d\n",tempPost);
    printf("Initial values: %d %d\n",varPre,varPost);
}

int main(){
    preIncrement();
    return 0;
}