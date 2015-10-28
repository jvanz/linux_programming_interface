CC=gcc
CC_FLAGS=-Wall

# --- sources ---
COMMON_SRC := $(wildcard src/common/*.c)
MEMORY_SRC := $(wildcard src/memory/*.c)
# --- include ---
COMMON_INCLUDE := -Iinclude/common/
MEMORY_INCLUDE := -Iinclude/memory/
# --- objects ---
COMMON_OBJ := $(COMMON_SRC:src%.c=bin/obj%.o)
MEMORY_OBJ := $(MEMORY_SRC:src%.c=bin/obj%.o)
OBJECTS := $(COMMON_OBJ) $(MEMORY_OBJ)

.PHONY: all clean common memory directories

all: common memory

common: $(COMMON_OBJ)

memory: $(MEMORY_OBJ)

clean:
	rm $(OBJECTS)

directories:
	mkdir -p bin/obj/{common,memory}

bin/obj/common/%.o:src/common/%.c
	$(CC) $(CC_FLAGS) $(COMMON_INCLUDE) -c -o $@ $<

bin/obj/memory/%.o:src/memory/%.c
	$(CC) $(CC_FLAGS) $(MEMORY_INCLUDE) -c -o $@ $<
