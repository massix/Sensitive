# Sensitive Makefile
# Copyright (C) 2011 Massimo Gengarelli <massimo.gengarelli@gmail.com>

include ../Qwt_qt_config.makefile

INCLUDES	=
LIBS		=

LDFLAGS		=

OBJECTS		= 	NextCalculator.o \
				NextException.o \
				LaGrangeCalculator.o \
				NewtonCalculator.o

TARGET		= librenal.$(LIB_EXT)


.PHONY: all clean

all: $(TARGET)

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<
	
$(TARGET): $(OBJECTS)
	$(CC) -shared $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
	cp $(TARGET) ../
	
clean:
	rm -fr $(TARGET) $(OBJECTS)
