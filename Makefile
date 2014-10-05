CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=
SOURCES=xtrinsic_mag.c mpl3115.c mag3110.c mma8491.c gpio.c i2c.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=xtrinsic_mag

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -lm -o $@ 

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf *.o $(EXECUTABLE)
