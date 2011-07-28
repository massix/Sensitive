CC			=	g++

QT_PATH		= 	C:\Qt\4.7.3
QT_INCLUDE	=	$(QT_PATH)\include
QT_LIBS		=	$(QT_PATH)\lib
QT_MOC		=	$(QT_PATH)\bin\moc.exe

INCLUDES	=	-I$(QT_INCLUDE) -I$(QT_INCLUDE)\QtGui -I$(QT_INCLUDE)\QtCore
LIBS		=	-L$(QT_LIBS) -lQtGui4 -lQtCore4

CXXFLAGS	= 	-O3 -ggdb -DDEBUG
LDFLAGS		=

MOCFILES	= 	moc_MainUI.cpp

OBJECTS		= 	moc_MainUI.o MainUI.o MainApplication.o

TARGET		= 	libgraphics.dll


.PHONY: clean

all: $(TARGET)

moc_MainUI.cpp: MainUI.h
	$(QT_MOC) -o $@ $<

.cpp.o: $(MOCFILES)
	$(CC) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<
	
$(TARGET): $(OBJECTS)
	$(CC) -shared $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
	cp $(TARGET) ..\
	
clean:
	rm -fr $(TARGET) $(OBJECTS) moc_*.cpp 