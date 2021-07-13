SHELL = /bin/sh
prefix = /usr
CC = gcc
C++ = g++
GLUT_LIBS = $(prefix)/lib/libglut.so.3
X_LIBADD =  -lXmu -lXext -lXi -lX11
INCLUDES = -Iinclude -I$(prefix)/include  
LDADD = $(GLUT_LIBS) $(prefix)/lib/libGLU.so.1 $(prefix)/lib/libGL.so  -lm
CFLAGS = -g -O2 -Wall -fomit-frame-pointer -ffast-math -fexpensive-optimizations -D_REENTRANT
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
LINK = $(CC) $(CFLAGS) $(LDFLAGS) -o $@

.SUFFIXES:
.SUFFIXES: .cpp .c .o 

.c.o:
	$(COMPILE) -c $<

.cpp.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c $<

all: main

CLI_OBJECTS=main.o CommandHandle.o DummyHandle.o FunctionCommand.o

main: main.o mod_base mod_dummy
	$(LINK) $(CLI_OBJECTS) $(LDADD) $(LIBS)

mod_base: CommandHandle.o FunctionCommand.o

CommandHandle.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_base/CommandHandle.cpp -o CommandHandle.o

FunctionCommand.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_base/FunctionCommand.cpp -o FunctionCommand.o

mod_dummy: DummyHandle.o

DummyHandle.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_dummy/DummyHandle.cpp -o DummyHandle.o

clean:
	-rm -f *.o $(PROGRAMS) *~ mod_*/*~

