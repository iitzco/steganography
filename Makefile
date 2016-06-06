TARGET = stegowav
LIBS = -lm
CC = gcc
IDIRS = -I./include -I/usr/local/opt/openssl/include
CFLAGS = -g -Wall $(IDIRS)
LDFLAGS = -lcrypto

.PHONY: default all clean test format

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

test: $(TARGET)
	@./test/run_tests.sh

format:
	find . -type f -name '*.c' | xargs clang-format -i
	find . -type f -name '*.h' | xargs clang-format -i
