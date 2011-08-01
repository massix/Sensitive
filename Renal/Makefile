CC			=	g++

INCLUDES	=
LIBS		=

CXXFLAGS	= -O3 -DDEBUG
LDFLAGS		=

OBJECTS		= NextCalculator.o NextException.o LaGrangeCalculator.o NewtonCalculator.o

TARGET		= librenal.dll


.PHONY: clean

all: $(TARGET)

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<
	
$(TARGET): $(OBJECTS)
	$(CC) -shared $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
	cp $(TARGET) ..\
	
clean:
	rm -fr $(TARGET) $(OBJECTS)