SUBDIRS		= Gui Renal

QT_PATH		= 	C:\Qt\4.7.3
QT_INCLUDE	=	$(QT_PATH)\include
QT_LIBS		=	$(QT_PATH)\lib
QT_MOC		=	$(QT_PATH)\bin\moc.exe

QWT_PATH	=	C:\Qwt-6.0.0
QWT_INCLUDE	=	$(QWT_PATH)\include

CC			= g++

CFLAGS		= -O3

LDFLAGS		=  -enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc -Wl,-s -mthreads -Wl -Wl,-subsystem,windows

INCLUDE		= -IRenal -I. -I$(QT_INCLUDE) -I$(QT_INCLUDE)\QtGui -I$(QT_INCLUDE)\QtCore -I$(QWT_INCLUDE)
LIBS		= -LRenal -LGraphics -lgraphics -lrenal -L$(QT_LIBS)  -lQtGui4 -lQtCore4

TARGET		= Sensitive.exe
OBJECTS		= Sensitive.o

INTERNAL_LIBS	= librenal.dll libgraphics.dll

.PHONY: clean $(INTERNAL_LIBS)


all: $(TARGET)

librenal.dll:
	cd Renal; make all
	
libgraphics.dll:
	cd Graphics; make all
	
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	
$(TARGET): $(INTERNAL_LIBS) $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
	
clean:
	rm -fr $(TARGET) $(OBJECTS) $(INTERNAL_LIBS)
	cd Renal; make clean
	cd Graphics; make clean
