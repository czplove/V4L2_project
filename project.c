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
    char yuyv[WIDTH*HIGHT*2];  //-存储一张图片的数据
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
        Get_Picture(yuyv);  //-从摄像头获取图片数据
        yuyv2rgb24(yuyv, bmp, WIDTH, HIGHT);  //-把图像数据转化为可用个格式
        Write_FrameBuffer(bmp);  //-转化后的数据输出到屏幕上进行显示
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
