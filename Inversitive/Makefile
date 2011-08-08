# Sensitive Makefile
# Copyright (C) 2011 Massimo Gengarelli <massimo.gengarelli@gmail.com>

include ../Qwt_qt_config.makefile

INCLUDE		=	
LIBS		=	

QT_CFLAGS	+=	-I$(QT_INCLUDE)/QtUiTools

ifeq ($(OS),Windows_NT)
	RC_FILE	= Inversitive.res
endif

LDFLAGS		=	$(OS_LDFLAGS)

MOCFILES	=	moc_MainUI.cpp

OBJECTS		=	moc_MainUI.o MainUI.o Inversitive.o qrc_Inversitive.o

TARGET		=	Inversitive$(EXE_EXT)

RESOURCES	=	qrc_Inversitive.cpp
RESOURCES_SRC = Inversitive.qrc

all: $(TARGET) $(RESOURCES) $(OBJECTS)

.PHONY: clean

moc_MainUI.cpp: MainUI.h
	$(QT_MOC) -o $@ $<

.cpp.o: %.cpp
	$(CC) $(CXXFLAGS) $(INCLUDE) $(QT_CFLAGS) -o $@ -c $<

$(RESOURCES): $(RESOURCES_SRC)
	$(QT_RCC) -name Inversitive -o $(RESOURCES) $(RESOURCES_SRC)

$(RC_FILE): Resources.rc
	$(MINGW_WRES) -O coff -o $(RC_FILE) Resources.rc

$(TARGET): $(OBJECTS) $(RC_FILE)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(RC_FILE) $(LIBS) $(QT_LDFLAGS)
	cp $(TARGET) ../

clean:
	rm -fr $(TARGET) $(OBJECTS) $(RESOURCES) $(MOCFILES) $(RC_FILE)