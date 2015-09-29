CC=gcc

LIB_SRC=$(PWD)/lib/*.c
OUTPUT_DIR=$(PWD)/bin

.PHONY: all memalloc memalloc-exec

all:
	memalloc

memalloc:
	$(output_dir)
	$(CC) -o $(PWD)/bin/free_and_sbrk $(LIB_SRC) memalloc/free_and_sbrk.c

memalloc-exec:
	$(output_dir)
	$(CC) -o $(PWD)/bin/free_and_sbrk-exec $(LIB_SRC) memalloc/exec/exec-7-1.c
	$(CC) -o $(PWD)/bin/program_segments $(LIB_SRC) memalloc/exec/prog-seg.c
	$(CC) -c memalloc/exec/memalloc.c


output_dir:
	@rm -rf $(OUTPUT_DIR)
	@mkdir -p $(OUTPUT_DIR)


