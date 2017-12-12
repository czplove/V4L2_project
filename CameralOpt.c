#include "CameralOpt.h"  
int video_fd ;   
int length ;   
char *yuv[COUNT] ;   
struct v4l2_buffer  enqueue  , dequeue ;  //定义出入队的操作结构体成员  
  
int Init_Cameral(int Width , int Hight)  
{  
    //参数检查  
    char *videodevname = NULL ;   
    videodevname = "/dev/video0" ;   
      
    //打开设备  
    video_fd = open(videodevname , O_RDWR);  
    if(-1 == video_fd )  
    {  
        perror("open video device fail");  
        return -1 ;   
    }  
  
    int i ;   
    int ret ;   
    struct v4l2_format  format ;   
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ;   
    format.fmt.pix.width  = Width;   
    format.fmt.pix.height = Hight;   
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV ;  //我支持的格式是这个  
  
    ret = ioctl(video_fd , VIDIOC_S_FMT , &format);  //-设置当前驱动的频捕获格式
    if(ret != 0)  
    {  
        perror("set video format fail");  
        return -2 ;   
    }  
  
  
    //申请buffer,切割成几个部分  
    //3  
    struct v4l2_requestbuffers  requestbuffer ;   
    requestbuffer.count = COUNT ;   //-缓存数量，也就是说在缓存队列里保持多少张照片
    requestbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ;   //-数据流类型，必须永远是V4L2_BUF_TYPE_VIDEO_CAPTURE
    requestbuffer.memory = V4L2_MEMORY_MMAP ;   //-V4L2_MEMORY_MMAP 或 V4L2_MEMORY_USERPTR
  
    ret = ioctl(video_fd , VIDIOC_REQBUFS , &requestbuffer);  //-分配内存
    if(ret != 0)  
    {  
        perror("request buffer fail ");  
        return -3  ;  
    }  
      
      
    //querybuffer  
    struct v4l2_buffer querybuffer ;   
    querybuffer.type =  V4L2_BUF_TYPE_VIDEO_CAPTURE ;   //-buffer 类型
    querybuffer.memory = V4L2_MEMORY_MMAP ;   //-IO 方式，被应用程序设置
  
    for(i = 0 ; i < COUNT ; i++)  
    {
        querybuffer.index = i ;       //-buffer 序号
  
        ret = ioctl(video_fd , VIDIOC_QUERYBUF , &querybuffer);  //-把VIDIOC_REQBUFS中分配的数据缓存转换成物理地址
        if(ret != 0)  
        {  
            perror("query buffer fail");  
            return -4 ;   
        }  
      
      printf("index:%d length:%d  offset:%d \n" ,   
      querybuffer.index , querybuffer.length , querybuffer.m.offset);  
        length = querybuffer.length ;   //-缓冲帧长度 
  
        //-一个文件或者其它对象映射进内存
        //-start：映射区的开始地址，设置为0时表示由系统决定映射区的起始地址。
        //-length：映射区的长度。//长度单位是 以字节为单位，不足一内存页按一内存页处理
        //-prot：期望的内存保护标志，不能与文件的打开模式冲突。是以下的某个值，可以通过or运算合理地组合在一起
        //-PROT_EXEC //页内容可以被执行
        //-PROT_READ //页内容可以被读取
        //-PROT_WRITE //页可以被写入
        //-PROT_NONE //页不可访问
        //-flags：指定映射对象的类型，映射选项和映射页是否可以共享。MAP_SHARED //与其它所有映射这个对象的进程共享映射空间。对共享区的写入，相当于输出到文件。
        //-fd：有效的文件描述词。一般是由open()函数返回，其值也可以设置为-1，此时需要指定flags参数中的MAP_ANON,表明进行的是匿名映射。
        //-off_toffset：被映射对象内容的起点。
        //将摄像头内存印射到进程的内存地址  
        yuv[i] = mmap(0,querybuffer.length , PROT_READ | PROT_WRITE , MAP_SHARED , video_fd , querybuffer.m.offset );  
  
  
        //列队  
          
        struct v4l2_buffer  queuebuffer ;   
        queuebuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ;   
        queuebuffer.memory =  V4L2_MEMORY_MMAP ;   
        queuebuffer.index = i ;       
  
        ret = ioctl(video_fd , VIDIOC_QBUF , &queuebuffer);  //-把数据放回缓存队列
        if(ret != 0)  
        {  
            perror("queuebuffer fail");  
            return -5 ;   
        }
    }
    //初始化入队出队  
    enqueue.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ;   
    dequeue.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ;   
    enqueue.memory = V4L2_MEMORY_MMAP ;   
    dequeue.memory = V4L2_MEMORY_MMAP ;   
  
    return 0 ;   
}  
  
int Exit_Cameral(void)  
{  
    int i ;   
    for(i = 0 ; i < COUNT ; i++)  
        munmap(yuv+i , length);  
    close(video_fd);  
    return 0 ;   
}  
  
int Start_Cameral(void)  
{  
    //开启摄像头  
    int ret ;   
    int on = 1 ;   
    ret = ioctl(video_fd , VIDIOC_STREAMON , &on);  
    if(ret != 0)  
    {  
        perror("start Cameral fail");  
        return -1 ;   
    }  
    return 0 ;   
}  
int Stop_Cameral(void)  
{  
    //停止摄像头  
    int ret ;   
    int off= 1 ;   
    ret = ioctl(video_fd , VIDIOC_STREAMOFF, &off);  
    if(ret != 0)  
    {  
        perror("stop Cameral fail");  
        return -1 ;   
    }  
    return 0 ;  
}  
  
int Get_Picture(char *buffer)  
{  
    int ret ;   
    //出队  
    ret = ioctl(video_fd , VIDIOC_DQBUF , &dequeue);  //-把数据从缓存中读取出来
    if(ret != 0)  
    {  
        perror("dequeue fail");  
        return -1 ;   
    }  
  
    //获取图片数据 YUV   yuv[dequeue.index]  
    memcpy(buffer , yuv[dequeue.index] , dequeue.length);  
//  write(yuyv_fd , yuv[dequeue.index] , dequeue.length);  
  
    enqueue.index = dequeue.index ;   
    ret = ioctl(video_fd , VIDIOC_QBUF , &enqueue);  
    if(ret != 0)  
    {  
        perror("enqueue fail");  
        return -2 ;   
    }  
    return 0 ;   
}  