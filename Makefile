#
# This is an example Makefile for a countwords program.  This
# program uses both the scanner module and a counter module.
# Typing 'make' or 'make count' will create the executable file.
#

# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CC = g++ -std=c++14
CFLAGS  = -g -Wall

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: recom

# To create the executable file count we need the object files
# countwords.o, counter.o, and scanner.o:
#
recom:  main.o hash_func.o Hash_table.o utilities.o cluster.o 
	$(CC) $(CFLAGS) -o recom main.o hash_func.o Hash_table.o utilities.o cluster.o

# To create the object file countwords.o, we need the source
# files countwords.c, scanner.h, and counter.h:
#
main.o:  hash_func.cpp Hash_table.cpp Value.h Key.h constants.h 
	$(CC) $(CFLAGS) -c main.cpp

# To create the object file counter.o, we need the source files
# counter.c and counter.h:
#
Hash_table.o:  Hash_table.cpp Hash_table.h 
	$(CC) $(CFLAGS) -c Hash_table.cpp

cluster.o:  cluster.cpp cluster.h 
	$(CC) $(CFLAGS) -c cluster.cpp

# To create the object file scanner.o, we need the source files
# scanner.c and scanner.h:
#
hash_func.o:  hash_func.cpp hash_func.h 
	$(CC) $(CFLAGS) -c hash_func.cpp

utilities.o:  utilities.cpp utilities.h 
	$(CC) $(CFLAGS) -c utilities.cpp
# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) count *.o *~
