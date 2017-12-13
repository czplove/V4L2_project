#include "juv.h"  
/*
在摄像头之类编程经常是会碰到YUV格式,而非大家比较熟悉的RGB格式. 我们可以把YUV看成是一个RGB的变种来理解.
YUV的原理是把亮度与色度分离，研究证明,人眼对亮度的敏感超过色度。利用这个原理，可以把色度信息减少一点，人眼也无法查觉这一点。
YUV三个字母中，其中"Y"表示明亮度（Lumina nce或Luma），也就是灰阶值；而"U"和"V"表示的
则是色度（Chrominance或Chroma），作用是描述影像色彩及饱和度，用于指定像素的颜色。用这
个三个字母好象就是通道命令。

RGB格式中，一个24bpp像素要占用4字节空间。在YUV格式中,可以对于UV分量的数据压缩,但是对图
像整体质量影响不大，这样YUV所占的空间就比RGB要小一些不过RGB中 16bpp的 565格式每一个点
只占2个字节，从这一点看也没有省多少。不过视频应用都是清一色的YUV应用。因此YUV的处理还是
一个比较重要课题。
*/
#define BIT_COUNT   24  
  
void set_bmp_header(struct bmp_header_t *header, u32 width, u32 height)  
{
    header->magic = 0x4d42;  //-BMP格式的文件,字符显示就是‘BM’
    header->image_size = width * height * BIT_COUNT/8;  //-文件的大小
    header->file_size = header->image_size + 54;
    header->RESERVED1 = 0;  
    header->offset = 54;  //-表示从文件头到位图数据的偏移
  
    header->head_num = 40;  //-信息头的大小，即40
    header->width = width;  
    header->height = height;  
    header->color_planes = 1;  
    header->bit_count = BIT_COUNT;  //-若8，表示每个像素用8位表示，一共有256个颜色；
    header->bit_compression = 0;  
    header->h_resolution = 0;  
    header->v_resolution = 0;  
    header->color_num = 0;  
    header->important_colors = 0;  
}

/*
4:2:2 表示UV分量采样减半,比如第一个像素采样Y,U,第二个像素采样Y,V,依次类推,这样每个点占用
2个字节.二个像素组成一个宏像素.

4:2:2示例
   如果原始数据三个像素是 Y0 U0 V0 ,Y1 U1 V1,Y2 U2 V2,Y3 U3 V3
   经过4：2：2采样后，数据变成了 Y0 U0 ,Y1 V1 ,Y2 U2,Y3 V3
   如果还原后，因为某一些数据丢失就补成 Y0 U0 V1,Y1 U0 V1,Y2 U2 V3 ,Y3 U3 Y2
*/
//yuyv转rgb24的算法实现  
int yuyv2rgb24(u8 *yuyv, u8 *rgb, u32 width, u32 height)  
{
    u32 i, in, rgb_index = 0;  
    u8 y0, u0, y1, v1;  
    int r, g, b;  
    u32 out = 0, x, y;  
   
    for(in = 0; in < width * height * 2; in += 4)  
    {
	    y0 = yuyv[in+0];  //-取的纯图片数据
	    u0 = yuyv[in+1];  
	    y1 = yuyv[in+2];  
	    v1 = yuyv[in+3];  
	  
	    for (i = 0; i < 2; i++)  
	    {
	        if (i)  
	            y = y1;  
	        else  
	            y = y0;   
	        r = y + (140 * (v1-128))/100;  //r  
	        g = y - (34 * (u0-128))/100 - (71 * (v1-128))/100; //g  
	        b = y + (177 * (u0-128))/100; //b  
	        if(r > 255)   r = 255;  
	            if(g > 255)   g = 255;  
	            if(b > 255)   b = 255;  
	            if(r < 0)     r = 0;  
	            if(g < 0)     g = 0;  
	            if(b < 0)     b = 0;  
	  
	        y = height - rgb_index/width -1;  
	        x = rgb_index%width;  
	        rgb[(y*width+x)*3+0] = b;  
	        rgb[(y*width+x)*3+1] = g;  
	        rgb[(y*width+x)*3+2] = r;  
	        rgb_index++;  
	    }  
    }  
    return 0;  
}  
