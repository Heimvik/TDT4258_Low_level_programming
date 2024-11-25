#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


/*First task:
- Make a generic 32-bit unsigned calculator using function pointers
- Should have operations: add, sub, all the bitwise
*/

void add(void* a,void* b,void* res){
    *(uint32_t*)res = *((uint32_t*)a)+*((uint32_t*)b);
}
void sub(void* a,void* b,void* res){
    *(uint32_t*)res = *((uint32_t*)a)-*((uint32_t*)b);
}
uint32_t calc(void* operations,uint8_t op,uint32_t* a,uint32_t* b, uint32_t* r){
    (*((void(**)(void*,void*,void*))operations)[op])(a,b,r);
}
void testCalc(void){
    uint32_t a = 20, b = 10, r = 0;
    void (*ops[2])(void*,void*,void*) = {&add,&sub};
    calc(ops,0,&a,&b,&r); //Calculates r = a + b
    calc(ops,1,&a,&b,&r); //Calculates r = a - b
}


/*
Task 2:
- Make a MAC, a = a+(b*c) and the other way around ACM, a = a * (b+c) using fucntion poitners
*/
void mac(void* b, void* c, void* a){
    *(uint32_t*)a += (*(uint32_t*)b * *(uint32_t*)c);
}
void acm(void* b, void* c, void* a){
    *(uint32_t*)a *= (*(uint32_t*)b)*(*(uint32_t*)c);
}
void calcAccumulate(void* operations,uint8_t op,uint32_t* b,uint32_t* c, uint32_t* a){
    (((void(**)(void*,void*,void*))operations)[op])(b,c,a);
}
void testAccumulate(){
    uint32_t a = 1;
    uint32_t b = 2;
    uint32_t c = 3;
    
    void (*ops[2])(void*,void*,void*) = {&mac,&acm};
    calcAccumulate(ops,0,&b,&c,&a);
    printf("a+(b*c)=%d\n",a);
    a = 1;
    b = 2;
    c = 3;
    calcAccumulate(ops,1,&b,&c,&a);
    printf("a*(b+c)=%d\n",a);
}

/*Task 3: Implement a binary calculator, only doing binary operations, treated as 64-bit unsigned*/

uint64_t calcAnd(void* a, void* b){
    return *(uint64_t*)a & *(uint64_t*)b;
}
uint64_t calcOr(void* a, void* b){
    return *(uint64_t*)a | *(uint64_t*)b;
}
uint64_t calcXor(void* a, void* b){
    return *(uint64_t*)a ^ *(uint64_t*)b;
}
void bitwiseCalc2(void* ops, uint8_t op,uint64_t* a,uint64_t* b){
    printf("%lx\n",((uint64_t(**)(void*,void*))ops)[op](a,b)); 
}

void bitwiseCalc(uint8_t op){
    uint64_t a = 0x8, b = 0x9;
    uint64_t (*operations[3])(void*,void*) = {&calcAnd,&calcOr,&calcXor};
    bitwiseCalc2(operations,op,&a,&b);
    printf("Result of op: %llx \n",(*operations[op])(&a,&b));
}


int main(void){
    //testAccumulate();
    //testCalc();
    bitwiseCalc(0);
    bitwiseCalc(1);
    bitwiseCalc(2);
    return 0;
}