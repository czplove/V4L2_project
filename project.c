#include <stdio.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include "juv.h"  
#include "CameralOpt.h"  
#include "FrameBufferOpt.h"  
  
#define    WIDTH   640  
#define    HIGHT   480  
  
int main(void)  
{  
    char yuyv[WIDTH*HIGHT*2];  
    char bmp[WIDTH*HIGHT*3];  
  
//  set_bmp_header((struct bmp_header_t *)bmp, WIDTH, HIGHT);  
    //初始化摄像头  
    Init_Cameral(WIDTH , HIGHT );  
    //初始化framebuffer  
    Init_FrameBuffer(WIDTH , HIGHT );   
  
    //开启摄像头  
    Start_Cameral();  
    //采集一张图片  
    int count = 0 ;   
    while(1)  
    {  
        Get_Picture(yuyv);  
        yuyv2rgb24(yuyv, bmp, WIDTH, HIGHT);  
        Write_FrameBuffer(bmp);  
//      printf("count:%d \n" , count++);  
    }  
    //关闭摄像头  
    Stop_Cameral();  
    //关闭Framebuffer  
    Exit_Framebuffer();  
    //退出  
    Exit_Cameral();  
      
    return 0;  
}  
