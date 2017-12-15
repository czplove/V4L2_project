# ------------------------------------------------------------------
# MQTT Client makefile
# ------------------------------------------------------------------
# Author:    taida
# Copyright: NXP B.V. 2016. All rights reserved
# ------------------------------------------------------------------
#-这个是最后一个makefile了

#-LDFLAGS += -lpthread
#CC=arm-none-linux-gnueabi-gcc
CC=gcc

TARGET = v4l2t
#-包含头文件的地方
INCLUDES = -I.
OBJECTS = project.o \
	CameralOpt.o \
	FrameBufferOpt.o \
	BmpOpt.o \
	juv.o


%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -Wall -g -c $< -o $@

all: clean build

build: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(TARGET)
#	cp $(TARGET) ../../swupdate/images/usr/bin/

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
#	-rm -f ../../swupdate/images/usr/bin/$(TARGET)

