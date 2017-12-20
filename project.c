#include <stdio.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include "juv.h"  
#include "CameralOpt.h"  
#include "FrameBufferOpt.h"  

#if 1
#define    WIDTH   640  
#define    HIGHT   480  
#else
#define    WIDTH   1280  
#define    HIGHT   720  
#endif

  
int main(void)  
{
    char yuyv[WIDTH*HIGHT*2];  //-存储一张图片的数据
    char bmp[54 + WIDTH*HIGHT*3];  //-24位图需要三个字节表示一个像素点
    char	inputc;
  
    set_bmp_header((struct bmp_header_t *)bmp, WIDTH, HIGHT);  
    //初始化摄像头  
    Init_Cameral(WIDTH , HIGHT );  
    //初始化framebuffer  
    Init_FrameBuffer(WIDTH , HIGHT );   
  
    //开启摄像头  
    Start_Cameral();  
    //采集一张图片  
    int count = 0 ;   
    //-read_bmp_header("5.bmp", (struct bmp_header_t *)bmp);
    //-show_bmp("5.bmp", "cap00.bmp");	//-读一个图片,显示到LCD,以文件形式存储
    while(1)  
    {
#if 1
	
        Get_Picture(yuyv);  //-从摄像头获取图片数据
        //-yuyv2rgb24(yuyv, bmp+54, WIDTH, HIGHT);  //-把图像数据转化为可用个格式
        yuyv2rgb16(yuyv, bmp+54, WIDTH, HIGHT);
        Write_FrameBuffer(bmp+54);  //-转化后的数据输出到屏幕上进行显示
        //-save_bmp(bmp, "cap01.bmp");
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

#if 0
	inputc = getchar();
 	if (inputc == 'Q' || inputc == 'q') break;
#endif
      printf("count:%d \n" , count++);  
    }
    //关闭摄像头  
    Stop_Cameral();  
    //关闭Framebuffer  
    Exit_Framebuffer();  
    //退出  
    Exit_Cameral();  
      
    return 0;  
}  

