#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void printStringInfo(const char* str, const char* name) {
    if (str == NULL) {
        printf("%s is NULL\n", name);
    } else {
        printf("%s: %s (Length: %zu)\n", name, str, strlen(str));
    }
}

char** conditionPalindrome(char* uncondCandidate,int uncondLength){
    char* condCandidate = (char*)malloc((uncondLength+1)*sizeof(char));
    int condIndex = 0;
    for(int i = 0;i<=uncondLength;i++){
        printf("%s\n",condCandidate);
        if(uncondCandidate[i]!=32){
            condCandidate[condIndex] = uncondCandidate[i];
            if(condCandidate[condIndex]>=97 && condCandidate[condIndex]<=122){
                condCandidate[condIndex]-=32;
            }
            condIndex++;
        }
    }
    condCandidate[condIndex] = '\0';
    char* flippedCondCandidate = (char*)malloc((condIndex+1)*sizeof(char));
    int condLength = condIndex-1;
    printf("Here: %s,%d",condCandidate,condLength);

    for(int i=condLength-1;i>=0;i--){
        flippedCondCandidate[i] = condCandidate[(condLength-1)-i];
    }
    flippedCondCandidate[condLength] = '\0'; // Null-terminate the flipped string
    printf("Uncond: %s, Cond: %s, Flippedcond: %s",uncondCandidate,condCandidate,flippedCondCandidate);
    free(uncondCandidate); //No need for this anymore
    char** candidateArray = (char**)malloc(2*sizeof(char*));
    
    printf("%s,%s",candidateArray[0],candidateArray[1]);
    candidateArray[0]=condCandidate;
    candidateArray[1]=flippedCondCandidate;
    printf("%s,%s",candidateArray[0],candidateArray[1]);
    return candidateArray;

}

bool checkPalindrome(char* candidate, char* flippedCandidate){
    int length = strlen(candidate);
    bool isPalindrome = true;
    for(int i=0;i<length;i++){
        printf("%d,%d\n",candidate[i],flippedCandidate[i]);
        if (!((candidate[i] == flippedCandidate[i]) || (candidate[i] == 63))){
            isPalindrome = false;
            break;
        }
    }
    return isPalindrome;
}

int main(){
    char uncondCandidate[] = "Was it a car or a cat I saw";
    
    int candidateLength = sizeof(uncondCandidate)/sizeof(uncondCandidate[0])-1; //-1 to hinder null terminating at the end
    //printf("%d",candidateLength);
    char** candidateArray = conditionPalindrome(uncondCandidate,candidateLength);
    //printf("%d",checkPalindrome(candidateArray[0],candidateArray[1]));
    free(candidateArray[0]);
    free(candidateArray[1]);
    free(candidateArray);

    return 0;
}