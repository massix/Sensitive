# Sensitive Makefile
# Copyright (C) 2011 Massimo Gengarelli <massimo.gengarelli@gmail.com>

include ../Qwt_qt_config.makefile

INCLUDE		=	-I..

LDFLAGS		=

MOCFILES	=	moc_SensitiveServer.cpp \
				moc_SensitiveClient.cpp

OBJECTS		=	$(MOCFILES:.cpp=.o) \
				SensitiveMessage.o \
				SensitiveServer.o \
				SensitiveClient.o \
				SensitiveException.o

TARGET		=	libsnsprotocol.$(LIB_EXT)

.PHONY:	all clean

all: $(TARGET)

%.cpp:
	$(QT_MOC) -o $@ $(subst .cpp,.h,$(subst moc_,,$@))

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDE) $(QT_CFLAGS) -o $@ -c $<

$(TARGET): $(MOCFILES) $(OBJECTS)
	$(CC) -shared $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(QT_NETWORK)
	cp $(TARGET) ../
	
clean:
	rm -fr $(TARGET) $(OBJECTS) $(MOCFILES)
