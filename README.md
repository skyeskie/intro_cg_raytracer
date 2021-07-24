# CS 258 Spring 2011

## Introduction to Computer Graphics

This code is primarily for archival/demonstration purposes. This is the
coursework for CS 258 - Introduction to Computer graphics, which consisted of
five programs that built on each other - all with fairly low-level C/C++
implementation.

While the organization, testing, and documentation are much more rudimentary
than my current work, it demonstrates more technical work than more current
app/website work.

- Project 1: Command line interpreter
- Project 2: TIFF read/write
- Project 3: Image Resizing
- Project 4: 3D Transformations
- Project 5: Ray Tracing

Updated July 2021 to make sure still runs (using Manjaro linux).

Build system switched entirely to CMake

## Building

* Requires OpenGL, GLUT, and X11 libraries (installed so CMake can find)
* Run on Manjaro 21.1.0, didn't verify which packages provided the libraries

1. Run Cmake (ex: from project root `cmake -S . -B build`)
1. Switch to build folder (`cd build`)
1. Compile (`make`)
1. Execute binary (`ipr`)

## Notes on running

- Keep in mind need to make sure the graphics window needs to be selected for
    keyboard input, _not_ the terminal it was launched from.
- Scripts in the `scripts` folder should be run from one level up.
- Several scripts like `test.icf` are *intended* to have an error.

## Supported commands:

Most of this documentation is from well after the period the class. This was for
a class, with no requirements on documentation, to a spec. (And we hadn't even
covered Doxygen-style documentation.) There may be some errors, see the scripts
and/or the specifications if more information is needed. Furthermore, some parts
of the assignments focus on fairly technical implementations. **The assignment
specification at times assumes technical knowledge and is not user-friendly.**
Especially the raytracer and 3D portions are not for the casual user.

### Basic commands

- `read <file>` - reads contents of `file` treating each line as a command
- `pause` - waits for keypress (with graphical window selected) to continue
- `reset` - sets the image array to black
- `resizewin <w> [<h>]` - resizes window to provided dimensions.
    - If omitted, `h` is copied from `w`
- `exit` - exit the program

### TIFF commands

As a convenience, will try to get image in `./images` directory before failing.

- `tiffstat <file>` - Display info about TIFF file
- `tiffread <file>` - Read file to window
- `tiffwrite <file>`- Write window to file
- `iotest <file>` - test read function with some TIFF values filled in

### Image resize commands

- `resize <factor>` - Resizes by specified `factor`.
    - Negative values will flip the image
- `resize <x_factor> <y_factor>` Resize by different factors on `x` and `y` axis
- `scale <factor>` - Resizes by specified factor
- `border <type>` - Changes how the image edge is handled for multipixel resize
        filters
    - `zero` - (default) outside pixels are black
    - `freeze`- pixel matches closest edge pixel
    - `circular` - reflects pixels that cross the edge
    - `truncate` - re-weight the filter to ignore outside pixels
- `select <method> [<filter_width>]` - sets the resize method
    - Methods include `lanczos`, `gaussian`, `mitchell`, `hamming`, `hann`,
        `tent`, `triangle`, `box`
    - Parameter `filter_width` defaults to `0`

### 3D Image Commands

The 3D drawing is performed by maintaining a stack of matrix transformations
(initialized with the identity matrix). All lines drawn are treated as a vector
and multiplied against the top of the transform stack before being drawn.

- `push` - Adds duplicate of topmost item to transformation stack
- `pop` - Manipulates the transform matrix stack
- `translate x y z` - Translate by elementary translation matrix created
    from `(x y z)`
- `scale sx sy sz` - Scale with scaling matrix created from `(sx sy sz)`
- `rotate <angle> ax ay az` - Rotate around axis `(ax, ay, az)` by `angle`
    in degrees
- `ortho l r b t n f` - Implement orthographic projection on the vertices
    - This is assumed viewing along the negative z-axis, displaying from
        `n` (near) to `f` (far).
    - The x-axis bounds are from `l` (left) to `r` (right)
    - The y-axis bounds are from `t` (top) to `b` (bottom)
- `orient <a...i>` - Used to create an arbitrary transformation matrix
```
   | a  b  c  0 |
   | d  e  f  0 |
   | g  h  i  0 |
   | 0  0  0  1 |
```
- `perspective fov a n f` - Implement perspective transformation
    - This is assumed from the origin along the negative z-axis
    - View is described in field of view `fov` and aspect ratio `a`
    - View clipped from `n` (near) to `f` (far) along the negative z-axis
- `lookat fx fy fz ax ay az ux uy uz` - Change point of view
    - Camera location specified by `(fx, fy, fz)`
    - Direction (at) vector specified by `(ax, ay, az)`
    - Up vector specified by `(ux uy uz)`
- `vertex <x> <y> <z>` - Draw line between points
- `printc` - Debug command to show the top of the transformation stack
- `printview` - Debug command to show information about the view
    (from `perspective` or `ortho`)
- `debug3d` - Toggle debug (verbose) mode

### 3D Raytracing Commands

The raytracing part functions by

1. Set up the screen
1. Set up the camera
1. Add object(s)
1. Add light(s)
1. Run `trace` to render the scene

**Colors for the various commands in this module are floats ranging from
`(0.0, 0.0, 0.0)` as black to `(1.0, 1.0, 1.0)` as white.**

The various commands are

- `screen` - Set screen resolution
- `orthocamera` - Setup camera as orthogonal using the screen width and height
- `camera w h Ex Ey Ez Gx Gy Gz Ux Uy Uz dist au av bu bv`
  Setup perspective camera
    - Screen dimenstions `w` by `h`
    - The eye vector `(Ex Ey Ez)` - where the eye is located
    - The gaze vector `(Gx Gy Gz)` - where the eye is looking
    - The up vector `(Ux Uy Uz)` - which direction is up from the eye
    - The screen is from `(au av dist)` to `(bu bv dist)`
- `background r g b` - Sets the background color, shown when the pixel's ray
  doesn't hit anything. Arguments default to `0.0`
- `sphere x y z r Mr Mg Mb Rr Rg Rb [Sr Sg Sb]` - Create a sphere in the scene
    - The sphere is centered on `(x y z)` with radius `r`
    - `(Mr, Mg, Mb)` specifies the ambient color (RGB)
    - `(Rr, Rg, Rb)` specifies the reflective color (RGB)
    - (Optional) `(Sr, Sg, Sb)` specifies the specular color (RGB)
- `triangle Ax Ay Az Bx By Bz Cx Cy Cz Mr Mg Mb Rr Rg Rb [Sr Sg Sb]`
    - The three vertices are `(Ax, Ay, Az)`, `(Bx, By, Bz)`, `(Cx, Cy, Cz)`
    - `(Mr, Mg, Mb)` specifies the ambient color (RGB)
    - `(Rr, Rg, Rb)` specifies the reflective color (RGB)
    - (Optional) `(Sr, Sg, Sb)` specifies the specular color (RGB)
    - The triangles are visible from either side
- `box Ax Ay Az Bx By Bz Mr Mg Mb Rr Rg Rb [Sr Sg Sb]`
    - The box is specified by the two points `(Ax, Ay, Az)` and `(Bx By Bz)`.
      The points are opposite corners, with edges along the axis.
    - `(Mr, Mg, Mb)` specifies the ambient color (RGB)
    - `(Rr, Rg, Rb)` specifies the reflective color (RGB)
    - (Optional) `(Sr, Sg, Sb)` specifies the specular color (RGB)
- `ilight r g b x y z` - Creates a light at infinity with color `(r, g, b)`
  shining in direction `(x, y, z)`
- `clear` - resets all objects, lights, and the viewscreen. Additionally the
  background color is set to black `(0, 0, 0)`
- `trace` - Runs the raytracer on configured objects and puts to the screen

### Dummy commands

**for purposes of having a CLI, will just print out the provided arguments**

Default values provided for move and draw of `(0, 0, 0)`

- `move` x y z
- `draw` x y z
- `color` r g b

## Additional notes

Project submission notes included in `ProjectNotes#.txt` for respective project

Assignment specifications included in `specs` folder. (Project 5 not available)

