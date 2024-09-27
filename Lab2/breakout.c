#include <stdlib.h>
#include <unistd.h>

#define set_bit(reg, bit) reg |= (1 << bit)
#define clear_bit(reg, bit) reg &= ~(1 << bit)
#define check_bit(reg, bit) (reg & (1 << bit))

#define RGB_TO_COLOR(r, g, b) (((r) << 16) | ((g) << 8) | (b))

#define N_COLS 2
#define N_ROWS 15

#define VGA_FRONT_BUFFER 0xC8000000
#define VGA_BACK_BUFFER 0xC0000000
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
#define TIMER_PRESCALER_BIT 8+0

#define VGA_CONTROL_BASE 0xFF203020
#define VGA_CONTROL_FRONT 0x0
#define VGA_CONTROL_BACK 0x4
#define VGA_STATUS_REG 0xC
#define VGA_STATUS_BIT 0x0

#define VGA_WIDTH 320
#define VGA_HEIGHT 240

#define BLOCK_BASE 306
#define BLOCK_WIDTH 15
#define BLOCK_HEIGHT 15
#define BAR_WIDTH 7
#define BAR_UNIT_HEIGHT 15
#define BALL_WIDTH 7
#define BALL_HEIGHT 7
#define BLOCK_OFFSET 5
#define BAR_OFFSET 5

#define NUM_WALLS 4
#define NUM_BARS 3
#define NUM_BALLS

#define MAX_HITPOINTS 4
#define MOVING_ITEMS_COUNT 2

#define JTAG_UART_DATA_REG    (*((volatile unsigned int *) 0xFF201000))
#define JTAG_UART_CONTROL_REG (*((volatile unsigned int *) 0xFF201004))
#define RVALID_BIT    15
#define WSPACE_START  16




/***************************************************************************************************
 * DON'T REMOVE THE VARIABLES BELOW THIS COMMENT                                                   *
 **************************************************************************************************/
unsigned long long __attribute__((used)) VGAaddress = 0xc8000000; // Memory storing pixels
unsigned int __attribute__((used)) red = 0x0000F0F0;
unsigned int __attribute__((used)) green = 0x00000F0F;
unsigned int __attribute__((used)) blue = 0x000000FF;
unsigned int __attribute__((used)) white = 0x0000FFFF;
unsigned int __attribute__((used)) black = 0x0;

const unsigned char n_cols = 2; // <- This variable might change depending on the size of the game. Supported value range: [1,18]
unsigned char n_rows = 15;

char *won = "You Won";       // DON'T TOUCH THIS - keep the string as is
char *lost = "You Lost";     // DON'T TOUCH THIS - keep the string as is
unsigned short height = 240; // DON'T TOUCH THIS - keep the value as is
unsigned short width = 320;  // DON'T TOUCH THIS - keep the value as is
char font8x8[128][8];        // DON'T TOUCH THIS - this is a forward declaration
/**************************************************************************************************/

/***
 * TODO: Define your variables below this comment
 */
unsigned int numItems;

typedef struct Vector
{
    int x;
    int y;
} Vector;

typedef struct VGA {
    unsigned int** frontBuffer;
    unsigned int** backBuffer;
    unsigned int* status;
} VGA;
typedef enum ItemType
{
    ITEMTYPE_NONE,
    ITEMTYPE_BLOCK,
    ITEMTYPE_UPPERBAR,
    ITEMTYPE_MIDBAR,
    ITEMTYPE_LOWERBAR,
    ITEMTYPE_BALL,
    ITEMTYPE_BOUNDARY_WALL,
    ITEMTYPE_WINNING_WALL,
    ITEMTYPE_LOSING_WALL,
} ItemType;
typedef struct Item
{
    ItemType type;
    unsigned int color;
    int xPos;
    int yPos;
    unsigned int width;
    unsigned int height;
    Vector direction;
} Item;

typedef enum Point
{
    NONE,UPPER,LOWER,RIGHT,LEFT,
}Point;

typedef struct HitPoint
{
    Point point;
    Item* item;
} HitPoint;

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
    Item** blocks;
    Item* bars;
    Item* walls;
    Item ball;
    GameState state;
} Game;


/***
 * Here follow the C declarations for our assembly functions
 */



//Pixel-level declarations
void setScreenColor(unsigned int** backBuffer, unsigned int color);
void setPixel(unsigned int** backBuffer, unsigned int xCoord, unsigned int yCoord, unsigned int color);
unsigned int rgbToColor(unsigned char r, unsigned char g, unsigned char b);

//Game-level declarations
void drawBlock(unsigned int** backBuffer, unsigned int xCoord, unsigned int yCoord, unsigned int width, unsigned int height, unsigned int color);
void drawBar(unsigned int** backBuffer,unsigned int y);
void drawGame(Game* game, unsigned int** buffer);
void moveItem(Item* item, unsigned int** buffer);


//VGA declarations
struct VGA* initVGA();

//UART declarations
char uartReadChar();
void writeChar(char c);




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

    "add r4, r4, #1\n\t"    //We want to draw the block, including the edge
    "add r5, r5, #1\n\t"    //We want to draw the block, including the edge

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

Game* initGame(){
    numItems = (n_rows * n_cols) + NUM_BALLS+NUM_WALLS+NUM_BARS;
    Item** blocks = (Item**)malloc(n_rows*sizeof(Item*));
    for(int i = 0; i < n_rows; i++){
        blocks[i] = (Item*)malloc(n_cols*sizeof(Item));
    }
    for(int i = 0; i < n_rows; i++){
        for(int j = 0; j < n_cols; j++){
            unsigned int blockHeight;
            blocks[i][j] = (Item){
                .type = ITEMTYPE_BLOCK,
                .color = rgbToColor(255-(10*i),(10*i),(10*j)),
                .xPos = BLOCK_BASE-(j*(BLOCK_WIDTH+2))-BLOCK_OFFSET,
                .yPos = 1+i*(BLOCK_HEIGHT+1),
                .width = BLOCK_WIDTH,
                .height = BLOCK_HEIGHT
            };
        }
    }
    volatile Item* bars = (Item*)malloc(3*sizeof(Item));
    bars[0]=(Item){
        .type = ITEMTYPE_UPPERBAR,
        .color = blue,
        .xPos = BAR_OFFSET,
        .yPos = 0,
        .width = BAR_WIDTH,
        .height = BAR_UNIT_HEIGHT
    };
    bars[1] = (Item){
        .type = ITEMTYPE_MIDBAR,
        .color = green,
        .xPos = bars[0].xPos,
        .yPos = bars[0].yPos+BAR_UNIT_HEIGHT,
        .width = BAR_WIDTH,
        .height = BAR_UNIT_HEIGHT
    };
    bars[2] = (Item){
        .type = ITEMTYPE_LOWERBAR,
        .color = blue,
        .xPos = bars[0].xPos,
        .yPos = bars[0].yPos+(2*BAR_UNIT_HEIGHT),
        .width = BAR_WIDTH,
        .height = BAR_UNIT_HEIGHT
    };

    Item ball = (Item){
        .type = ITEMTYPE_BALL,
        .color = red,
        .xPos = 200,
        .yPos = 70,
        .width = BALL_WIDTH,
        .height = BALL_HEIGHT,
        .direction = (Vector){
            .x = 1,
            .y = 1
        }
    };

    volatile Item* walls = (Item*)malloc(4*sizeof(Item));
    walls[0] = (Item){
        .type = ITEMTYPE_BOUNDARY_WALL,
        .color = white,
        .xPos = 0,
        .yPos = 0,
        .width = VGA_WIDTH,
        .height = 1
    };
    walls[1] = (Item){
        .type = ITEMTYPE_BOUNDARY_WALL,
        .color = white,
        .xPos = 0,
        .yPos = VGA_HEIGHT-1,
        .width = VGA_WIDTH,
        .height = 1
    };
    walls[2] = (Item){
        .type = ITEMTYPE_WINNING_WALL,
        .color = rgbToColor(0,255,0),
        .xPos = VGA_WIDTH-1,
        .yPos = 0,
        .width = 1,
        .height = VGA_HEIGHT
    };
    walls[3] = (Item){
        .type = ITEMTYPE_LOSING_WALL,
        .color = rgbToColor(255,0,0),
        .xPos = 0,
        .yPos = 0,
        .width = 1,
        .height = VGA_HEIGHT
    };
    volatile Game* game = (Game*)malloc(sizeof(Game));
    *game = (Game){
        .blocks = blocks,
        .bars = bars,
        .walls = walls,
        .ball = ball,
        .state = Stopped
    };
    return game;
}

void drawGame(Game* game,unsigned int** buffer){
    for(int i = 0; i < n_rows; i++){
        for(int j = 0; j < n_cols; j++){
            drawBlock(buffer, game->blocks[i][j].xPos, game->blocks[i][j].yPos, game->blocks[i][j].width, game->blocks[i][j].height, game->blocks[i][j].color);
        }
    }
    for(int i = 0; i < NUM_BARS; i++){
        drawBlock(buffer, game->bars[i].xPos, game->bars[i].yPos, game->bars[i].width, game->bars[i].height, game->bars[i].color);
    }
    for(int i = 0; i<NUM_WALLS; i++){
        drawBlock(buffer, game->walls[i].xPos, game->walls[i].yPos, game->walls[i].width, game->walls[i].height, game->walls[i].color);
    }
    drawBlock(buffer, game->ball.xPos, game->ball.yPos, game->ball.width, game->ball.height, game->ball.color);
}


unsigned int rgbToColor(unsigned char r, unsigned char g, unsigned char b) {
    r = r & 0x1F; 
    g = g & 0x3F; 
    b = b & 0x1F; 

    return (r << 11) | (g << 5) | (b);
}


Item* getItemAt(Game* game, unsigned int x, unsigned int y){
    Item* item = NULL;
    for(int i = 0; i < n_rows; i++){
        for(int j = 0; j < n_cols; j++){
            if((x >= game->blocks[i][j].xPos && x <= game->blocks[i][j].xPos+game->blocks[i][j].width && y >= game->blocks[i][j].yPos && y <= game->blocks[i][j].yPos+game->blocks[i][j].height) 
            && game->blocks[i][j].color != white){
                item = &game->blocks[i][j];
            }
        }
    }
    for(int i = 0; i < NUM_BARS; i++){
        if(x >= game->bars[i].xPos && x <= game->bars[i].xPos+game->bars[i].width && y >= game->bars[i].yPos && y <= game->bars[i].yPos+game->bars[i].height){
            item = &game->bars[i];
        }
    }
    for (int i = 0; i < NUM_WALLS; i++){
        if(x >= game->walls[i].xPos && x <= game->walls[i].xPos+game->walls[i].width && y >= game->walls[i].yPos && y <= game->walls[i].yPos+game->walls[i].height){
            item = &game->walls[i];
        }
    }
    return item;
}

Point getPoint(int i, int j) {
    if(j==0 && i==0){
        return UPPER;
    } else if(j==0 && i==1){
        return RIGHT;
    } else if(j==1 && i==0){
        return LEFT;
    } else if(j==1 && i==1){
        return LOWER;
    }
    return NONE;
}

HitPoint* updateHitPoints(Game* game, unsigned int** frontBuffer, HitPoint* hitPoints){
    //Iterate through all corner pixels of the ball, checking for overlaps with blocks
    unsigned int currentHitCount = 0;
    const unsigned int sensePixelDistance = (unsigned int)(BALL_WIDTH-1)/2;
    unsigned int senseX = 0;
    unsigned int senseY = 0;

    for(int i = 0; i< 2;i++){
        for(int j = 0; j<2;j++){
            if(j == 0){ //Doing double component for x
                senseX = (game->ball.xPos+sensePixelDistance) + (i*(sensePixelDistance+1));
                senseY = game->ball.yPos+(i*(sensePixelDistance));
            } 
            else if (j == 1){ //Doing double component for y
                senseX = game->ball.xPos+(i*(sensePixelDistance));
                senseY = (game->ball.yPos+sensePixelDistance) + (i*(sensePixelDistance+1));
            }
            hitPoints[currentHitCount] = (HitPoint){
                .point = getPoint(i,j),
                .item = getItemAt(game, senseX, senseY)
            };
            if(hitPoints[currentHitCount].item != NULL){
                currentHitCount++;
            }
        }
    }
    return hitPoints;
}
void updateBallDirection(Game* game, HitPoint* hitPoints){
    //Find the number of centers hit
    unsigned int currentHitCount = 0;
    for(int i = 0; i < MAX_HITPOINTS; i++){
        if(hitPoints[i].item != NULL){
            currentHitCount++;
            uartWriteChar((char)currentHitCount);
        }
    }
    Vector currentDirection = game->ball.direction;
    if(hitPoints[0].item != NULL){
        switch(hitPoints[0].item->type){
            case ITEMTYPE_BLOCK:
                switch (currentHitCount)
                    {
                    case 0:
                        game->ball.direction = (Vector){currentDirection.x, currentDirection.y};
                    case 1:
                        switch (hitPoints[0].point)
                        {
                        case UPPER:
                            game->ball.direction = (Vector){currentDirection.x, -currentDirection.y};
                            break;
                        case LOWER:
                            game->ball.direction = (Vector){currentDirection.x, -currentDirection.y};
                            break;
                        case RIGHT:
                            game->ball.direction = (Vector){-currentDirection.x, currentDirection.y};
                            break;
                        case LEFT:
                            game->ball.direction = (Vector){-currentDirection.x, currentDirection.y};
                            break;
                        default:
                            break;
                        }
                        break;
                    case 2:
                        game->ball.direction = (Vector){-currentDirection.x, -currentDirection.y};
                        break;
                    }
            case ITEMTYPE_MIDBAR:
                game->ball.direction = (Vector){-currentDirection.x, currentDirection.y};
                break;
            case ITEMTYPE_UPPERBAR:
                game->ball.direction = (Vector){1, 1};
                break;
            case ITEMTYPE_LOWERBAR:
                game->ball.direction = (Vector){1, -1};
                break;
            case ITEMTYPE_BOUNDARY_WALL:
                game->ball.direction = (Vector){currentDirection.x, -currentDirection.y};
                break;
            case ITEMTYPE_WINNING_WALL:
                game->ball.direction = (Vector){0,0};
                game->state = Won;
                break;
            case ITEMTYPE_LOSING_WALL:
                game->ball.direction = (Vector){0,0};
                game->state = Lost;
                break;
            default:
                break;
        }
    }
}

void updateBarDirection(Game* game, unsigned int** buffer){
    char input = uartReadChar();
    if(input == 'w'){
        for(int i = 0; i < 3; i++){
            if(game->bars[i].yPos > 0){
                game->bars[i].direction.y -= 5;
            }
            
        }
        for(int i = 0; i < 3; i++){
            moveItem(&(game->bars[i]), buffer);
        }
    } else if(input == 's'){
        for(int i = 0; i < 3; i++){
            if(game->bars[i].yPos+game->bars[i].height < VGA_HEIGHT){
                game->bars[i].direction.y += 5;
            }
        }
        for(int i = 2; i >= 0; i--){
            moveItem(&(game->bars[i]), buffer);
        }
    } else {
        for(int i = 0; i < 3; i++){
            game->bars[i].direction.y = 0;
        }
    }
}

void updateBlocks(HitPoint* hitPoints, unsigned int** buffer){
    for(int i = 0; i < MAX_HITPOINTS; i++){
        if(hitPoints[i].item->type == ITEMTYPE_BLOCK){
            hitPoints[i].item->color = white;
            drawBlock(buffer, hitPoints[i].item->xPos, hitPoints[i].item->yPos, hitPoints[i].item->width, hitPoints[i].item->height, hitPoints[i].item->color);
        }
    }
}

void moveItem(Item* item,unsigned int** buffer){
    drawBlock(buffer, item->xPos, item->yPos, item->width, item->height, white);
    item->xPos += item->direction.x;
    item->yPos += item->direction.y;
    drawBlock(buffer, item->xPos, item->yPos, item->width, item->height, item->color);
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
void loadTimer(unsigned int frequency){
    unsigned int* timerCtrl = (unsigned int*)(TIMER_BASE+TIMER_CONTROL);
    clear_bit(*timerCtrl, TIMER_ENABLE_BIT);
    set_bit(*timerCtrl, TIMER_AUTO_BIT);
    set_bit(*timerCtrl, TIMER_PRESCALER_BIT);

    unsigned int* timerLoad = (unsigned int*)(TIMER_BASE+TIMER_LOAD);
    *timerLoad = ((unsigned int)TIMER_FREQUENCY)/frequency;

    set_bit(*timerCtrl, TIMER_ENABLE_BIT);
}

// Function to write a character to the JTAG UART
void uartWriteChar(char c) {
    while (((JTAG_UART_CONTROL_REG >> WSPACE_START) & 0xFFFF) == 0) {}
    JTAG_UART_DATA_REG = (unsigned int) c;
}

void uartWriteString(const char* str) {
    while (*str) {
        uartWriteChar(*str);
        str++;
    }
}
char uartReadChar() {
    if(check_bit(JTAG_UART_DATA_REG, RVALID_BIT)) {
        return (char)(JTAG_UART_DATA_REG & 0xFF);
    } else {
        return '\0';
    }
}

int main(int argc, char *argv[])
{
    VGA* vga = initVGA();
    Game* game = initGame();
    loadTimer(60);
    HitPoint* hitPoints = (HitPoint*)calloc(MAX_HITPOINTS,sizeof(HitPoint));
    uartWriteString("Game started\n");

    setScreenColor(vga->backBuffer, white);
    drawGame(game,vga->frontBuffer);

    while (game->state != Exit) {
        if (checkTimer()) {
            updateHitPoints(game,vga->frontBuffer,hitPoints);
            updateBallDirection(game, hitPoints);
            updateBarDirection(game,vga->frontBuffer);

            updateBlocks(hitPoints, vga->frontBuffer);

            moveItem(&(game->ball), vga->frontBuffer);
        }
    }  
    free(hitPoints);
    for(int i = 0; i < n_rows; i++){
        free(game->blocks[i]);
    }
    free(game->blocks);
    free(game);
    free(vga);
}
