# Sensitive Makefile
# Copyright (C) 2011 Massimo Gengarelli <massimo.gengarelli@gmail.com>

include ../Qwt_qt_config.makefile

INCLUDE		=	-I..
LIBS		=	-L../Renal -lrenal -L../SensitiveProtocol -lsnsprotocol

LDFLAGS		=

MOCFILES	= 	moc_MainUI.cpp \
				moc_MainApplication.cpp \
				moc_Starter.cpp \
				moc_SLabel.cpp \
				moc_STableWidget.cpp \
				moc_SPlot.cpp \
				moc_ServerWindow.cpp \
				moc_ClientWindow.cpp

OBJECTS		= 	$(MOCFILES:.cpp=.o) \
				$(RESOURCES:.cpp=.o) \
				AboutDialog.o \
				MainUI.o \
				MainApplication.o \
				Starter.o \
				SLabel.o \
				SPlot.o \
				STableWidget.o \
				ServerWindow.o \
				ClientWindow.o

RESOURCES	=	qrc_Sensitive.cpp
RESOURCES_SRC = Sensitive.qrc

TARGET		= 	libgraphics.$(LIB_EXT)


.PHONY: all clean

all: $(TARGET)

%.cpp:
	$(QT_MOC) -o $@ $(subst .cpp,.h,$(subst moc_,,$@))

$(RESOURCES): $(RESOURCES_SRC)
	$(QT_RCC) -name Sensitive -o $(RESOURCES) $(RESOURCES_SRC)

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDE) $(QT_CFLAGS) $(QWT_CFLAGS) -o $@ -c $<
	
$(TARGET): $(MOCFILES) $(OBJECTS) $(RESOURCES)
	$(CC) -shared $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(QT_LDFLAGS) $(QWT_LDFLAGS)
	cp $(TARGET) ../
	
clean:
	rm -fr $(TARGET) $(OBJECTS) $(MOCFILES) $(RESOURCES) 
