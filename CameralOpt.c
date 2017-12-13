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
    struct v4l2_capability cap; 
    int ret ; 
      
    //打开设备  
    video_fd = open(videodevname , O_RDWR);  
    if(-1 == video_fd )  
    {  
        perror("open video device fail");  
        return -1 ;   
    }
    
    ret = ioctl(video_fd,VIDIOC_QUERYCAP,&cap);	//-查询设备属性
    if(ret != 0)  
    {
        perror("Query device properties fail");  
        return -2 ;   
    }
    printf("Driver Name:%s\nCard Name:%s\nBus info:%s\nDriver Version:%u.%u.%u\n",
    	cap.driver,cap.card,cap.bus_info,(cap.version>>16)&0XFF, (cap.version>>8)&0XFF,cap.version&0XFF);
    printf("Device support operation = %d\r\n",cap.capabilities);  
    
    struct v4l2_format fmt; 
    fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE; 
    ioctl(video_fd, VIDIOC_G_FMT, &fmt);	//-显示当前帧的相关信息
	printf("Current data format information:\n\twidth:%d\n\theight:%d\n",
		fmt.fmt.pix.width,fmt.fmt.pix.height);
    
    struct v4l2_fmtdesc fmtdesc; 
    fmtdesc.index=0; 
    fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE; 
    printf("Support format:\n");
    while(ioctl(video_fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1)	//-显示所有支持的格式
	{
		printf("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
		fmtdesc.index++;
	}
	
	//-struct v4l2_format fmt; 
	fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	fmt.fmt.pix.pixelformat=V4L2_PIX_FMT_RGB32; 
	if(ioctl(video_fd,VIDIOC_TRY_FMT,&fmt)==-1)	//-检查是否支持某种帧格式
		if(errno==EINVAL)
			printf("not support format RGB32!\n");
    
  
    int i ;   
    //-int ret ;   
    struct v4l2_format  format ;   
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ;   
    format.fmt.pix.width  = Width;   
    format.fmt.pix.height = Hight;   
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV ;  //我支持的格式是这个  v4l2_fourcc('Y', 'U', 'Y', 'V') /* 16  YUV 4:2:2     */
  
    ret = ioctl(video_fd , VIDIOC_S_FMT , &format);  //-设置当前驱动的频捕获格式
    if(ret != 0)  
    {  
        perror("set video format fail");  
        return -2 ;   
    }
  
  	//-应用程序和设备有三种交换数据的方法，直接 read/write、内存映射(memory mapping)和用户指针。
    //申请buffer,切割成几个部分  
    //3  
    struct v4l2_requestbuffers  requestbuffer ;   
    requestbuffer.count = COUNT ;   //-缓存数量，也就是说在缓存队列里保持多少张照片
    requestbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ;   //-数据流类型，必须永远是V4L2_BUF_TYPE_VIDEO_CAPTURE
    requestbuffer.memory = V4L2_MEMORY_MMAP ;   //-V4L2_MEMORY_MMAP 或 V4L2_MEMORY_USERPTR(区别是内存映射还是用户指针方式)
  
    ret = ioctl(video_fd , VIDIOC_REQBUFS , &requestbuffer);  //-向设备申请缓冲区
    if(ret != 0)  
    {  
        perror("request buffer fail ");  
        return -3  ;  
    }  
      
    //-获取并记录缓存的物理空间
    //-使用VIDIOC_REQBUFS，我们获取了req.count个缓存，下一步通过调用VIDIOC_QUERYBUF
    //-命令来获取这些缓存的地址，然后使用mmap函数转换成应用程序中的绝对地址，最后把这
    //-段缓存放入缓存队列
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
  
        ret = ioctl(video_fd , VIDIOC_QBUF , &queuebuffer);  //-将缓冲帧放入队列
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
    int on = V4L2_BUF_TYPE_VIDEO_CAPTURE ;   
    ret = ioctl(video_fd , VIDIOC_STREAMON , &on);  //-启动数据流
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
    int off= V4L2_BUF_TYPE_VIDEO_CAPTURE ;   
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
    ret = ioctl(video_fd , VIDIOC_DQBUF , &dequeue);  //-从缓冲区取出一个缓冲帧
    if(ret != 0)  
    {  
        perror("dequeue fail");  
        return -1 ;   
    }  
  
    //获取图片数据 YUV   yuv[dequeue.index]  
    memcpy(buffer , yuv[dequeue.index] , dequeue.length);  
//  write(yuyv_fd , yuv[dequeue.index] , dequeue.length);  
  
    enqueue.index = dequeue.index ;   
    ret = ioctl(video_fd , VIDIOC_QBUF , &enqueue);  //-放回队列中就可以再次获取到数据了
    if(ret != 0)  
    {
        perror("enqueue fail");  
        return -2 ;   
    }  
    return 0 ;   
}