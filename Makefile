# Sensitive Makefile
# Copyright (C) 2011 Massimo Gengarelli <massimo.gengarelli@gmail.com>

include Qwt_qt_config.makefile

LDFLAGS		=  $(OS_LDFLAGS) -g

INCLUDE		= -I.
LIBS		= -LRenal -LGraphics -LSensitiveProtocol -lgraphics -lrenal -lsnsprotocol

ifeq ($(OS),Windows_NT)
	RESOURCES	= Sensitive.res
endif

TARGET		= Sensitive$(EXE_EXT)
OBJECTS		= Sensitive.o


# Targets
LIBRENAL		= librenal.$(LIB_EXT)
LIBSNSPROTOCOL	= libsnsprotocol.$(LIB_EXT)
LIBGRAPHICS		= libgraphics.$(LIB_EXT)
INVERSITIVE		= Inversitive_UI$(EXE_EXT)

.PHONY: clean $(INVERSITIVE) $(LIBRENAL) $(LIBSNSPROTOCOL) $(LIBGRAPHICS)


all: $(INVERSITIVE) $(TARGET)

$(LIBRENAL):
	cd Renal && $(MAKE)

$(LIBSNSPROTOCOL):
	cd SensitiveProtocol && $(MAKE)
	
$(LIBGRAPHICS): $(LIBRENAL) $(LIBSNSPROTOCOL) 
	cd Graphics && $(MAKE)

$(INVERSITIVE): $(LIBSNSPROTOCOL)
	cd Inversitive && $(MAKE)

Sensitive.res: Resources.rc
	$(MINGW_WRES) -O coff -o $(RESOURCES) Resources.rc

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDE) $(QT_CFLAGS) $(QWT_CFLAGS) -c $< -o $@
	
$(TARGET): $(OBJECTS) $(LIBRENAL) $(LIBSNSPROTOCOL) $(LIBGRAPHICS) $(RESOURCES)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(RESOURCES) $(QT_LDFLAGS) $(QWT_LDFLAGS) $(LIBS)
	
win32installer: $(TARGET) $(INVERSITIVE) Sensitive.nsi
	$(NSIS_MAKE) Sensitive.nsi
	
clean:
	rm -fr $(TARGET) $(OBJECTS) $(LIBRENAL) $(LIBGRAPHICS) $(LIBSNSPROTOCOL) $(INVERSITIVE) $(RESOURCES)
	cd Renal && $(MAKE) clean
	cd Graphics && $(MAKE) clean
	cd SensitiveProtocol && $(MAKE) clean
	cd Inversitive && $(MAKE) clean
