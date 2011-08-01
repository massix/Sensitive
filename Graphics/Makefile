# Sensitive Makefile
# Copyright (C) 2011 Massimo Gengarelli <massimo.gengarelli@gmail.com>

include ../Qwt_qt_config.makefile

INCLUDE		=	-I..
LIBS		=	-L../Renal -lrenal

CXXFLAGS	= 	-O3
LDFLAGS		=

MOCFILES	= 	moc_MainUI.cpp moc_MainApplication.cpp moc_Starter.cpp moc_SLabel.cpp

OBJECTS		= 	moc_MainUI.o moc_MainApplication.o moc_Starter.o moc_SLabel.o AboutDialog.o MainUI.o MainApplication.o Starter.o SLabel.o qrc_Sensitive.o 

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
	
qrc_Sensitive.cpp:
	$(QT_RCC) -name Sensitive -o $(RESOURCES) $(RESOURCES_SRC)

.cpp.o: $(MOCFILES) $(RESOURCES)
	$(CC) $(CXXFLAGS) $(INCLUDE) $(QT_CFLAGS) $(QWT_CFLAGS) -o $@ -c $<
	
$(TARGET): $(OBJECTS) $(RESOURCES)
	$(CC) -shared $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(QT_LDFLAGS) $(QWT_LDFLAGS)
	cp $(TARGET) ../
	
clean:
	rm -fr $(TARGET) $(OBJECTS) $(MOCFILES) $(RESOURCES) 
