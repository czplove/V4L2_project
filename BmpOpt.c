//#include"stdio.h"
//#include"iostream.h"
//#include"string.h"
//#include <conio.h>
//#include"stdlib.h"
////-#include"windows.h"
//#include"linux_bmp.h"
//
//unsigned char *pBmpBuf;//读入图像数据的指针
//int bmpWidth;//图像的宽
//int bmpHeight;//图像的高
//RGBQUAD *pColorTable;//颜色表指针
//int biBitCount;//图像类型，每像素位数
//
///*****************************************************/
///*函数名称readBmp()*/
//
//bool readBmp(char *bmpName)
//{
//	//二进制读方式打开指定的图像文件
//	FILE *fp=fopen(bmpName,"rb");
//	if(fp==0)  return 0;
//	
//	//跳过位图文件头结构BITMAPFILEHEADER
//	fseek(fp,sizeof(BITMAPFILEHEADER),0);
//	
//	//定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中
//	BITMAPINFOHEADER head;
//	fread(&head, sizeof(BITMAPINFOHEADER), 1,fp);
//	
//	//获取图像宽、高、每像素所占位数等信息
//	bmpWidth=head.biWidth;
//	bmpHeight=head.biHeight;
//	biBitCount=head.biBitCount;
//	
//	//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）
//	//int lineByte=(bmpWidth * biBitCount/8+3)/4*4;
//	int lineByte=(bmpWidth*biBitCount+31)/32*4;
//	
//	//灰度图像有颜色表，且颜色表表项为256
//	if(biBitCount==8)
//	{
//	//申请颜色表所需要的空间，读颜色表进内存
//	pColorTable=new RGBQUAD[256];
//	fread(pColorTable,sizeof(RGBQUAD),256,fp);
//	}
//	
//	//申请位图数据所需要的空间，读位图数据进内存
//	pBmpBuf=new unsigned char[lineByte * bmpHeight];
//	fread(pBmpBuf,1,lineByte * bmpHeight,fp);
//	
//	
//	int BytesPerLine;
//	BYTE data[999999];
//	BYTE Color[1300][3];
//	BytesPerLine=(bmpWidth*biBitCount+31)/32*4;
//	    fseek(fp,54,0);
//	for(int i=bmpHeight-1;i>=0;i--)
//	{ 
//	fread(data,1,BytesPerLine,fp); 
//	
//	for(int k=0;k <bmpWidth*3;k++)
//	{
//	if(k%3==2)
//	{
//	Color[k/3][2]=data[k-2];//b
//	Color[k/3][1]=data[k-1];//g
//	Color[k/3][0]=data[k-0];//r
//	}
//	}
//	
//	int m=1,n;
//	cout<<"第"<<i<<"行的颜色red分量矩阵为:"<<endl;
//	for(k=0;k<bmpWidth;k++)
//	{
//	n=int(Color[k][0]);
//	if(n/100==0&&n/10==0)
//	cout<<"  "<<n<<"    ";
//	else if(n/100==0)
//	cout<<" "<<n<<"    ";
//	else
//	cout<<n<<"    ";
//	if(k==10*m-1)
//	{
//	cout<<endl;
//	m++;
//	}
//	}
//	cout<<endl<<endl<<endl;
//	
//	m=1;
//	cout<<"第"<<i<<"行的颜色green分量矩阵为:"<<endl;
//	for(k=0;k<bmpWidth;k++)
//	{
//	n=int(Color[k][1]);
//	if(n/100==0&&n/10==0)
//	cout<<"  "<<n<<"    ";
//	else if(n/100==0)
//	cout<<" "<<n<<"    ";
//	else
//	cout<<n<<"    ";
//	if(k==10*m-1)
//	{
//	cout<<endl;
//	m++;
//	}
//	}
//	cout<<endl<<endl<<endl;
//	
//	m=1;
//	cout<<"第"<<i<<"行的颜色blue分量矩阵为:"<<endl;
//	for(k=0;k<bmpWidth;k++)
//	{
//	n=int(Color[k][2]);
//	if(n/100==0&&n/10==0)
//	cout<<"  "<<n<<"    ";
//	else if(n/100==0)
//	cout<<" "<<n<<"    ";
//	else
//	cout<<n<<"    ";
//	if(k==10*m-1)
//	{
//	cout<<endl;
//	m++;
//	}
//	}
//	cout<<endl<<endl<<endl;
//	}
//	
//	//关闭文件
//	fclose(fp);
//	return 1;
//}
//
//
///*函数名称：saveBmp()函数参数：char *bmpName文件名字及路径;
//unsigned char *imgBuf待存盘的位图数据;
//int width以像素为单位待存盘位图的宽;
//int  height以像素为单位待存盘位图高;
//int biBitCount每像素所占位数;
//RGBQUAD *pColorTable颜色表指针
//返回值：0为失败，1为成功.
//说明：给定一个图像位图数据、宽、高、颜色表指针及每像素所占的位数等信息，
//将其写到指定文件中*/
//
//
//bool saveBmp(char *bmpName,unsigned char *imgBuf,int width,int height,
// int biBitCount, RGBQUAD *pColorTable)
//{
//	//如果位图数据指针为0，则没有数据传入，函数返回
//	if(!imgBuf) return 0; 
//	
//	//颜色表大小，以字节为单位，灰度图像颜色表为1024字节，彩色图像颜色表大小为0
//	int colorTablesize=0;
//	if(biBitCount==8)
//	colorTablesize=1024;
//	
//	//待存储图像数据每行字节数为4的倍数
//	//int lineByte=(width * biBitCount/8+3)/4*4;
//	int lineByte=(bmpWidth*biBitCount+31)/32*4;
//	
//	//以二进制写的方式打开文件
//	FILE *fp=fopen(bmpName,"wb");
//	if(fp==0) return 0;
//	
//	//申请位图文件头结构变量，填写文件头信息
//	BITMAPFILEHEADER fileHead;
//	fileHead.bfType = 0x4D42;
//	
//	//bmp类型 
//	//bfSize是图像文件4个组成部分之和
//	fileHead.bfSize=sizeof(BITMAPFILEHEADER)+
//	sizeof(BITMAPINFOHEADER)+colorTablesize+
//	lineByte*height;
//	fileHead.bfReserved1=0;
//	fileHead.bfReserved2=0;
//	
//	//bfOffBits是图像文件前3个部分所需空间之和
//	fileHead.bfOffBits=54+colorTablesize;
//	
//	//写文件头进文件
//	fwrite(&fileHead,sizeof(BITMAPFILEHEADER),1,fp);
//	
//	//申请位图信息头结构变量，填写信息头信息
//	BITMAPINFOHEADER head;
//	head.biBitCount=biBitCount;
//	head.biClrImportant=0;
//	head.biClrUsed=0;head.biCompression=0;
//	head.biHeight=height;
//	head.biPlanes=1;
//	head.biSize=40;
//	head.biSizeImage=lineByte*height;
//	head.biWidth=width;
//	head.biXPelsPerMeter=0;
//	head.biYPelsPerMeter=0;
//	
//	//写位图信息头进内存
//	fwrite(&head,sizeof(BITMAPINFOHEADER),1,fp);
//	
//	//如果灰度图像，有颜色表，写入文件
//	if(biBitCount==8)
//	fwrite(pColorTable,sizeof(RGBQUAD),256,fp);
//	
//	//写位图数据进文件
//	fwrite(imgBuf,height*lineByte,1,fp);
//	
//	//关闭文件fclose(fp);
//	return 1;
//}
//
//
//void main()
//{
//	//读入指定BMP文件进内存
//	char readPath[]="D:\\4.bmp";
//	readBmp(readPath);
//	
//	//输出图像的信息
//	printf("width=%d,height=%d,biBitCount=%d\n",bmpWidth,bmpHeight,biBitCount);
//	
//	cout<<endl<<endl<<endl;
//	
//	//将图像数据存盘
//	char writePath[]="D:\\2.bmp";
//	saveBmp(writePath,pBmpBuf,bmpWidth,bmpHeight,biBitCount,pColorTable);
//	
//	//清除缓冲区，pBmpBuf和pColorTable是全局变量，在文件读入时申请的空间
//	delete []pBmpBuf;
//	if(biBitCount==8)
//	delete []pColorTable;
//}
//

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
  
#if 1  
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
#endif  

///////////////////////////////////////////////////////////////////////////////
//-1. 在BMP文件中，如果一个数据需要用几个字节来表示的话，那么该数据的存放字节顺序为“低地址村存放低位数据，高地址存放高位数据”。
#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <fcntl.h>  
#include <string.h>  
#include <linux/fb.h>  
#include <sys/mman.h>  
#include <sys/ioctl.h>  
#include <arpa/inet.h>  
#include <errno.h>  
   
//14byte文件头  
typedef struct  
{  
    char    cfType[2];//文件类型，"BM"(0x4D42)  
    int     cfSize;//文件大小（字节）  
    int     cfReserved;//保留，值为0  
    int     cfoffBits;//数据区相对于文件头的偏移量（字节）  
}__attribute__((packed)) BITMAPFILEHEADER;  
//__attribute__((packed))的作用是告诉编译器取消结构在编译过程中的优化对齐  
   
//40byte信息头  
typedef struct  
{  
    char ciSize[4];//BITMAPFILEHEADER所占的字节数  
    int  ciWidth;//宽度  
    int  ciHeight;//高度  
    char ciPlanes[2];//目标设备的位平面数，值为1  
    short ciBitCount;//每个像素的位数  
    char ciCompress[4];//压缩说明  
    char ciSizeImage[4];//用字节表示的图像大小，该数据必须是4的倍数  
    char ciXPelsPerMeter[4];//目标设备的水平像素数/米  
    char ciYPelsPerMeter[4];//目标设备的垂直像素数/米  
    char ciClrUsed[4]; //位图使用调色板的颜色数  
    char ciClrImportant[4]; //指定重要的颜色数，当该域的值等于颜色数时（或者等于0时），表示所有颜色都一样重要  
}__attribute__((packed)) BITMAPINFOHEADER;  
   
typedef struct  
{  
    unsigned char blue;  
    unsigned char green;  
    unsigned char red;  
    unsigned char reserved;  
}__attribute__((packed)) PIXEL;//颜色模式RGB  
   
BITMAPFILEHEADER FileHead;  
BITMAPINFOHEADER InfoHead;  
   
static char *fbp = 0;  
static int xres = 0;  
static int yres = 0;  
static int bits_per_pixel = 0;  
int width,height;  
   
int show_bmp();  
int fbfd = 0;  
   
   
static int cursor_bitmap_format_convert(char *dst,char *src)  
{  
    int i ,j ;  
    char *psrc = src ;  
    char *pdst = dst;  
    char *p = psrc;  
   
    /* 由于bmp存储是从后面往前面，所以需要倒序进行转换 */  
    pdst += (width * height * 3);  
    for(i=0;i<height;i++){  
        p = psrc + (i+1) * width * 3;  
        for(j=0;j<width;j++){  
            pdst -= 3;  
            p -= 3;  
            pdst[0] = p[0];  
            pdst[1] = p[1];  
            pdst[2] = p[2];  
        }  
    }  
    return 0;  
}  
   
int show_bmp(char *path,char *fb_path)  
{
    int i;  
    FILE *fp,*fb_file;  
    int rc;  
    int line_x, line_y;  
    long int location = 0, BytesPerLine = 0;  
    char *bmp_data_buf = NULL;  
    char *bmp_buf_dst = NULL;  
    char * buf = NULL;  
    int flen = 0;  
    int ret = -1;  
    int bmp_data_length = 0;  
   
    printf("into show_bmp function\n");  
    if(path == NULL || fb_path == NULL)  
    {  
        printf("path Error,return\n");  
        return -1;  
    }  
    printf("path = %s\n", path);  
    fp = fopen( path, "rb" );  
    if(fp == NULL){  
        printf("load cursor file open failed\n");  
        return -1;  
    }  
   
    printf("fb_path = %s\n", fb_path);  
    fb_file = fopen( fb_path, "wb" );  
    if(fb_file == NULL){  
        printf("load cursor file open failed\n");  
        return -1;  
    }  
   
    /* 求解文件长度 */  
    fseek(fp,0,SEEK_SET);  
    fseek(fp,0,SEEK_END);  
   
    flen = ftell(fp);  
    printf("the length of file is %d\n",flen);  //-整个文件的实际大小
   
    bmp_data_buf = (char*)calloc(1,flen - 54);//位图纯数据部分  
    if(bmp_data_buf == NULL){  
        printf("load > malloc bmp out ofmemory!\n");  
        return -1;  
    }  
   
   
    /* 再移位到文件头部 */  
    fseek(fp,0,SEEK_SET);  
   
    rc = fread(&FileHead, sizeof(BITMAPFILEHEADER),1, fp);  
    if ( rc != 1)  
    {  
        printf("read header error!\n");  
        fclose( fp );  
        return( -2 );  
    }  
   
    //检测是否是bmp图像  
    if (memcmp(FileHead.cfType, "BM", 2) != 0)  //-文件头
    {  
        printf("it's not a BMP file\n");  
        fclose( fp );  
        return( -3 );  
    }
    rc = fread( (char *)&InfoHead, sizeof(BITMAPINFOHEADER),1, fp );  //-位图头
    if ( rc != 1)  
    {  
        printf("read infoheader error!\n");  
        fclose( fp );  
        return( -4 );  
    }  
    width = InfoHead.ciWidth;  
    height = InfoHead.ciHeight;  
   
    printf("FileHead.cfSize =%d byte\n",FileHead.cfSize);  
    printf("flen = %d\n", flen);  
    printf("width = %d, height = %d\n", width, height);  
   
    bmp_data_length = width * height * 2;  
   
    printf("bmp_data_length = %d\n", bmp_data_length);  
   
    //跳转的数据区  
    fseek(fp, FileHead.cfoffBits, SEEK_SET);  
    printf(" FileHead.cfoffBits = %d\n", FileHead.cfoffBits);  
    printf(" InfoHead.ciBitCount = %d\n", InfoHead.ciBitCount);  
   
    //每行字节数  
    buf = bmp_data_buf;  
    while ((ret = fread(buf,1,bmp_data_length,fp)) >= 0) { 
        if (ret == 0) {
            usleep(100);  
            continue;  
        }  
        printf("ret = %d\n", ret);  
        buf = ((char*) buf) + ret;  
        bmp_data_length = bmp_data_length -ret;  
        if(bmp_data_length == 0)  
            break;  
    }  
   
    ///重新计算，很重要！！  
    //-bmp_data_length = width * height *3;  
    //-bmp_buf_dst = (char*)calloc(1,bmp_data_length );  
    //-if(bmp_buf_dst == NULL){  
    //-    printf("load > malloc bmp out ofmemory!\n");  
    //-    return -1;  
    //-}  
   
   memcpy(FrameBuffer , bmp_data_buf , bmp_data_length); 
    //-cursor_bitmap_format_convert(bmp_buf_dst,bmp_data_buf);  
    fwrite(bmp_buf_dst,1,bmp_data_length,fb_file);  
    free(bmp_data_buf);  
    free(bmp_buf_dst);  
   
    fclose(fp);  
    fclose(fb_file);  
   
    printf("show logo return 0\n");  
    return 0;  
}  
   
#if 0
int main()  
{  
   
    show_bmp( "/home/ubuntu/data.bmp","/home/ubuntu/data.fb");
   
    printf("the size of char is %d byte, the size of intis %d byte\n",sizeof(char),sizeof(int));  
}  
#endif