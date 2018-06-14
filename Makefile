# CSE 374 Makefile exampmle for the talk/speak/shout program.

# The compiler: gcc for C programs
CC = gcc

# Compiler flags:
# -Wall for debugger warnings
# -std=c11 for updated standards
# -g for debugging info
CFLAGS = -Wall -std=c11 -g

# The name of the program that we are producing.
TARGET = bench

# When we just run "make", what gets built? This is a "phony" target
# that just tells make what other targets to build.
all: $(TARGET)

# All the .o files we need for our executable.
OBJS = getmem.o freemem.o get_mem_stats.o print_heap.o mem_utils.o bench.o 

# The executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o bench $(OBJS)

# Individual source files
bench.o: bench.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -c bench.c

getmem.o: getmem.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -c getmem.c

freemem.o: freemem.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -c freemem.c

get_mem_stats.o: get_mem_stats.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -c get_mem_stats.c

print_heap.o: print_heap.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -c print_heap.c

mem_utils.o: mem_utils.c mem_impl.h
	$(CC) $(CFLAGS) -c mem_utils.c

# A "phony" target to remove built files and backups
clean:
	rm -f *.o bench *~

# Run bench with default parameter
test: 
	make
	./bench

# Run bench with test paramter
test1:
	make
	./bench 1000000 50 10 200 20000 

test2:
	make
	./bench 100000 20 10 200 20000 

test3:
	make
	./bench 100000 70 10 200 20000 

test4:
	make
	./bench 100000 50 1 200 20000 

test5:
	make
	./bench 100000 50 30 200 20000 

test6:
	make
	./bench 100000 50 10 10 20000 

test7:
	make
	./bench 100000 50 10 800 20000 

test8:
	make
	./bench 100000 50 10 200 60000 

test9:
	make
	./bench 100000 50 10 200 2000 

