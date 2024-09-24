#include <stdlib.h>
#include <unistd.h>

#define set_bit(reg, bit) reg |= (1 << bit)
#define clear_bit(reg, bit) reg &= ~(1 << bit)
#define check_bit(reg, bit) (reg & (1 << bit))

#define VGA_FRONT_BUFFER 0xC8000000
#define VGA_BACK_BUFFER 0xC000000
#define VGA_BUFFER_LENGTH 0x3BE7E

#define TIMER_BASE 0xFFFEC600
#define TIMER_LOAD 0x0
#define TIMER_VALUE 0x4
#define TIMER_CONTROL 0x8
#define TIMER_STATUS 0xC
#define TIMER_FREQUENCY 200000000
#define TIMER_ENABLE_BIT 0x0
#define TIMER_STATUS_BIT 0x2
#define TIMER_AUTO_BIT 0x1
#define TIMER_FLAG_BIT 0x0

#define VGA_CONTROL_BASE 0xFF203020
#define VGA_CONTROL_FRONT 0x0
#define VGA_CONTROL_BACK 0x4
#define VGA_STATUS_REG 0xC
#define VGA_STATUS_BIT 0x0

#define VGA_WIDTH 320
#define VGA_HEIGHT 240

#define BLOCK_WIDTH 15
#define BLOCK_HEIGHT 15
#define BAR_WIDTH 45
#define BAR_HEIGHT 7
#define BALL_WIDTH 7
#define BALL_HEIGHT 7



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
unsigned char n_rows = 5;

char *won = "You Won";       // DON'T TOUCH THIS - keep the string as is
char *lost = "You Lost";     // DON'T TOUCH THIS - keep the string as is
unsigned short height = 240; // DON'T TOUCH THIS - keep the value as is
unsigned short width = 320;  // DON'T TOUCH THIS - keep the value as is
char font8x8[128][8];        // DON'T TOUCH THIS - this is a forward declaration
/**************************************************************************************************/

/***
 * TODO: Define your variables below this comment
 */

typedef struct VGA {
    unsigned int** frontBuffer;
    unsigned int** backBuffer;
    unsigned int* status;
} VGA;

typedef struct Block
{
    unsigned int color;
    unsigned int xPos;
    unsigned int yPos;
    unsigned int width;
    unsigned int height;
} Block;

typedef struct Bar
{
    unsigned int color;
    unsigned int xPos;
    unsigned int yPos;
    unsigned int width;
    unsigned int height;
} Bar;

typedef struct Ball
{
    unsigned int color;
    unsigned int xPos;
    unsigned int yPos;
    unsigned int width;
    unsigned int height;
} Ball;

typedef enum GameState
{
    Stopped = 0,
    Running = 1,
    Won = 2,
    Lost = 3,
    Exit = 4,
} GameState;

typedef struct Game
{
    Block** blocks;
    Bar bar;
    Ball ball;
    GameState state;
} Game;


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
    "bx lr\n\t"
);
asm(
    "drawBar: \n\t"
    "push {lr}\n\t"
    "push {r4,r5}\n\t"
    
    "mov r2,r1\n\t"
    "mov r1,#0\n\t"
    "mov r3,#7\n\t"
    "mov r4,#45\n\t"
    "ldr r5,=black\n\t"
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


//VGA definitions
VGA* initVGA(){
    VGA* controller = (VGA*)malloc(sizeof(VGA));
    controller->frontBuffer = (unsigned int**)(VGA_CONTROL_BASE+VGA_CONTROL_FRONT);
    controller->backBuffer = (unsigned int**)(VGA_CONTROL_BASE+VGA_CONTROL_BACK);
    controller->status = (unsigned int*)(VGA_CONTROL_BASE+VGA_STATUS_REG);

    *(controller->frontBuffer) = (unsigned int*)VGA_FRONT_BUFFER;
    *(controller->backBuffer) = (unsigned int*)VGA_BACK_BUFFER;

    return controller;
}

Game initGame(){
    Block** blocks = (Block**)malloc(n_rows*sizeof(Block*));
    for(int i = 0; i < n_rows; i++){
        blocks[i] = (Block*)malloc(n_cols*sizeof(Block));
    }
    for(int i = 0; i < n_rows; i++){
        for(int j = 0; j < n_cols; j++){
            blocks[i][j] = (Block){
                .color = black,
                .xPos = (VGA_WIDTH-BALL_WIDTH)-(j*(BLOCK_WIDTH+1)),
                .yPos = i*(BLOCK_HEIGHT+1),
                .width = BLOCK_WIDTH,
                .height = BLOCK_HEIGHT
            };
        }
    }
    Bar bar = (Bar){
        .color = blue,
        .xPos = 0,
        .yPos = 0,
        .width = BAR_WIDTH,
        .height = BAR_HEIGHT
    };
    Ball ball = (Ball){
        .color = red,
        .xPos = 100,
        .yPos = 100,
        .width = BALL_WIDTH,
        .height = BALL_HEIGHT
    };
    Game game = (Game){
        .blocks = blocks,
        .bar = bar,
        .ball = ball,
        .state = Stopped
    };
    return game;
}

void drawGame(unsigned int** buffer, Game game){
    for(int i = 0; i < n_rows; i++){
        for(int j = 0; j < n_cols; j++){
            drawBlock(buffer, game.blocks[i][j].xPos, game.blocks[i][j].yPos, game.blocks[i][j].width, game.blocks[i][j].height, game.blocks[i][j].color);
        }
    }
    drawBlock(buffer, game.bar.xPos, game.bar.yPos, game.bar.width, game.bar.height, game.bar.color);
    drawBlock(buffer, game.ball.xPos, game.ball.yPos, game.ball.width, game.ball.height, game.ball.color);
}

void freeResources(){
    //VGA
    //Blocks
}
// Interrupts was not avalibale for the S bit here, so this is polling based :(
void checkForBufferSwitch(unsigned int* status,unsigned int** frontBuffer){
    if(!(check_bit(*status, VGA_STATUS_BIT))){
        *frontBuffer = (unsigned int*)0x1;
    }
}

int checkTimer(){
    unsigned int* timerStatus = (unsigned int*)(TIMER_BASE+TIMER_STATUS);
    if(check_bit(*timerStatus, TIMER_FLAG_BIT)){
        set_bit(*timerStatus, TIMER_FLAG_BIT);
        return 1;
    } else {
        return 0;
    }
}
void loadTimer(){
    unsigned int* timerCtrl = (unsigned int*)(TIMER_BASE+TIMER_CONTROL);
    clear_bit(*timerCtrl, TIMER_ENABLE_BIT);
    set_bit(*timerCtrl, TIMER_AUTO_BIT);

    unsigned int* timerLoad = (unsigned int*)(TIMER_BASE+TIMER_LOAD);
    *timerLoad = 3*TIMER_FREQUENCY;   //Gives us one second of delay

    set_bit(*timerCtrl, TIMER_ENABLE_BIT);
}

//UART definitions
//rewriteReadUart to c, along with its writeUart companion

int main(int argc, char *argv[])
{
    VGA vga = *initVGA();
    Game game = initGame();
    loadTimer();
    while(1){
        setScreenColor(vga.backBuffer, white);
        drawGame(vga.backBuffer, game);

        if (checkTimer()){
            checkForBufferSwitch(vga.status, vga.frontBuffer);
        }
    }
}
