SUBDIRS		= Gui Renal

CC			= g++

CFLAGS		= -O3 -ggdb

INCLUDE		= -IRenal -I.
LIBS		= -LRenal -lrenal

TARGET		= Sensitive.exe
OBJECTS		= Sensitive.o

INTERNAL_LIBS	= librenal.dll

.PHONY: clean $(INTERNAL_LIBS)


all: $(TARGET)

librenal.dll:
	cd Renal; make all
	
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	
$(TARGET): $(INTERNAL_LIBS) $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)
	
clean:
	rm -fr $(TARGET) $(OBJECTS) $(INTERNAL_LIBS)
	cd Renal; make clean