# Sensitive Makefile
# Copyright (C) 2011 Massimo Gengarelli <massimo.gengarelli@gmail.com>

include ../Qwt_qt_config.makefile

INCLUDE		=	-I..
LIBS		=	-L../Renal -lrenal -L../SensitiveProtocol -lsnsprotocol

LDFLAGS		=	-g

MOCFILES	= 	moc_MainUI.cpp \
				moc_MainApplication.cpp \
				moc_Starter.cpp \
				moc_SLabel.cpp \
				moc_STableWidget.cpp \
				moc_SPlot.cpp \
				moc_ServerWindow.cpp \
				moc_ClientWindow.cpp

OBJECTS		= 	moc_MainUI.o \
				moc_MainApplication.o \
				moc_Starter.o \
				moc_SLabel.o \
				moc_STableWidget.o \
				moc_SPlot.o \
				moc_ServerWindow.o \
				moc_ClientWindow.o \
				AboutDialog.o \
				MainUI.o \
				MainApplication.o \
				Starter.o \
				SLabel.o \
				SPlot.o \
				STableWidget.o \
				ServerWindow.o \
				ClientWindow.o \
				qrc_Sensitive.o 

RESOURCES	=	qrc_Sensitive.cpp
RESOURCES_SRC = Sensitive.qrc

TARGET		= 	libgraphics.$(LIB_EXT)


.PHONY: clean

all: $(TARGET)

moc_MainUI.cpp: MainUI.h
	$(QT_MOC) -o $@ $<
	
moc_MainApplication.cpp: MainApplication.h
	$(QT_MOC) -o $@ $<
	
moc_Starter.cpp: Starter.h
	$(QT_MOC) -o $@ $<
	
moc_SLabel.cpp: SLabel.h
	$(QT_MOC) -o $@ $<

moc_STableWidget.cpp: STableWidget.h
	$(QT_MOC) -o $@ $<
	
moc_SPlot.cpp: SPlot.h
	$(QT_MOC) -o $@ $<
	
moc_ServerWindow.cpp: ServerWindow.h
	$(QT_MOC) -o $@ $<
	
moc_ClientWindow.cpp: ClientWindow.h
	$(QT_MOC) -o $@ $<

qrc_Sensitive.cpp:
	$(QT_RCC) -name Sensitive -o $(RESOURCES) $(RESOURCES_SRC)

.cpp.o: $(MOCFILES) $(RESOURCES)
	$(CC) $(CXXFLAGS) $(INCLUDE) $(QT_CFLAGS) $(QWT_CFLAGS) -o $@ -c $<
	
$(TARGET): $(OBJECTS) $(RESOURCES)
	$(CC) -shared $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(QT_LDFLAGS) $(QWT_LDFLAGS)
	cp $(TARGET) ../
	
clean:
	rm -fr $(TARGET) $(OBJECTS) $(MOCFILES) $(RESOURCES) 
