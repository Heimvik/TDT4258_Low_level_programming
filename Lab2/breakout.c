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
#define END_TIME 11

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
#define BAR_OFFSET 2

#define NUM_WALLS 4
#define NUM_BARS 3
#define NUM_BALLS 1

#define MAX_HITPOINTS 4
#define MOVING_ITEMS_COUNT 2

#define JTAG_UART_BASE 0xFF201000
#define JTAG_UART_DATA 0x0
#define JTAG_UART_CONTROL 0x4
#define JTAG_UART_RVALID_BIT    15
#define JTAG_UART_WSPACE_START  16

#define CHAR_BUFFER_BASE 0xC9000000
#define CHAR_BOX_X 8
#define CHAR_BOX_Y 8
#define MAX_CHAR_X 79
#define MAX_CHAR_Y 59



/***************************************************************************************************
 * DON'T REMOVE THE VARIABLES BELOW THIS COMMENT                                                   *
 **************************************************************************************************/
unsigned long long __attribute__((used)) VGAaddress = 0xc8000000; // Memory storing pixels
unsigned int __attribute__((used)) red = 0x0000F0F0;
unsigned int __attribute__((used)) green = 0x00000F0F;
unsigned int __attribute__((used)) blue = 0x000000FF;
unsigned int __attribute__((used)) white = 0x0000FFFF;
unsigned int __attribute__((used)) black = 0x0;

const unsigned char n_cols = 18; // <- This variable might change depending on the size of the game. Supported value range: [1,18]
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
    ITEM_NONE,
    ITEM_BLOCK,
    ITEM_UPPERBAR,
    ITEM_MIDBAR,
    ITEM_LOWERBAR,
    ITEM_BALL,
    ITEM_BOUNDARY_WALL,
    ITEM_WINNING_WALL,
    ITEM_LOSING_WALL,
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
    POINT_NONE,POINT_UPPER_RIGHT,POINT_UPPER_LEFT,POINT_LOWER_RIGHT,POINT_LOWER_LEFT
}Point;

typedef enum Corner
{
    CORNER_NONE,CORNER_UPPER_RIGHT,CORNER_UPPER_LEFT,CORNER_LOWER_RIGHT,CORNER_LOWER_LEFT
}Corner;

typedef enum Side
{
    SIDE_NONE,SIDE_TOP,SIDE_BOTTOM,SIDE_LEFT,SIDE_RIGHT
} Side;

typedef struct HitPoint
{
    Point point;
    Item* item;
} HitPoint;

typedef enum GameState
{
    GAMESTATE_INIT,
    GAMESTATE_RUNNING,
    GAMESTATE_WON,
    GAMESTATE_LOST,
    GAMESTATE_EXIT
} GameState;

typedef struct Game
{
    Item** blocks;
    Item* bars;
    Item* walls;
    Item ball;
    GameState state;
    unsigned int speed;
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
void loadTimer(unsigned int frequency);


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
    "ldr r5,=0x0\n\t"
    "ldr r5,[r5]\n\t"
    "push {r4,r5}\n\t"
    "bl drawBlock\n\t"
	"pop {r0,r1}\n\t" //add r13,13,0x8 would also do the trick

	"pop {r4,r5}\n\t"
    "pop {lr}\n\t"
    "bx lr"
);


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
    red = rgbToColor(255, 0, 0);
    green = rgbToColor(0, 255, 0);
    blue = rgbToColor(0, 0, 255);
    unsigned int orange = rgbToColor(255, 165, 0);
    Item** blocks = (Item**)malloc(n_rows*sizeof(Item*));
    for(int i = 0; i < n_rows; i++){
        blocks[i] = (Item*)malloc(n_cols*sizeof(Item));
    }
    for(int i = 0; i < n_rows; i++){
        for(int j = 0; j < n_cols; j++){
            unsigned int greyTone = 255 - ((i + j) * 255 / (n_rows + n_cols - 2));
            blocks[i][j] = (Item){
                .type = ITEM_BLOCK,
                .color = rgbToColor(greyTone, greyTone, greyTone),  // Grey tone
                .xPos = BLOCK_BASE - (j * (BLOCK_WIDTH + 2)) - BLOCK_OFFSET,
                .yPos = 1 + i * (BLOCK_HEIGHT + 1),
                .width = BLOCK_WIDTH,
                .height = BLOCK_HEIGHT
            };
        }
    }
    Item* bars = (Item*)malloc(NUM_BARS*sizeof(Item));
    bars[0]=(Item){
        .type = ITEM_UPPERBAR,
        .color = blue,
        .xPos = BAR_OFFSET,
        .yPos = 0,
        .width = BAR_WIDTH,
        .height = BAR_UNIT_HEIGHT
    };
    bars[1] = (Item){
        .type = ITEM_MIDBAR,
        .color = green,
        .xPos = bars[0].xPos,
        .yPos = bars[0].yPos+BAR_UNIT_HEIGHT,
        .width = BAR_WIDTH,
        .height = BAR_UNIT_HEIGHT
    };
    bars[2] = (Item){
        .type = ITEM_LOWERBAR,
        .color = blue,
        .xPos = bars[0].xPos,
        .yPos = bars[0].yPos+(2*BAR_UNIT_HEIGHT),
        .width = BAR_WIDTH,
        .height = BAR_UNIT_HEIGHT
    };

    Item ball = (Item){
        .type = ITEM_BALL,
        .color = orange,
        .xPos = BAR_WIDTH+2,
        .yPos = BAR_UNIT_HEIGHT+5,
        .width = BALL_WIDTH,
        .height = BALL_HEIGHT,
        .direction = (Vector){
            .x = 1,
            .y = 1
        }
    };

    Item* walls = (Item*)malloc(NUM_WALLS*sizeof(Item));
    walls[0] = (Item){
        .type = ITEM_BOUNDARY_WALL,
        .color = white,
        .xPos = 0,
        .yPos = 0,
        .width = VGA_WIDTH,
        .height = 1
    };
    walls[1] = (Item){
        .type = ITEM_BOUNDARY_WALL,
        .color = white,
        .xPos = 0,
        .yPos = VGA_HEIGHT-1,
        .width = VGA_WIDTH,
        .height = 1
    };
    walls[2] = (Item){
        .type = ITEM_WINNING_WALL,
        .color = green,
        .xPos = VGA_WIDTH-1,
        .yPos = 0,
        .width = 1,
        .height = VGA_HEIGHT
    };
    walls[3] = (Item){
        .type = ITEM_LOSING_WALL,
        .color = red,
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
        .state = GAMESTATE_INIT,
        .speed = 1
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

    r = (r >> 3) & 0x1F;  // Convert 8-bit red to 5-bit
    g = (g >> 2) & 0x3F;  // Convert 8-bit green to 6-bit
    b = (b >> 3) & 0x1F;  // Convert 8-bit blue to 5-bit
    return (r << 11) | (g << 5) | b;
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
        return POINT_UPPER_LEFT;
    } else if(j==1 && i==0){
        return POINT_LOWER_LEFT;
    } else if(j==0 && i==1){
        return POINT_UPPER_RIGHT;
    } else if(j==1 && i==1){
        return POINT_LOWER_RIGHT;
    }
    return POINT_NONE;
}

void updateHitPoints(Game* game, unsigned int** frontBuffer, HitPoint* hitPoints){
    //Iterate through all corner pixels of the ball, checking for overlaps with blocks
    unsigned int currentHitCount = 0;
    const unsigned int sensePixelDistance = (unsigned int)(BALL_WIDTH);
    unsigned int senseX = 0;
    unsigned int senseY = 0;
    unsigned int currOuterX = game->ball.xPos;
    unsigned int currOuterY = game->ball.yPos;

    for(int i = 0; i<MAX_HITPOINTS;i++){
        hitPoints[i] = (HitPoint){
            .point = POINT_NONE,
            .item = NULL
        };
    }

    for(int i = 0; i< MAX_HITPOINTS/2;i++){
        for(int j = 0; j< MAX_HITPOINTS/2;j++){
            senseX = currOuterX + (i*(sensePixelDistance));
            senseY = currOuterY + (j*(sensePixelDistance));

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

unsigned int* getHitCorners(HitPoint* hitPoints){
    unsigned int* corners = (unsigned int*)calloc(4,sizeof(unsigned int));
    for(int i = 0;i<MAX_HITPOINTS;i++){
        if(hitPoints[i].item != NULL){
            switch(hitPoints[i].point){
                case POINT_UPPER_LEFT:
                    corners[0]++;
                    break;
                case POINT_UPPER_RIGHT:
                    corners[1]++;
                    break;
                case POINT_LOWER_LEFT:
                    corners[2]++;
                    break;
                case POINT_LOWER_RIGHT:
                    corners[3]++;
                    break;
            }
        }
    }
    return corners;
}

Corner findHitCorner(HitPoint* hitPoints){
    Corner corner = CORNER_NONE;
    unsigned int* corners= getHitCorners(hitPoints);
    if(corners[0] && corners[1] && corners[2]){
        corner = CORNER_UPPER_LEFT;
    } else if(corners[0] && corners[1] && corners[3]){
        corner = CORNER_UPPER_RIGHT;
    } else if(corners[0] && corners[2] && corners[3]){
        corner = CORNER_LOWER_LEFT;
    } else if(corners[1] && corners[2] && corners[3]){
        corner = CORNER_LOWER_RIGHT;
    }
    free(corners);
    return corner;
}

Side findHitSide(HitPoint* hitPoints){
    Side side = SIDE_NONE;
    unsigned int* corners = getHitCorners(hitPoints);
    if(corners[0] && corners[1]){
        side = SIDE_TOP;
    } else if(corners[2] && corners[3]){
        side = SIDE_BOTTOM;
    } else if(corners[0] && corners[2]){
        side = SIDE_LEFT;
    } else if(corners[1] && corners[3]){
        side = SIDE_RIGHT;
    }
    free(corners);
    return side;
}

void updateBallDirection(Game* game, HitPoint* hitPoints){
    //Find the number of centers hit
    volatile unsigned int currentHitCount = 0;
    for(int i = 0; i < MAX_HITPOINTS; i++){
        if(hitPoints[i].item != NULL){
            currentHitCount++;
        }
    }
    Vector currentDirection = game->ball.direction;
    if(currentHitCount > 0){
        Side hitSide = findHitSide(hitPoints);
        Corner hitCorner = findHitCorner(hitPoints);
        switch(hitPoints[0].item->type){
            case ITEM_BLOCK:
                switch (currentHitCount)
                    {
                    case 0:
                        game->ball.direction = (Vector){currentDirection.x, currentDirection.y};
                        break;
                    case 1:
                        game->ball.direction = (Vector){-currentDirection.x, -currentDirection.y};
                        break;
                    case 2:
                        switch (hitSide){
                        case SIDE_TOP:
                            game->ball.direction = (Vector){currentDirection.x, -currentDirection.y};
                            break;
                        case SIDE_BOTTOM:
                            game->ball.direction = (Vector){currentDirection.x, -currentDirection.y};
                            break;
                        case SIDE_LEFT:
                            game->ball.direction = (Vector){-currentDirection.x, currentDirection.y};
                            break;
                        case SIDE_RIGHT:
                            game->ball.direction = (Vector){-currentDirection.x, currentDirection.y};
                            break;
                        }
                        break;
                    case 3:
                        switch (hitCorner)
                        {
                        case CORNER_UPPER_LEFT:
                            game->ball.direction = (Vector){1,1};
                            break;
                        case CORNER_UPPER_RIGHT:
                            game->ball.direction = (Vector){-1,1};
                            break;
                        case CORNER_LOWER_LEFT:
                            game->ball.direction = (Vector){1,-1};
                            break;
                        case CORNER_LOWER_RIGHT:
                            game->ball.direction = (Vector){-1,-1};
                            break;
                        }
                        break;
                    case 4:
                        game->ball.direction = (Vector){-currentDirection.x, currentDirection.y};
                        break;
                    }
                    break;
            case ITEM_MIDBAR:
                game->ball.direction = (Vector){1,0};
                break;
            case ITEM_UPPERBAR:
                game->ball.direction = (Vector){1, -1};
                break;
            case ITEM_LOWERBAR:
                game->ball.direction = (Vector){1, 1};
                break;
            case ITEM_BOUNDARY_WALL:
                game->ball.direction = (Vector){currentDirection.x, -currentDirection.y};
                break;
            case ITEM_WINNING_WALL:
                game->ball.direction = (Vector){0,0};
                game->state = GAMESTATE_WON;
                game->speed = 5;
                loadTimer(game->speed);
                break;
            case ITEM_LOSING_WALL:
                game->ball.direction = (Vector){0,0};
                game->state = GAMESTATE_LOST;
                game->speed = 5;
                loadTimer(game->speed);
                break;
            default:
                break;
        }
    }
}

void handleInput(Game* game, unsigned int** buffer){
    char input = uartReadChar();
    switch(input){
        case 'w':
            for(int i = 0; i < 3; i++){
                if(game->bars->yPos>0 && game->bars[i].direction.y < 5 && game->bars[i].direction.y > -5){
                    game->bars[i].direction.y -= 5;
                } else {
                    game->bars[i].direction.y = 0;
                }   
            }
            for(int i = 0; i < 3; i++){
                moveItem(&(game->bars[i]), buffer);
            }
            break;
        case 's':
            for(int i = 0; i < 3; i++){
                if(game->bars->yPos+(game->bars->height*NUM_BARS) < VGA_HEIGHT && game->bars[i].direction.y < 5 && game->bars[i].direction.y > -5){
                    game->bars[i].direction.y += 5;
                } else {
                    game->bars[i].direction.y = 0;
                }
            }
            for(int i = 2; i >= 0; i--){
                moveItem(&(game->bars[i]), buffer);
            }
            break;
        case 'd':
            if(game->speed < 10000){
                game->speed += 10;
                loadTimer(game->speed);
            }
            break;
        case 'a':
            if(game->speed > 10){
                game->speed -= 10;
                loadTimer(game->speed);
            }
            break;
        default:
            for(int i = 0; i < 3; i++){
               game->bars[i].direction.y = 0;
            }
            break;
    }
}

void updateBlocks(HitPoint* hitPoints, unsigned int** buffer){
    for(int i = 0; i < MAX_HITPOINTS; i++){
        if(hitPoints[i].item->type != NULL){
            if(hitPoints[i].item->type == ITEM_BLOCK){
                hitPoints[i].item->color = white;
                drawBlock(buffer, hitPoints[i].item->xPos, hitPoints[i].item->yPos, hitPoints[i].item->width, hitPoints[i].item->height, hitPoints[i].item->color);
            }
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

void vgaWriteChar(char c, unsigned int x, unsigned int y){
    unsigned char* charBuffer = (unsigned int*)CHAR_BUFFER_BASE;
    charBuffer = charBuffer + (x | y<<7);
    *charBuffer = c;
}

void emptyCharBuffer(){
    for(int i = 0; i < MAX_CHAR_X; i++){
        for(int j = 0; j < MAX_CHAR_Y; j++){
            vgaWriteChar(0, i, j);    
        }
    }
}

void drawText(unsigned int** buffer, char* text, unsigned int x, unsigned int y){
    unsigned int currentY = x;
    unsigned int currentX = y;
    for(int i = 0; text[i] != '\0'; i++){
        if(text[i] == '\n'){
            currentY++;
            currentX = x;
        } else {
            if(currentY < MAX_CHAR_Y && currentX<MAX_CHAR_X){
                vgaWriteChar(text[i], currentX, currentY);
                currentX++;
            }
        }
    }
}


void uartWriteChar(char c) {
    //unsigned int* uartControl = (unsigned int*)(JTAG_UART_BASE + JTAG_UART_CONTROL);
    //while (!(*uartControl & (0x7FFF<<JTAG_UART_WSPACE_START))){};    
    unsigned int* uartData = (unsigned int*)(JTAG_UART_BASE + JTAG_UART_DATA);
    *uartData = (unsigned int) c;
}

void uartWriteString(const char* str) {
    while (*str) {
        uartWriteChar(*str);
        str++;
    }
}
char uartReadChar() {
    unsigned int* uartData = (unsigned int*)(JTAG_UART_BASE + JTAG_UART_DATA);
    if(check_bit(*uartData, JTAG_UART_RVALID_BIT)) {
        return (char)(*uartData & 0xFF);
    } else {
        return '\0';
    }
}


int main(int argc, char *argv[])
{
    VGA* vga = initVGA();
    Game* game = initGame();
    char* initText = "Welcome to Breakout!\n\nIf the ball hits the green wall, you win.\nIf the ball hits the red wall, you lose.\n\n\n   s = bar down \n   w = bar up \n   d = speed up \n   a = speed down\n\n\nAre you ready?\n\n\n\n\nStarting in: ";  
    char* won = "You won!";
    char* lost = "You lost!";
    volatile HitPoint* hitPoints = (HitPoint*)calloc(MAX_HITPOINTS,sizeof(HitPoint));


    loadTimer(game->speed);
    char mainTimer = 5;
    setScreenColor(vga->frontBuffer, black);
    drawText(vga->frontBuffer, initText,CHAR_BOX_X,CHAR_BOX_Y);
    char* mainTimerText = (char*)malloc(sizeof(char)*4);
    mainTimerText[1] = '\0';
    

    while (game->state != GAMESTATE_EXIT){
        switch(game->state){
            case GAMESTATE_INIT:
                if(checkTimer()){
                    if (mainTimer == 0){
                        mainTimer = END_TIME;
                        emptyCharBuffer();
                        setScreenColor(vga->frontBuffer, white);
                        drawGame(game,vga->frontBuffer);
                        game->state = GAMESTATE_RUNNING;
                        game->speed = 60;
                        loadTimer(game->speed);
                        break;
                    }
                    mainTimer--;
                    mainTimerText[0] = (char)(mainTimer+48);
                    drawText(vga->frontBuffer, mainTimerText,25,22+2*mainTimer);
                }
                break;
            case GAMESTATE_RUNNING:
                if (checkTimer()) {
                    updateHitPoints(game,vga->frontBuffer,hitPoints);
                    updateBallDirection(game, hitPoints);
                    handleInput(game, vga->frontBuffer);

                    updateBlocks(hitPoints, vga->frontBuffer);

                    moveItem(&(game->ball), vga->frontBuffer);
                }
                break;
            case GAMESTATE_WON:
                if(checkTimer()){
                    if (mainTimer == END_TIME-1){
                        emptyCharBuffer();
                        setScreenColor(vga->frontBuffer, green);
                        drawText(vga->frontBuffer, won,CHAR_BOX_X,CHAR_BOX_Y);
                        uartWriteString(won);
                    } else if (mainTimer == 0){
                        setScreenColor(vga->frontBuffer, black);
                        game->state = GAMESTATE_EXIT;
                        break;
                    }
                    mainTimer--;
                }
                break;
            case GAMESTATE_LOST:
                if(checkTimer()){
                    if (mainTimer == END_TIME-1){
                        emptyCharBuffer();
                        setScreenColor(vga->frontBuffer, red);
                        drawText(vga->frontBuffer, lost,CHAR_BOX_X,CHAR_BOX_Y);
                        uartWriteString(lost);
                    } else if (mainTimer == 0){
                        emptyCharBuffer();
                        setScreenColor(vga->frontBuffer, black);
                        game->state = GAMESTATE_EXIT;
                        break;
                    }
                    mainTimer--;
                }
                break;
            case GAMESTATE_EXIT:
                break;
        }
    }
    free(mainTimerText);
    free(hitPoints);
    for(int i = 0; i < n_rows; i++){
        free(game->blocks[i]);
    }
    free(game->blocks);
    free(game->bars);
    free(game->walls);
    free(game);
    free(vga);
    return 0;
}
