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
    
#include "juv.h"    
   
extern int *FrameBuffer;
extern struct fb_fix_screeninfo finfo;
extern struct fb_var_screeninfo vinfo;

char *bmp_head_buf = NULL;  
   

//14byte文件头  
typedef struct  
{  
    char    cfType[2];//文件类型，"BM"(0x4D42)  
    int     cfSize;//文件大小（字节）  
    int     cfReserved;//保留，值为0  
    int     cfoffBits;//数据区相对于文件头的偏移量（字节）  
}__attribute__((packed)) BITMAPFILEHEADER;  
//__attribute__((packed))的作用是告诉编译器取消结构在编译过程中的优化对齐  
   


BITMAPFILEHEADER FileHead;
int width,height;
int flen;

//-0xF800、0x07E0、0x001F
static int cursor_bitmap_format_convert(char *dst,char *src)  
{
    int i ,j ;  
    char *psrc = src ;  
    char *pdst = dst;  
    char *p = psrc;  
   
    /* 由于bmp存储是从后面往前面，所以需要倒序进行转换 */  
    pdst += (width * height * 2);  //-指到数组最后
    for(i=0;i<height;i++){
        p = psrc + (i+1) * width * 2;  
        for(j=0;j<width;j++){
            pdst -= 2;  
            p -= 2;  
            pdst[0] = p[0];  
            pdst[1] = p[1];  
            //-pdst[2] = p[2];  
        }
    }  
    return 0;  
}




#if 1
int read_bmp_header(char *bmpName)
{
	int rc;
	struct bmp_header_t *header;
	
	printf("into read_bmp_header function\n");
    if(bmpName == NULL)
    {  
        printf("path Error,return\n");  
        return -1;  
    }
    printf("path = %s\n", bmpName);  
	//二进制读方式打开指定的图像文件
	FILE *fp=fopen(bmpName,"rb");
	if(fp==0)  return 0;
	
	/* 求解文件长度 */  
    fseek(fp,0,SEEK_SET);  
    fseek(fp,0,SEEK_END);  
   
    flen = ftell(fp);  
    printf("the length of file is %d\n",flen);  //-整个文件的实际大小
	
	/* 再移位到文件头部 */  
    fseek(fp,0,SEEK_SET);  
   
    rc = fread(&FileHead, sizeof(BITMAPFILEHEADER),1, fp);  //-文件头的大小是固定的
    if ( rc != 1)  
    {
        printf("read header error!\n");  
        fclose( fp );
        return( -2 );  
    }
	
	//检测是否是bmp图像  
    if (memcmp(FileHead.cfType, "BM", 2) != 0)  //-文件头,这个必须是地址的情况下比较否则Segmentation fault (core dumped)
//    if(header->magic != 0x4D42)
    {  
        printf("it's not a BMP file\n");  
        fclose( fp );  
        return( -3 );  
    }
	
	//-printf("FileHead file type = %d \n",header->magic);  
    printf("FileHead real the length of file = %d\n", flen);  //-文件的实际大小
    printf("FileHead offset = %d\n", FileHead.cfoffBits);
    
    /* 再移位到文件头部 */  
    fseek(fp,0,SEEK_SET);
    
  	bmp_head_buf = (char*)calloc(1,FileHead.cfoffBits);//位图数据之前的内容
    if(bmp_head_buf == NULL){
        printf("load > malloc bmp out ofmemory!\n");  
        return -1;  
    }
    
    rc = fread(bmp_head_buf, FileHead.cfoffBits,1, fp);  //-文件头的大小是固定的
    if ( rc != 1)
    {
        printf("read header error!\n");  
        free(bmp_head_buf);
        fclose( fp );
        return( -2 );  
    }
	
	header = bmp_head_buf;
	printf("InfoHead head_num = %d \n",header->head_num);  
    printf("InfoHead width = %d, height = %d\n", header->width, header->height);  
    printf("InfoHead color_planes = %d\n", header->color_planes);
	printf("InfoHead bit_count = %d\n", header->bit_count);
	printf("InfoHead bit_compression = %d\n", header->bit_compression);
	printf("InfoHead image_size = %d\n", header->image_size);
	printf("InfoHead color_num = %d\n", header->color_num);
	printf("InfoHead important_colors = %d\n", header->important_colors);
	
	printf("out read_bmp_header function\n");  
	//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）
	//int lineByte=(bmpWidth * biBitCount/8+3)/4*4;
	//-int lineByte=(bmpWidth*biBitCount+31)/32*4;
	fclose( fp );
}
#endif
   
#if 1
int show_bmp(char *bmpName, char *fb_path)  
{
    int i, j;  
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
    struct bmp_header_t *header;
   
   
	read_bmp_header(bmpName);
	header = bmp_head_buf;
   
    printf("into show_bmp function\n");  
    if(bmpName == NULL)  
    {
        printf("path Error,return\n");  
        return -1;  
    }  
    printf("path = %s\n", bmpName);  
    fp = fopen( bmpName, "rb" );  
    if(fp == NULL){
        printf("load cursor file open failed\n");  
        return -1;  
    }  
   
   
    //跳转到数据区  
    fseek(fp, header->offset, SEEK_SET);  
    printf(" FileHead.cfoffBits = %d\n", header->offset);  
    //-printf(" InfoHead.ciBitCount = %d\n", InfoHead.ciBitCount);  
   
   	bmp_data_buf = (char*)calloc(1,header->image_size);//位图纯数据部分和实际像素是对应的,只是有些软件会在最后多两个信息位不影响显示  
    if(bmp_data_buf == NULL){
        printf("load > malloc bmp out ofmemory!\n");  
        return -1;  
    }
    
    bmp_data_length = header->image_size;	//-整个位图数据的长度
    printf("bmp_data_length = %d\n", bmp_data_length);  
    
    //每行字节数,下面把所有位图数据读出
    buf = bmp_data_buf;  
    while ((ret = fread(buf,1,bmp_data_length,fp)) >= 0) {//-返回真实读取的项数，若大于count则意味着产生了错误。
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
    
    //-准备存储图片文件
    if(fb_path == NULL)  
    {
        printf("path Error,return\n");  
        return -1;  
    }
    printf("fb_path = %s\n", fb_path);  
    fb_file = fopen( fb_path, "wb" );  
    if(fb_file == NULL){
        printf("load cursor file open failed\n");  
        return -1;  
    }    
   
    ///重新计算，很重要！！ 
    width = header->width;
    height = header->height;
    bmp_data_length = header->image_size;
    bmp_buf_dst = (char*)calloc(1,bmp_data_length );  
    if(bmp_buf_dst == NULL){  
        printf("load > malloc bmp out ofmemory!\n");  
        return -1;  
    }  
    
    //-memcpy(FrameBuffer , bmp_data_buf , bmp_data_length); 
    cursor_bitmap_format_convert(bmp_buf_dst,bmp_data_buf);  //-目前转化中少最后多余的两个信息字节
    
#if 0
	//-实现图片屏幕显示
    for(j=0; j<vinfo.yres - 1; j++){	//-全屏底色是黑的
		   for(i=0; i<vinfo.xres - 1 ; i++) {
				fb_draw_point(FrameBuffer,vinfo.xres,vinfo.yres,i,j,0);
		   }
	}
	
	for(j = 0 ; j < vinfo.yres ; j++)  //-行 y
    {
        for(i = 0 ; i < vinfo.xres ;  i++)  //-列 x
        {
            if((j < height)  && (i < width))  
            {
            	//-*((unsigned short int *)bmp_buf_dst+j*width+i) = 0xffff;
            	//-0xF800、0x07E0、0x001F	分别表示红 绿 蓝
                *((unsigned short int *)FrameBuffer+j*vinfo.xres+i) = *((unsigned short int *)bmp_buf_dst+j*width+i);
            }
        }  
    }
#endif

    fwrite(bmp_head_buf,1,FileHead.cfoffBits,fb_file);  
    fwrite(bmp_data_buf,1,bmp_data_length,fb_file);  
    //-char *str="hello,I am a test program!";  
	//-fwrite(str,sizeof(char),strlen(str),fb_file);
	free(bmp_head_buf);
    free(bmp_data_buf);
    free(bmp_buf_dst);  
   
    fclose(fp);  
    fclose(fb_file);  
   
    printf("show logo return 0\n");  
    return 0;  
}
#endif
   
#if 0
int main()  
{  
   
    show_bmp( "/home/ubuntu/data.bmp","/home/ubuntu/data.fb");
   
    printf("the size of char is %d byte, the size of intis %d byte\n",sizeof(char),sizeof(int));  
}  
#endif
