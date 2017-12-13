#ifndef __JYUV_H  
#define __JYUV_H  
  
typedef unsigned char  u8;  
typedef unsigned short u16;  
typedef unsigned int   u32;  

/*
BMP（全称Bitmap）是Windows操作系统中的标准图像文件格式
BMP格式的文件从头到尾依次是如下信息：
bmp文件头（bmp file header）：共14字节；
位图信息头（bitmap information）：共40字节；
调色板（color palette）：可选；
位图数据；

最常见的就是24位图，所谓的24位图，就是说一个像素的颜色信息用24位来表示，也就是说，对
于三原色BRG，每一个颜色都用以字节（8）位来表示。除了24位图，还有1位（单色），2位（4
色，CGA），4位（16色，VGA），8位（256色），16位（增强色），24位（真彩色）和32位等。
*/

#pragma pack(1)  
//定义bmp头  
struct bmp_header_t{
    u16        magic;  		//-文件类型
    u32       file_size;  	//-文件大小
    u32       RESERVED1;  	//-保留，必须设置为0
    u32       offset;         //54 bytes 表示54个偏移量(从头到位图数据的偏移)  
  
    u32       head_num;    //40  信息头的大小，即40
    u32       width;  		//-以像素为单位说明图像的宽度
    u32       height;  		//-以像素为单位说明图像的高度
    u16       color_planes; //1  为目标设备说明颜色平面数，总被设置为1
    u16       bit_count;  	//-说明比特数/像素数，值有1、2、4、8、16、24、32；
    u32       bit_compression; //0  说明图像的压缩类型，最常用的就是0（BI_RGB），表示不压缩
    u32       image_size; //except the size of header  说明位图数据的大小，当用BI_RGB格式时，可以设置为0
    u32       h_resolution;  //-表示水平分辨率，单位是像素/米，有符号整数；
    u32       v_resolution;  //-表示垂直分辨率，单位是像素/米，有符号整数；
    u32       color_num;  //-说明位图使用的调色板中的颜色索引数，为0说明使用所有；
    u32       important_colors;  //-说明对图像显示有重要影响的颜色索引数，为0说明都重要；
};  
  
#pragma pack()  
  
void set_bmp_header(struct bmp_header_t * header, u32 width, u32 height);  
int yuyv2rgb24(u8 *yuyv, u8 *rgb, u32 width, u32 height);  
  
#endif /* __JYUV_H */   