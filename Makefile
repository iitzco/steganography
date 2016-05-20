TARGET = stegowav
LIBS = -lm
CC = gcc
IDIR = ./include
CFLAGS = -g -Wall -I$(IDIR)
LDFLAGS = -lcrypto

.PHONY: default all clean format

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(shell find . -name '*.c'))
HEADERS = $(wildcard */*.h)
OBJ = $(shell find . -name '*.o')

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -Wall $(LIBS) -o $@

clean:
	find . -type f -name '*.o' -delete
	-rm -f $(TARGET)

format:
	find . -type f -name '*.c' | xargs clang-format -i
	find . -type f -name '*.h' | xargs clang-format -i
