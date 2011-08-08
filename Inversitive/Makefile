# Sensitive Makefile
# Copyright (C) 2011 Massimo Gengarelli <massimo.gengarelli@gmail.com>

include ../Qwt_qt_config.makefile

INCLUDE		=
LIBS		=

QT_CFLAGS	+=	-I$(QT_INCLUDE)/QtUiTools

ifeq ($(OS),Windows_NT)
	RC_FILE	= Inversitive.res
	RC_SRC	= Resources.rc
endif

LDFLAGS		=	$(OS_LDFLAGS)

MOCFILES	=	moc_MainUI.cpp

OBJECTS		=	$(MOCFILES:.cpp=.o) \
				$(RESOURCES:.cpp=.o) \
				MainUI.o \
				Inversitive.o

TARGET		=	Inversitive_UI$(EXE_EXT)

RESOURCES	=	qrc_Inversitive.cpp
RESOURCES_SRC = Inversitive.qrc

all: $(TARGET) $(RESOURCES) $(OBJECTS)

.PHONY: all clean

%.cpp:
	$(QT_MOC) -o $@ $(subst .cpp,.h,$(subst moc_,,$@))

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDE) $(QT_CFLAGS) -o $@ -c $<

$(RESOURCES): $(RESOURCES_SRC)
	$(QT_RCC) -name Inversitive -o $(RESOURCES) $(RESOURCES_SRC)

$(RC_FILE): $(RC_SRC)
	$(MINGW_WRES) -O coff -o $(RC_FILE) $(RC_SRC)

$(TARGET): $(MOCFILES) $(OBJECTS) $(RC_FILE)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(RC_FILE) $(LIBS) $(QT_LDFLAGS)
	cp $(TARGET) ../

clean:
	rm -fr $(TARGET) $(OBJECTS) $(RESOURCES) $(MOCFILES) $(RC_FILE)
