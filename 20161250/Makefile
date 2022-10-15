CC=gcc
CFLAGS=-g -Wall
OBJS=main.o list.o list_utils.o hash.o hash_utils.o bitmap.o bitmap_utils.o hex_dump.o
TARGET=testlib
OUTPUT=Score.txt

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

main.o: list.h list_utils.h hash_utils.h bitmap.h bitmap_utils.h main.c
list.o: list.h
list_utils.o: list_utils.h
hash.o: hash.h
hash_utils.o: hash_utils.h
bitmap.o: bitmap.h
bitmap_utils.o: bitmap_utils.h
hex_dump.o: hex_dump.h

clean:
	rm $(OBJS) $(TARGET) $(OUTPUT)
