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
static struct fb_fix_screeninfo finfo;
static struct fb_var_screeninfo vinfo;
  
//初始化framebuffer  
int Init_FrameBuffer(int Width , int Higth)  
{
	
    W = Width ;   
    H = Higth ;   
    Frame_fd = open("/dev/fb0" , O_RDWR);  
    if(-1 == Frame_fd)  
    {  
        perror("open frame buffer fail");  
        return -1 ;   
    }  
    
    if(ioctl(Frame_fd, FBIOGET_FSCREENINFO, &finfo))	//-获取固定信息,各个硬件不同,所以需要动态获取
	{
	   printf("Error:reading fixed info.\n");
	   return -1;
	}
  	printf("fb_size = %d\r\n",finfo.smem_len);						//-1638400
  	
  	if(ioctl(Frame_fd, FBIOGET_VSCREENINFO, &vinfo))	//-返回的是与Framebuffer有关的可变信息。
	{
	   printf("Error reading var info.\n");
	   return -1;
	}
	printf("fb_xres = %d\r\n",vinfo.xres);						//-800
	printf("fb_yres = %d\r\n",vinfo.yres);						//-480
  	
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
int Write_FrameBuffer(const char *buffer)	//-传入的数据是纯RGB数据
{
    int row  , col ;   
    char *p = NULL ;   
    for(row = 0 ; row < vinfo.yres ; row++)  //-行 y
    {
        for(col = 0 ; col < vinfo.xres ;  col++)  //-列 x
        {
            if((row < H)  && (col < W))  
            {
                p = (char *)(buffer + (row * W+ col ) * 3);   
                FrameBuffer[row*vinfo.xres+col] = RGB((unsigned char)(*(p+2)),(unsigned char)(*(p+1)),(unsigned char )(*p));  
            }
        }  
    }  
    return 0 ;   
}
  
//退出framebuffer  
int Exit_Framebuffer(void)  
{  
    munmap(FrameBuffer ,  finfo.smem_len);  
    close(Frame_fd);  
    return 0 ;   
}  

///////////////////////////////////////////////////////////////////////////////

//-针对目前M3352平台 液晶屏的测试参数:xres 800, yres 480, width -1, height -1, 16bpp
void fb_draw_point(void *memp, 
	  			   unsigned int xres, unsigned int yres, 
				   unsigned int x, unsigned int y, 
				   unsigned int color)
{
   *((unsigned short int *)memp+xres*y+x)=color;	//-整个屏幕映射成了一块内存,实际操作驱动去完成
}

//-直接在屏幕上画图,来测试屏幕实际反馈参数
int FrameBuffer_draw(void)
{
	int k=0, i=0, j=0;
	int col[] = {0xffffffff,0x00000000,~0x1f,0x0000f800,0x7e0,0x1f};
	char	inputc;
	
	for(k = 0; k<1600; k++)
	{
	   printf("%d:col[%d] = 0x%x",k,k%5,col[k%5]);
	   /*
			0:col[0] = 0xffffffff
			1:col[1] = 0x0
			2:col[2] = 0xffffffe0
			3:col[3] = 0xf800
			4:col[4] = 0x7e0
	   */

	   //-color = 1<<k;
	   
	   for(j=0; j<vinfo.yres - 1; j++){
		   for(i=0; i<vinfo.xres - 1 ; i++) {
				fb_draw_point(FrameBuffer,vinfo.xres,vinfo.yres,i,j,col[0]);
		   }
	   }


		for(i = vinfo.xres / 2 - 50; i < vinfo.xres / 2 + 50; i++) {
	        for(j=vinfo.yres / 2 - 50; j<vinfo.yres / 2 + 50; j++) {
	            fb_draw_point(FrameBuffer,vinfo.xres,vinfo.yres,i,j,col[k%5]);
			}
		}
 		   
	    inputc = getchar();
 	    if (inputc == 'Q' || inputc == 'q') break;
	}
}

