#define set_bit(reg, bit) reg |= (1 << bit)
#define clear_bit(reg, bit) reg &= ~(1 << bit)
#define check_bit(reg, bit) (reg & (1 << bit))

#define VGA_FRONT_BUFFER 0xC8000000
#define VGA_BACK_BUFFER 0xC000000
#define VGA_BUFFER_LENGTH 0x3BE7E

#define VGA_CONTROL_BASE 0xFF203020
#define VGA_CONTROL_FRONT 0x0
#define VGA_CONTROL_BACK 0x4
#define VGA_STATUS_REG 0xC
#define VGA_STATUS_BIT 0x0

#define VGA_HEIGHT 320
#define VGA_WIDTH 240
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

/***************************************************************************************************
 * DON'T REMOVE THE VARIABLES BELOW THIS COMMENT                                                   *
 **************************************************************************************************/
unsigned long long __attribute__((used)) VGAaddress = 0xc8000000; // Memory storing pixels
unsigned int __attribute__((used)) red = 0x0000F0F0;
unsigned int __attribute__((used)) green = 0x00000F0F;
unsigned int __attribute__((used)) blue = 0x000000FF;
unsigned int __attribute__((used)) white = 0x0000FFFF;
unsigned int __attribute__((used)) black = 0x0;

unsigned char n_cols = 10; // <- This variable might change depending on the size of the game. Supported value range: [1,18]

char *won = "You Won";       // DON'T TOUCH THIS - keep the string as is
char *lost = "You Lost";     // DON'T TOUCH THIS - keep the string as is
unsigned short height = 240; // DON'T TOUCH THIS - keep the value as is
unsigned short width = 320;  // DON'T TOUCH THIS - keep the value as is
char font8x8[128][8];        // DON'T TOUCH THIS - this is a forward declaration
/**************************************************************************************************/

/***
 * TODO: Define your variables below this comment
 */

struct VGA {
    unsigned int** frontBuffer;
    unsigned int** backBuffer;
    unsigned int* status;
};
/***
 * You might use and modify the struct/enum definitions below this comment
 */
typedef struct _block
{
    unsigned char destroyed;
    unsigned char deleted;
    unsigned int pos_x;
    unsigned int pos_y;
    unsigned int color;
} Block;

typedef enum _gameState
{
    Stopped = 0,
    Running = 1,
    Won = 2,
    Lost = 3,
    Exit = 4,
} GameState;
GameState currentState = Stopped;

/***
 * Here follow the C declarations for our assembly functions
 */



//Pixel-level declarations
void setScreenColor(unsigned int** backBuffer, unsigned int color);
void setPixel(unsigned int** backBuffer, unsigned int xCoord, unsigned int yCoord, unsigned int color);

//Game-level declarations
void drawBlock(unsigned int** backBuffer, unsigned int xCoord, unsigned int yCoord, unsigned int width, unsigned int height, unsigned int color);
void drawBar(unsigned int** backBuffer,unsigned int y);

//VGA declarations
struct VGA* initVGA();

//UART declarations
int readUart();
void writeUart(char c);




//Pixel-level definitions

/*
Inline asssembly, option 1: 

Here I represent one way of doing inline assembly, the best way in my opinion is to use the __asm__ construct, as it is more readable and easier to understand.
The output parameters in the first :, the inputs after the second :, and the clobberend callee registers at the last :
*/

void setScreenColor(unsigned int** backBuffer, unsigned int color){
    unsigned long long backBufferLength = (unsigned long long)VGA_BUFFER_LENGTH;
    __asm__(
        "ldr r4,[%[backBuf]]\n\t" //Gives the adress for the start of the back buffer
        "mov r5,#0x0\n\t"       //Iterator in bytes
        "mov r6,%[length]\n\t"      
        "mov r7,%[col]\n\t"

        "sSCloop:\n\t"
        "strh r7,[r4,r5]\n\t"
        "add r5,r5,#0x2\n\t"
        "cmp r5,r6\n\t"
        "bne sSCloop\n\t"
        : //No output
        : [backBuf] "r"(backBuffer), [length]"r"(backBufferLength), [col] "r"(color) // Input
        : "r4", "r5", "r6", "r7" // Clobbered registers
    );
}
void setPixel(unsigned int** backBuffer, unsigned int xCoord, unsigned int yCoord, unsigned int color){
    __asm__(
        "ldr r4, [%[backBuf]]\n\t"
        "mov r5, %[x]\n\t"
        "mov r6, %[y]\n\t"
        "lsl r5, r5, #1\n\t" //lsl x
        "lsl r6, r6, #10\n\t"//lsl y
        "add r5, r5, r6\n\t"

        "mov r7, %[col]\n\t"
        "strh r7, [r4, r5]\n\t"
        : //No output
        : [backBuf] "r" (backBuffer), [x] "r" (xCoord), [y] "r" (yCoord), [col] "r" (color)
        : "r4", "r5", "r6", "r7" // Clobbered registers
    );
}


/*
Inline assembly, option 2: 

Here i represent another way of doing inline assembly, using the asm() and implementing the functuion at the label of very same function
    Seems here that you want us to explicitly use the r0-r3 for parameters, in addition to pushing the last to stack, so i will do that, even that can be taken care of by the constructs above
    As i am using the back buffer, 2 parameters needs to be pushed and popped to/from stack
*/

//Assumes: backBuffer at r0, xCoord at r1, yCoord at r2, width at r3, height at [r13], color at [r13,#0x4]
asm(
    "drawBlock: \n\t"
    "push {lr}\n\t"
    "push {r4-r9}\n\t"
    "mov r4, r3\n\t" //width
    "ldr r5,[r13,#0x1C]\n\t" //height
    "ldr r3,[r13,#0x20]\n\t" //color

    "mov r6, r1\n\t" //Initial coordiante of x
    "mov r7, r2\n\t" //initial coordiante of y
    "mov r8, #0x0\n\t" //Iterator for x
    "mov r9, #0x0\n\t" //Iterator for y

    "dBloop:\n\t"
    "bl setPixel\n\t"
    "add r1, r6, r8\n\t"
    "add r8, r8, #0x1\n\t"
    "cmp r8, r4\n\t"
    "bne dBloop\n\t"
    "add r2, r7, r9\n\t"
    "add r9, r9, #0x1\n\t"
    "mov r8, #0x0\n\t"
    "cmp r9, r5\n\t"
    "bne dBloop\n\t"
    
    "pop {r4-r9}\n\t"
    "pop {lr}\n\t"
    //"pop {r0,r1}\n\t" //THESE ARE THE PROBLEM LINES!//Has to be like this, as in the native C noone can pop for that one
    "bx lr\n\t"
);
//Important: the native does NOT push and pop onto stack, aka dec and inc on the stack pointer, it just uses the offset
// TODO: Impelement the DrawBar function in assembly. You need to accept the parameter as outlined in the c declaration above (unsigned int y)
asm(
    "drawBar: \n\t"
    "push {lr}\n\t"
    "push {r4,r5}\n\t"
    
    "mov r2,r1\n\t"
    "mov r1,#0\n\t"
    "mov r3,#7\n\t"
    "mov r4,#45\n\t"
    "ldr r5,=white\n\t"
    "ldr r5,[r5]\n\t"
    "push {r4,r5}\n\t"
    "bl drawBlock\n\t"
	"pop {r0,r1}\n\t" //add r13,13,0x8 would also do the trick

	"pop {r4,r5}\n\t"
    "pop {lr}\n\t"
    "bx lr"
);

asm("ReadUart:\n\t"
    "LDR R1, =0xFF201000 \n\t"
    "LDR R0, [R1]\n\t"
    "BX LR");

//Game-level definitions
void initGame(){
    //Initializes a new game
}

//VGA definitions
struct VGA* initVGA(){
    struct VGA* controller = (struct VGA*)malloc(sizeof(struct VGA));
    controller->frontBuffer = (unsigned int**)(VGA_CONTROL_BASE+VGA_CONTROL_FRONT);
    controller->backBuffer = (unsigned int**)(VGA_CONTROL_BASE+VGA_CONTROL_BACK);
    controller->status = (unsigned int**)(VGA_CONTROL_BASE+VGA_STATUS_REG);

    *(controller->frontBuffer) = (unsigned int*)VGA_FRONT_BUFFER;
    *(controller->backBuffer) = (unsigned int*)VGA_BACK_BUFFER;

    return controller;
}

// Interrupts was not avalibale for the S bit here, so this is pollong based :(
void checkForBufferSwitch(unsigned int* status,unsigned int** frontBuffer){
    if(!(check_bit(*status, VGA_STATUS_BIT))){
        *frontBuffer = (unsigned int*)0x1;
    }
}


int main(int argc, char *argv[])
{
    struct VGA vga = *initVGA();
    
    while(1){
        setScreenColor(vga.frontBuffer,black);
        drawBlock(vga.frontBuffer, 100, 100, 10, 10, white);
        drawBar(vga.frontBuffer, 100);
    }
}
