# Compiler and compile options.
#

CC = gcc
CFLAGS = -g -Wall

# Macros specifying path for compile.
SRCS := $(wildcard src/*.c)
HEADERS := $(wildcard src/*.h)

OBJS := $(SRCS:.c=.o)

# Result Binary path
BIN = ./bin/

# If you have arguments
# ARGS = many-input2.txt

TESTFILES := $(wildcard tests/*.txt)

# Result binary name
TARGET = main

target : $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN)$(TARGET) $(OBJS)
ifdef ARGS
	$(BIN)$(TARGET) $(ARGS)
else
	$(BIN)$(TARGET)
endif

test : target
	printf "\n\n"
	for file in $(TESTFILES) ; do \
		$(BIN)$(TARGET) $${file}; \
		printf "\n\n"; \
	done \

#Delete binary 
clean :
	rm -f $(BIN)$(TARGET) $(OBJS)
