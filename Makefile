SHELL = /bin/sh
prefix = /usr
CC = gcc
C++ = g++
GLUT_LIBS = $(prefix)/lib/libglut.so.3
X_LIBADD =  -lXmu -lXext -lXi -lX11
INCLUDES = -Iinclude -I$(prefix)/include  
LDADD = $(GLUT_LIBS) $(prefix)/lib/libGLU.so.1 $(prefix)/lib/libGL.so  -lm
CFLAGS = -fomit-frame-pointer -ffast-math -D_REENTRANT -O2 -fexpensive-optimizations
CFLAGSD = -g -Wall -D_REENTRANT
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
LINK = $(CC) $(CFLAGS) $(LDFLAGS) -o $@

.SUFFIXES:
.SUFFIXES: .cpp .c .o 

.c.o:
	$(COMPILE) -c $<

.cpp.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c $<

all: main

CLI_OBJECTS=main.o CommandHandle.o DummyHandle.o ModuleCommand.o FunctionCommand.o TiffIO.o IFD_Entry.o ImageResize.o TransformStack.o Matrix3.o Matrix4.o Vector3.o Vector4.o lines.o DrawTransformed.o ILight.o Ray.o RayTracer.o Screen.o ScreenOrtho.o ScreenPersp.o RGBscale.o Sphere.o Triangle.o Box.o

main: main.o mod_base mod_dummy mod_tiff mod_filter mod_3d mod_ray mod_geo
	$(LINK) $(CLI_OBJECTS) $(LDADD) $(LIBS)

mod_base: CommandHandle.o FunctionCommand.o ModuleCommand.o

CommandHandle.o: 
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_base/CommandHandle.cpp -o CommandHandle.o

FunctionCommand.o: ModuleCommand.o
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_base/FunctionCommand.cpp -o FunctionCommand.o

ModuleCommand.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_base/ModuleCommand.cpp -o ModuleCommand.o

mod_dummy: DummyHandle.o

DummyHandle.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_dummy/DummyHandle.cpp -o DummyHandle.o

mod_tiff: TiffIO.o IFD_Entry.o

TiffIO.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_tiff/TiffIO.cpp -o TiffIO.o

IFD_Entry.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_tiff/IFD_Entry.cpp -o IFD_Entry.o

mod_filter: ImageResize.o

ImageResize.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_filter/ImageResize.cpp -o ImageResize.o


mod_matrix: TransformStack.o Vector3.o Vector4.o Matrix3.o Matrix4.o

TransformStack.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_matrix/TransformStack.cpp -o TransformStack.o

Vector3.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_matrix/Vector3.cpp -o Vector3.o

Vector4.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_matrix/Vector4.cpp -o Vector4.o

Matrix3.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_matrix/Matrix3.cpp -o Matrix3.o

Matrix4.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_matrix/Matrix4.cpp -o Matrix4.o

mod_3d: DrawTransformed.o lines.o mod_matrix
DrawTransformed.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_3d/DrawTransformed.cpp -o DrawTransformed.o

lines.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_3d/lines.cpp -o lines.o

mod_ray: ILight.o Ray.o RayTracer.o Screen.o ScreenOrtho.o ScreenPersp.o

ILight.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_ray/ILight.cpp -o ILight.o

Ray.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_ray/Ray.cpp -o Ray.o

RayTracer.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_ray/RayTracer.cpp -o RayTracer.o

Screen.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_ray/Screen.cpp -o Screen.o

ScreenOrtho.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_ray/ScreenOrtho.cpp -o ScreenOrtho.o

ScreenPersp.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_ray/ScreenPersp.cpp -o ScreenPersp.o


mod_geo: RGBscale.o Sphere.o Triangle.o Box.o

RGBscale.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_geo/RGBscale.cpp -o RGBscale.o

Sphere.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_geo/Sphere.cpp -o Sphere.o

Triangle.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_geo/Triangle.cpp -o Triangle.o

Box.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_geo/Box.cpp -o Box.o


#:
#	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c mod_geo/.cpp -o .o


clean:
	-rm -f *.o $(PROGRAMS) *~ mod_*/*~

