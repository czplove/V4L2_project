# ------------------------------------------------------------------
# MQTT Client makefile
# ------------------------------------------------------------------
# Author:    taida
# Copyright: NXP B.V. 2016. All rights reserved
# ------------------------------------------------------------------
#-��������һ��makefile��

#-LDFLAGS += -lpthread
CC=arm-none-linux-gnueabi-gcc
#CC=gcc

TARGET = v4l2t
#-����ͷ�ļ��ĵط�
INCLUDES = -I.
OBJECTS = project.o \
	CameralOpt.o \
	FrameBufferOpt.o \
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

