﻿#ifndef  _FRAMEBUFFEROPT_H  
#define  _FRAMEBUFFEROPT_H  
  
#include <stdio.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <sys/mman.h>  
  
  
#define    RGB(r,g,b)       ((r<<16)|(g<<8)|b)  
  
//初始化ramebuffer  
int Init_FrameBuffer(int Width , int Higth);  
  
//写数据到framebuffer  
int Write_FrameBuffer(const char *buffer);  
  
//退出framebuffer  
int Exit_Framebuffer(void);  
  
struct fb_lcd
{
   int fb_fd;
   void *fb_mem;
   unsigned int fb_xres;
   unsigned int fb_yres;
   unsigned int fb_line_len;
   unsigned int fb_size;
   unsigned int fb_bpp;
};
  

  
#endif //_FRAMEBUFFEROPT_H  