# Sensitive Makefile
# Copyright (C) 2011 Massimo Gengarelli <massimo.gengarelli@gmail.com>


# Generic variables

CC			=	g++
CXXFLAGS	= 	-O3 -DQT_SHARED -DNO_DEBUG

ifeq ($(OS),Windows_NT)
	QT_PATH		= 	C:\Qt\4.7.3
	QT_INCLUDE	=	$(QT_PATH)\include

	QT_LIBS		=	$(QT_PATH)\lib
	QT_MOC		=	$(QT_PATH)\bin\moc.exe
	QT_RCC		=	$(QT_PATH)\bin\rcc.exe

	QWT_PATH	=	C:\Qwt-6.0.0
	QWT_INCLUDE	=	$(QWT_PATH)\include
	QWT_LIBS	=	$(QWT_PATH)\lib

	QT_LDFLAGS	=	-L$(QT_LIBS) -lQtUiTools -lQtGui4 -lQtCore4 -lQtSvg4
	QT_NETWORK	=	-L$(QT_LIBS) -lQtNetwork4 -lQtCore4

	LIB_EXT		=	dll

	MINGW_PATH	=	C:\MinGW
	MINGW_WRES	=	$(MINGW_PATH)\bin\windres.exe
	
	NSIS_PATH	=	C:\Program Files\NSIS
	NSIS_MAKE	=	"$(NSIS_PATH)\makensis.exe"


	OS_LDFLAGS	=	-enable-stdcall-fixup -Wl,-enable-auto-import \
					-Wl,-enable-runtime-pseudo-reloc -Wl,-s -mthreads \
					-Wl -Wl,-subsystem,windows

	EXE_EXT		=	.exe
else
	QT_PATH 	= 	/usr/include/qt4
	QT_INCLUDE 	= 	$(QT_PATH)

	QT_LIBS		=	/usr/lib
	QT_MOC		=	moc-qt4
	QT_RCC		=	rcc

	QWT_PATH	=	/usr/local/qwt-6.0.1
	QWT_INCLUDE	=	$(QWT_PATH)/include
	QWT_LIBS	=	$(QWT_PATH)/lib

	QT_LDFLAGS 	=	-L$(QT_LIBS) -lQtGui -lQtCore -lQtSvg
	QT_NETWORK	=	-L$(QT_LIBS) -lQtNetwork

	LIB_EXT		=	so

	OS_LDFLAGS	=

	EXE_EXT		=
endif


QT_CFLAGS	=	-I$(QT_INCLUDE) -I$(QT_INCLUDE)/QtGui -I$(QT_INCLUDE)/QtCore -I$(QT_INCLUDE)/QtNetwork

QWT_CFLAGS	=	-I$(QWT_INCLUDE)
QWT_LDFLAGS	=	-L$(QWT_LIBS) -lqwt
