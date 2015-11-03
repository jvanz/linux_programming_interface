CC=gcc
CFLAGS=-Wall

# --- sources ---
COMMON_SRC := $(wildcard src/common/*.c)
MEMORY_SRC := $(wildcard src/memory/*.c)
SAMPLES_SRC := $(wildcard src/samples/*.c)
# --- include ---
INCLUDE := -Iinclude
COMMON_INCLUDE := -Iinclude/common/
MEMORY_INCLUDE := -Iinclude/memory/
# --- objects ---
COMMON_OBJ := $(COMMON_SRC:src%.c=bin/obj%.o)
MEMORY_OBJ := $(MEMORY_SRC:src%.c=bin/obj%.o)
SAMPLES_OBJ := $(SAMPLES_SRC:src%.c=bin/obj%.o)
OBJECTS := $(COMMON_OBJ) $(MEMORY_OBJ) $(SAMPLES_OBJ)

.PHONY: all clean common memory directories samples debug

all: directories common memory samples

common: $(COMMON_OBJ)

memory: $(MEMORY_OBJ) $(SAMPLES_OBJ)

samples: $(MEMORY_OBJ) $(SAMPLES_OBJ)
	$(CC) $(CFLAGS) -o bin/memory-allocation $(MEMORY_OBJ) bin/obj/samples/memory-allocation.o

debug: CFLAGS += -g
debug: clean all

directories:
	mkdir -p bin/obj/{common,memory}

clean:
	rm $(OBJECTS)

bin/obj/common/%.o:src/common/%.c
	$(CC) $(CFLAGS) $(COMMON_INCLUDE) -c -o $@ $<

bin/obj/memory/%.o:src/memory/%.c
	$(CC) $(CFLAGS) $(MEMORY_INCLUDE) -c -o $@ $<

bin/obj/samples/%.o:src/samples/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<
