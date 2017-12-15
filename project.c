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
    char bmp[WIDTH*HIGHT*3];  //-24位图需要三个字节表示一个像素点
  
  	//set_bmp_header((struct bmp_header_t *)bmp, WIDTH, HIGHT);  
    //初始化摄像头  
    Init_Cameral(WIDTH , HIGHT );  
    //初始化framebuffer  
    Init_FrameBuffer(WIDTH , HIGHT );   
  
    //开启摄像头  
    Start_Cameral();  
    //采集一张图片  
    int count = 0 ;   
    read_bmp_header("5.bmp", (struct bmp_header_t *)bmp);
    while(1)  
    {
#if 0
        Get_Picture(yuyv);  //-从摄像头获取图片数据
        yuyv2rgb24(yuyv, bmp, WIDTH, HIGHT);  //-把图像数据转化为可用个格式
        Write_FrameBuffer(bmp);  //-转化后的数据输出到屏幕上进行显示
#endif
        
#if 0
	//-屏幕显示固定图片
	//-像素点和内存映射之间还有RGB的关系
	FrameBuffer_draw();
#endif

#if 0
	show_bmp( "/media/mmcblk0p1/hello16.bmp","/media/mmcblk0p1/data.fb");  
#endif

#if 0
	//-视屏获取的图片存储为文件形式
	
#endif

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

