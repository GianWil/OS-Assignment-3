CC = gcc
CFLAGS = -Wall -pthread -O2

TARGET = test-mergesort
OBJS = test-mergesort.o mergesort.o

all: $(TARGET)

$(TARGET): $(OBJS)
<TAB>$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

mergesort.o: mergesort.c mergesort.h
<TAB>$(CC) $(CFLAGS) -c mergesort.c

test-mergesort.o: test-mergesort.c mergesort.h
<TAB>$(CC) $(CFLAGS) -c test-mergesort.c

clean:
<TAB>rm -f *.o $(TARGET)