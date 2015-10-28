CC=gcc
CC_FLAGS=

# --- sources ---
COMMON_SRC := $(wildcard src/common/*.c)
MEMORY_SRC := $(wildcard src/memory/*.c)
# --- include ---
COMMON_INCLUDE := -Iinclude/common/
# --- objects ---
COMMON_OBJ := $(COMMON_SRC:src%.c=bin/obj%.o)
OBJECTS := $(COMMON_OBJ)

.PHONY: all clean common

all: common

common: $(COMMON_OBJ)

clean:
	rm $(OBJECTS)

bin/obj/common/%.o:src/common/%.c
	$(CC) $(CC_FLAGS) $(COMMON_INCLUDE) -c -o $@ $<
