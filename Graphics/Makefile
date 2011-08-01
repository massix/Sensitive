CC			=	g++

QT_PATH		= 	C:\Qt\4.7.3
QT_INCLUDE	=	$(QT_PATH)\include
QT_LIBS		=	$(QT_PATH)\lib
QT_MOC		=	$(QT_PATH)\bin\moc.exe
QT_RCC		=	$(QT_PATH)\bin\rcc.exe

QWT_PATH	=	C:\Qwt-6.0.0
QWT_INCLUDE	=	$(QWT_PATH)\include
QWT_LIBS	=	$(QWT_PATH)\lib

INCLUDES	=	-I$(QT_INCLUDE) -I$(QT_INCLUDE)\QtGui -I$(QT_INCLUDE)\QtCore -I$(QWT_INCLUDE) -I..
LIBS		=	-L$(QT_LIBS) -lQtGui4 -lQtCore4 -lQtSvg4 -L$(QWT_LIBS) -lqwt -L..\Renal -lrenal

CXXFLAGS	= 	-O3
LDFLAGS		=

MOCFILES	= 	moc_MainUI.cpp moc_MainApplication.cpp moc_Starter.cpp moc_SLabel.h

OBJECTS		= 	moc_MainUI.o moc_MainApplication.o moc_Starter.o moc_SLabel.o AboutDialog.o MainUI.o MainApplication.o Starter.o SLabel.o qrc_Sensitive.o 

RESOURCES	=	qrc_Sensitive.cpp
RESOURCES_SRC = Sensitive.qrc

TARGET		= 	libgraphics.dll


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
	$(CC) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<
	
$(TARGET): $(OBJECTS) $(RESOURCES)
	$(CC) -shared $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
	cp $(TARGET) ..\
	
clean:
	rm -fr $(TARGET) $(OBJECTS) $(MOCFILES) $(RESOURCES) 