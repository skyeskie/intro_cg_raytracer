SHELL = /bin/sh
prefix = /usr
CC = gcc
GLUT_LIBS = $(prefix)/lib/libglut.so.3
X_LIBADD =  -lXmu -lXext -lXi -lX11
INCLUDES = -Iinclude -I$(prefix)/include  
LDADD = $(GLUT_LIBS) $(prefix)/lib/libGLU.so.1 $(prefix)/lib/libGL.so.1 -lm
CFLAGS = -g -O2 -Wall -fomit-frame-pointer -ffast-math -fexpensive-optimizations -D_REENTRANT
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
LINK = $(CC) $(CFLAGS) $(LDFLAGS) -o $@

.SUFFIXES:
.SUFFIXES: .c .o 

.c.o:
	$(COMPILE) -c $<

all: main

CLI_OBJECTS=main.o 

main: $(CLI_OBJECTS)
	$(LINK) $(CLI_OBJECTS) $(LDADD) $(LIBS)

clean:
	-rm -f *.o $(PROGRAMS)

