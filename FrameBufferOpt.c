#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include<sys/ioctl.h>
#include "FrameBufferOpt.h"  
  
static int Frame_fd ;   
static int *FrameBuffer = NULL ;   
static int W , H ;  
  
//初始化framebuffer  
int Init_FrameBuffer(int Width , int Higth)  
{  
	struct fb_fix_screeninfo finfo;
	
    W = Width ;   
    H = Higth ;   
    Frame_fd = open("/dev/fb0" , O_RDWR);  
    if(-1 == Frame_fd)  
    {  
        perror("open frame buffer fail");  
        return -1 ;   
    }  
    
    if(ioctl(Frame_fd, FBIOGET_FSCREENINFO, &finfo))
	{
	   printf("Error:reading fixed info.\n");
	   return -1;
	}
  
//根本就不用CPU搬运   用DMA做为搬运工  
FrameBuffer = mmap(0, finfo.smem_len , PROT_READ | PROT_WRITE , MAP_SHARED , Frame_fd ,0 );  
    if(FrameBuffer == (void *)-1)  
    {  
        perror("memory map fail");  
        return -2 ;  
    }  
    return 0 ;   
}  
  
//写入framebuffer  
int Write_FrameBuffer(const char *buffer)  
{  
    int row  , col ;   
    char *p = NULL ;   
    for(row = 0 ; row <1024 ; row++)  
    {  
        for(col = 0 ; col < 1280 ;  col++)  
        {  
            if((row < H)  && (col < W))  
            {  
                p = (char *)(buffer + (row * W+ col ) * 3);   
                FrameBuffer[row*1280+col] = RGB((unsigned char)(*(p+2)),(unsigned char)(*(p+1)),(unsigned char )(*p));  
            }  
        }  
    }  
    return 0 ;   
}  
  
//退出framebuffer  
int Exit_Framebuffer(void)  
{  
    munmap(FrameBuffer ,  W*H*4);  
    close(Frame_fd);  
    return 0 ;   
}  
