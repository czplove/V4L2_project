/* linux_bmp.h*/
/*
 * Copyright (c)2010 Chris Grant
 *
 * Win32 USER structures and API
 */
#include <iostream>
typedef   struct   tagBITMAPFILEHEADER{   
        unsigned   short   bfType;       //2  此处需留意
        unsigned   long    bfSize;         //4                              /*   File   size   in   bytes                     */   
        unsigned   short   bfReserved1;   //2
        unsigned   short   bfReserved2;   //2 
        unsigned   long    bfOffBits;      //4                                  /*   Offset   to   image   data,   bytes   */   
  } __attribute__((packed))BITMAPFILEHEADER,   *PBITMAPFILEHEADER;  //Attention:"__"是两个"_"!  字边界对齐！4字节对齐   知道在linux的gcc下默认的是四字节的
    
typedef   struct   tagBITMAPINFOHEADER{   
        unsigned   long    biSize;              //4                 /*   Header   size   in   bytes            */   
         long         biWidth;               //4              /*   Width   of   image                         */   
         long         biHeight;                  //4          /*   Height   of   image                       */   
        unsigned short   biPlanes;        //2        /*   Number   of   colour   planes       */   
        unsigned short   biBitCount;       //2     /*   Bits   per   pixel                         */   
        unsigned   long biCompression;       //4           /*   Compression   type                     */   
        unsigned   long biSizeImage;    //4                  /*   Image   size   in   bytes               */   
        long   biXPelsPerMeter;     //4
        long   biYPelsPerMeter;      //4  /*   Pixels   per   meter                     */   
        unsigned   long biClrUsed;       //4                   /*   Number   of   colours                   */   
        unsigned   long biClrImportant;       //4         /*   Important   colours                   */   
  } __attribute__((packed))BITMAPINFOHEADER,*PBITMAPINFOHEADER;   
    
typedef   struct   tagRGBQUAD   {   
 unsigned   char   rgbBlue;   /*   Blue   value     */   
 unsigned   char   rgbGreen;   /*   Green   value   */   
 unsigned   char   rgbRed;   /*   Red   value       */   
 unsigned   char   rgbReserved;   /*   Reserved         */   
  }   RGBQUAD;   