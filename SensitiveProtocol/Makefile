# Sensitive Makefile
# Copyright (C) 2011 Massimo Gengarelli <massimo.gengarelli@gmail.com>

include ../Qwt_qt_config.makefile

INCLUDE		=	-I..

CXXFLAGS	=	-O3 -g

MOCFILES	=	moc_SensitiveServer.cpp
OBJECTS		=	moc_SensitiveServer.o SensitiveMessage.o SensitiveServer.o SensitiveClient.o

TARGET		=	libsnsprotocol.$(LIB_EXT)

.PHONY:	clean

all: $(TARGET)

moc_SensitiveServer.cpp: SensitiveServer.h
	$(QT_MOC) -o $@ $<

.cpp.o: %.c $(MOCFILES)
	$(CC) $(CXXFLAGS) $(INCLUDE) $(QT_CFLAGS) -o $@ -c $<

$(TARGET): $(OBJECTS) 
	$(CC) -shared $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(QT_NETWORK)
	cp $(TARGET) ../
	
clean:
	rm -fr $(TARGET) $(OBJECTS) $(MOCFILES)