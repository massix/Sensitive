# Sensitive Makefile
# Copyright (C) 2011 Massimo Gengarelli <massimo.gengarelli@gmail.com>

include Qwt_qt_config.makefile

LDFLAGS		=  $(OS_LDFLAGS)

INCLUDE		= -I.
LIBS		= -LRenal -LGraphics -lgraphics -lrenal

ifeq ($(OS),Windows_NT)
	RESOURCES	= Sensitive.res
endif

TARGET		= Sensitive$(EXE_EXT)
OBJECTS		= Sensitive.o

INTERNAL_LIBS	= librenal.$(LIB_EXT) libgraphics.$(LIB_EXT)

.PHONY: clean $(INTERNAL_LIBS)


all: $(TARGET)

librenal.$(LIB_EXT):
	cd Renal; make all
	
libgraphics.$(LIB_EXT):
	cd Graphics; make all

Sensitive.res: Resources.rc
	$(MINGW_WRES) -O coff -o $(RESOURCES) Resources.rc

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDE) $(QT_CFLAGS) $(QWT_CFLAGS) -c $< -o $@
	
$(TARGET): $(INTERNAL_LIBS) $(OBJECTS) $(RESOURCES)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(RESOURCES) $(QT_LDFLAGS) $(QWT_LDFLAGS) $(LIBS)
	
win32installer: Sensitive.nsi
	$(NSIS_MAKE) Sensitive.nsi
	
clean:
	rm -fr $(TARGET) $(OBJECTS) $(INTERNAL_LIBS) $(RESOURCES)
	cd Renal; make clean
	cd Graphics; make clean
