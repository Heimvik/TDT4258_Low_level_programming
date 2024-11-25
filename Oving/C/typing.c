// TDT4258 autumn 2024, code to test some slides in Lecture 5, Lasse Natvig.
//
#include <stdio.h> // Needed for printf
#include<stdlib.h> // Needed for malloc
#include<string.h> // Needed for memcpy
#include<stdint.h>
void slide17() {
    int a = 524353; 
    short b;
    b = a;
    printf("slide 17: %x\n", b);
}

void slide18() {
    int a = 32833; 
    short b;
    b = a;
    printf("slide 18: %x\n", b);
}

void slide20() {
	int a;
	a = 8;
	short b;
	b = *(short*)&a;
    printf("slide 20: %x\n", b);
}

void slide21(){
	short a = 8; 
	int b; // L.E. = little endian
    b = *(int*)&a;
    printf("slide 21: %x\n", b);
    // masking to remove the unknown xxxx ... bits in the slide
	printf("masked on L.E. computer  %x\n", b & 0x0000ffff);  // slide 21    
}

void slide23() {
    int dummy1[ ] = {1, 2, 3, 4, 5};
    int arr[6];  // indexed by 0..5 
    arr[0] = 4;
    arr[5] = 40;
    arr[6] = 20;
    arr[10] = 30;
    arr[-3] = 10;
}

typedef struct point {
    int x;
    int y;
	int dummy1;
} point;

void slide29_30(){
    point p1;
    p1.x = 10;
    p1.y = 20;
    ((point* )&(p1.y))->x = 30;
    ((point* )&(p1.y))->y = 40;
    printf("slides 29-30: %d\n", p1.dummy1);
}

void swap(void* a, void* b, int size){
    char* buffer;
    if (( buffer = malloc(size)) == NULL) {
        printf("error: could not allocate memory\n");
    } 
    memcpy(buffer, a, size);
    memcpy(a, b, size);
    memcpy(b, buffer, size);
}

void slide36() {
    int x = 10;
    int y = 20;
    swap(&x, &y, sizeof(int));
    printf("slide 35: x, y: %d, %d\n", x, y);
    float f = 3.14; // more testing
    float g = 6.28;
    swap(&f, &g, sizeof(float));  
    printf("slide 35+: f, g: %f, %f", f, g);  
}

void slide37() {
    unsigned int a = -1; 
    int b = (int)a;      
    printf("slide 37: a = %u, b = %d\n", a, b);
}

void slide38() {
    float f = 5.7;
    int i = (int)f; 
    printf("slide 38: f = %f -> i = %d\n", f, i);
}

void slide39() {
    double d = -123.456;
    int i = (int)d; 
    printf("slide 39: d = %lf -> i = %d\n", d, i);
}

void slide40() {
    int a = -1;
    unsigned int b = (unsigned int)a; 
    printf("slide 40: b = %x\n", b);
}

void slide41() {
    float f = 3.14f;
    uint32_t* pi = (uint32_t*)&f;
    printf("slide 41: float as uint32_t = %x\n", *pi);
}

void slide42() {
    char c = 127; 
    c += 1;       
    printf("slide 42: c = %d\n", c);
}

void slide43() {
    struct align_test {
        char c;
        int i;
    } at;
    printf("slide 43: sizeof(struct align_test) = %zu\n", sizeof(at));
}

void slide44() {
    union {
        float f;
        int i;
    } u;
    u.f = 1.5f;
    printf("slide 44: f = %f, i = %x\n", u.f, u.i);
}

void slide45() {
    short s = -300;
    unsigned short us = (unsigned short)s;
    printf("slide 45: s = %d, us = %u (hex: %x)\n", s, us, us);
}

void slide46() {
    float f = -0.0f;
    unsigned int* bits = (unsigned int*)&f;
    printf("slide 46: f = %f, as bits = %x\n", f, *bits);
}

void slide47() {
    unsigned char a = 255;
    char b = (char)a; 
    printf("slide 47: a = %u, b = %d\n", a, b);
}

void slide48() {
    int a = 1;
    char* p = (char*)&a;
    printf("slide 48: first byte = %d\n", *p);
}

void slide49() {
    int a = 0x12345678;
    short* sp = (short*)&a;
    printf("slide 49: lower 16 bits = %x, upper 16 bits = %x\n", sp[0], sp[1]);
}

void slide50() {
    float f = 1.0f;
    double d = f; // Promote float to double
    printf("slide 50: f = %f, d = %lf\n", f, d);
}

void slide51() {
    int a = -12345678;
    unsigned int b = *(unsigned int*)&a;
    printf("slide 51: a = %d, as unsigned = %u (hex: %x)\n", a, b, b);
}

void slide52() {
    char arr[4] = {1, 2, 3, 4};
    int* p = (int*)arr;
    printf("slide 52: int = %x\n", *p);
}

void slide53() {
    int arr[3] = {0x12345678, 0x90abcdef, 0xdeadbeef};
    char *p = (char *)arr;
    printf("slide 53: First byte = %x, Second int = %x\n", *p, *((int *)(p + 4)));
}

void slide54() {
    struct {
        char a;
        int b;
        char c;
    } s = {'A', 0x12345678, 'B'};
    printf("slide 54: Struct size = %zu, offset b = %lx\n", 
           sizeof(s), (long int)&s.b-(long int)&s);
}

void slide55() {
    union {
        struct {
            char a;
            short b;
            int c;
        } s;
        int arr[2];
    } u;
    u.arr[0] = 0x12345678;
    u.arr[1] = 0x90abcdef;
    printf("slide 55: Struct a = %x, b = %x, c = %x\n", 
           u.s.a, u.s.b, u.s.c);
}

void slide56() {
    char arr[5] = {'A', 'B', 'C', 'D', '\0'};
    printf("slide 56: arr = %s, arr[3] = %c\n", arr, arr[3]);
}

void slide57() {
    char str1[] = "Hello";
    char str2[] = "World";
    strcpy(str1 + 3, str2);
    printf("slide 57: Modified str1 = %s\n", str1);
}

void slide58() {
    volatile int x = 5;
    int y = x * x;
    printf("slide 58: x = %d, y = %d\n", x, y);
}

void slide59() {
    int arr[4] = {1, 2, 3, 4};
    printf("slide 59: Address of arr[0] = %p, arr[-1] = %x\n", 
           (void *)&arr[0], arr[-1]);
}

void slide60() {
    int a = 0x12345678;
    char *p = (char *)&a;
    printf("slide 60: Integer = %x, Bytes = %x %x %x %x\n", 
           a, p[0], p[1], p[2], p[3]);
}
void slide61() {
    unsigned int a = 0xAABBCCDD;
    unsigned char *p = (unsigned char *)&a;
    p[2] |= 0xFF;
    printf("%x\n", a);
}

void slide62() {
    int a = -1024;
    unsigned char *p = (unsigned char *)&a;
    printf("%x\n", a);
    p[0] ^= 0xFF;
    p[1] ^= 0xFF;
    printf("%x\n", a);
}

void slide63() {
    unsigned int a = 0xDEADBEEF;
    unsigned int *p = &a;
    *p = (*p << 16) | (*p >> 16);
    printf("%x\n", a);
}

void slide64() {
    int64_t fixedPoint = 0x123456789ABCDEF0;
    unsigned char *p = (unsigned char *)&fixedPoint;
    *(p + 4) &= 0xF0;
    *(p + 5) |= 0x0F;
    *(p + 3) ^= 0xFF;
    *(p + 6) &= 0x3F;

    printf("%lx\n", fixedPoint);
}

void slide65() {
    unsigned char data[] = {0x01, 0x02, 0x03, 0x04};
    unsigned int *p = (unsigned int *)data;
    *p |= 0x00FF0000;
    unsigned short *q = (unsigned short *)(p + 1);
    *(q-1) &= 0xFF00;
    unsigned char *r = (unsigned char *)(q-2);
    *(r + 1) ^= 0x55;
    unsigned char *s = (((unsigned char*)p)+3);
    *(s-1) |= 0xAA;
    unsigned short *t = (unsigned short *)(s-3);
    *t &= 0xFF00;
    printf("%x %x %x %x\n", data[0], data[1], data[2], data[3]);
}

void slide66() {
    unsigned long long a = 0x123456789ABCDEF0;
    unsigned char *p = (unsigned char *)&a;
    printf("%x,%x\n",*(p + 7), *(p + 6));
    *(p + 7) ^= 0xAA;
    *(p + 6) |= 0x0F;
    unsigned int *q = (unsigned int *)(p + 4);
    *q &= ~((0xFF << 16) | (0xF << 4));
    printf("%llx\n", a);
}

void slide67() {
    int arr[] = {0, 1, 2, 3, 4};
    int **p = (int **)arr;
    p[0] = arr + 2;
    printf("%d\n", **p);
}
void slide68() {
    unsigned char *p = (unsigned char *)calloc(16,sizeof(char));
    *(p + 5) = 0x77;
    *(p + 10) = 0x88;
    unsigned int **q = (unsigned int **)&p;
    **q ^= 0xA5A5A5A5;
    unsigned short *r = (unsigned short *)(*q + 1);
    *r |= 0xF00D;
    printf("%x %x %x\n", *(p + 5), *(p + 10), **q);
    free(p);
}

void slide69() {
    unsigned int data[4] = {0x12345678, 0xABCDEF01, 0xDEADBEEF, 0xCAFEBABE};
    unsigned char *p = (unsigned char *)data;
    unsigned short *q = (unsigned short *)(p + 3);
    *(q - 1) ^= 0x0F0F;
    unsigned int **r = (unsigned int **)&q;
    **r &= 0xFFFF0000;
    printf("%x %x %x\n", data[0], data[1], data[2]);
}

void slide70() {
    unsigned int a[2] = {0x12345678, 0x9ABCDEF0};
    unsigned char *p = (unsigned char *)a;
    unsigned char **q = &p;
    for (int i = 0; i < 8; i++) {
        *(*q + i) = (*(*q + i) ^ 0xAA) + (i % 3);
    }
    unsigned short *r = (unsigned short *)(p + 4);
    *r ^= 0x5555;
    printf("%x %x\n", a[0], a[1]);
}

void slide71() {
    unsigned int a = 0xCAFEBABE;
    unsigned char *p = (unsigned char *)&a;
    unsigned short **q = (unsigned short **)&p;
    *(p + 2) = (*(p + 2) >> 2) | 0xC0;
    **q ^= 0xABCD;
    printf("%x\n", a);
}

void slide72() {
    unsigned int data[] = {0x12345678, 0x9ABCDEF0};
    unsigned int *p = data;
    unsigned int **q = &p;
    **q ^= *(*q + 1);
    unsigned char *r = (unsigned char *)(*q + 1);
    *r &= 0xF0;
    printf("%x %x\n", data[0], data[1]);
}

void slide73() {
    unsigned int a = 0xDEADBEEF;
    unsigned short *p = (unsigned short *)&a;
    unsigned short **q = &p;
    **q = (**q >> 4) | ((**q & 0xF) << 12);
    *(p + 1) ^= 0xAAAA;
    printf("%x\n", a);
}

void slide74() {
    int arr[5] = {1, 2, 3, 4, 5};
    int **p = (int**)&arr;
    printf("%d\n", *(*p + 2));
}

void slide75() {
    unsigned int a = 0xAA55AA55;
    unsigned char *p = (unsigned char *)&a;
    for (int i = 0; i < 4; i++) {
        p[i] ^= 0xFF;
    }
    printf("%x\n", a);
}

void slide76() {
    double a = 1.0;
    unsigned char *p = (unsigned char *)&a;
    *(p + 3) ^= 0xFF;
    printf("%lf\n", a);
}

void slide77() {
    int a = 0x12345678;
    int *p = &a;
    *p = (*p & 0x00FF00FF) | ((*p >> 16) & 0xFF00FF00);
    printf("%x\n", a);
}

void slide78() {
    unsigned short arr[] = {0x1234, 0x5678, 0x9ABC};
    unsigned char *p = (unsigned char *)arr;
    *(p + 4) ^= 0xFF;
    printf("%x\n", arr[1]);
}

void slide79() {
    unsigned int a = 0xABCDEF01;
    unsigned char *p = (unsigned char *)&a;
    *p = *p >> 4;
    printf("%x\n", a);
}

void slide80() {
    unsigned int a = 0xFFFFFFFF;
    unsigned int *p = &a;
    *(p + 1) = 0;
    printf("%x\n", a);
}


int main(int argc, char *argv[]) {
    void (*slides[])() = {
    &slide36, &slide37, &slide38, &slide39, &slide40, &slide41, &slide42,
    &slide43, &slide44, &slide45, &slide46, &slide47, &slide48,
    &slide49, &slide50, &slide51, &slide52, &slide53, &slide54, &slide55,
    &slide56, &slide57, &slide58, &slide59, &slide60, &slide61, &slide62,
    &slide63, &slide64, &slide65, &slide66, &slide67, &slide68, &slide69,
    &slide70, &slide71, &slide72, &slide73, &slide74, &slide75, &slide76,
    &slide77, &slide78, &slide79, &slide80
    };
    int slideToRun;

    while (1) {
        // Prompt user for input
        printf("Enter slide number to run (36-80, or 0 to exit): ");
        if (scanf("%d", &slideToRun) != 1) { // Check for valid input
            printf("Invalid input. Please enter a valid slide number.\n");
            while (getchar() != '\n'); // Clear invalid input
            continue;
        }

        // Exit if user enters 0
        if (slideToRun == 0) {
            printf("Exiting program.\n");
            break;
        }

        // Validate the input and execute the corresponding function
        if (slideToRun >= 36 && slideToRun <= 100) {
            printf("Running slide %d:\n", slideToRun);
            (*slides[slideToRun - 36])(); // Call the corresponding slide function
        } else {
            printf("Invalid slide number! Please enter a number between 36 and 80.\n");
        }

        printf("\n"); // Add spacing for readability
    }
}