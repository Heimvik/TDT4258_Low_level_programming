#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <poll.h>
#include <fcntl.h> 
#include <errno.h>
#include <stdint.h>
#include <math.h>

#define X_PX 300
#define Y_PX 300

typedef enum DEVICETYPE{
    DEFAULT,INPUT
} DEVICETYPE;

int openDevice(char* deviceName,DEVICETYPE deviceType){
    char filePath[240];
    memset(filePath,0,240);
    char base[50];
    memset(filePath,0,50);
    switch(deviceType){
        case(DEFAULT):
            strcpy(base, "/dev/%s");
            break;
        case(INPUT):
            strcpy(base, "/dev/input/%s");
            break;
        default:
    }
    sprintf(filePath,base,deviceName);
    int fileDescriptor = open(filePath,O_RDWR);
    
    if(fileDescriptor != -1){
        printf("Successfully opened file %s\n",filePath);
    } else {
        printf("Error opening file %s\n",filePath);
    }
}

/*
How to use the fb.
*/
void printFbProperties(int fbFd){
    struct fb_fix_screeninfo fb0FixInfo;
    struct fb_var_screeninfo fb0VarInfo;

    ioctl(fbFd,FBIOGET_FSCREENINFO,&fb0FixInfo);
    ioctl(fbFd,FBIOGET_VSCREENINFO,&fb0VarInfo);

    printf("Fixed info:\n");
    printf("ID: %s\n",fb0FixInfo.id);
    printf("Start of frame buffer mem: %lu\n",fb0FixInfo.smem_start);
    printf("Length of frame buffer mem: %u\n",fb0FixInfo.smem_len);
    printf("Type: %u\n",fb0FixInfo.type);
}

uint16_t* initFrameBuffer(){
    int fbFd = openDevice("fb0",DEFAULT);
    printFbProperties(fbFd);
    //now to only mmap the framebuffer
    const size_t fameBufferSize = (sizeof(uint16_t)*2)*X_PX*Y_PX;
    return mmap(NULL,fameBufferSize,PROT_READ|PROT_WRITE,MAP_SHARED,fbFd,0);
}

/*
How to use poll() with the events.
*/
char readKeyboard(int fd){
    struct pollfd pollKeyboard = {
        .fd = fd,
        .events = POLLIN
    };
    struct input_event inputEvent;
    poll(&pollKeyboard,1,0);
    if(pollKeyboard.revents & POLLIN) { //Timeout 0 makes it non-blocking
        read(fd,&inputEvent,sizeof(struct input_event));
        printf("Event: type=%d, code=%d, value=%d\n", inputEvent.type, inputEvent.code, inputEvent.value);
    }
    return 0;
}

void main(){
    //initFrameBuffer();
    int stdinFd = openDevice("stdin",DEFAULT);
    while(1){
        readKeyboard(stdinFd);
    }
}