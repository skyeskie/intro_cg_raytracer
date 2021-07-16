# CS 258 Spring 2011

## Introduction to Computer Graphics

- Project 1: Command line interpreter
- Project 2: TIFF read/write
- Project 3: Image Resizing
- Project 4: 3D Transformations

Updated July 2021 to make sure still runs (using Manjaro linux).

Build system switched entirely to CMake

## Building

* Requires OpenGL, GLUT, and X11 libraries (installed so CMake can find)
    **Run on Manjaro 21.1.0, didn't verify which packages provided the libraries**

1. Run `cmake -S . -B build`
1. Switch to build folder (`cd build`)
1. Compile with `make`
1. Execute binary `ipr`

## Notes on running

- Keep in mind need to make sure the graphics window needs to be selected for keyboard input, _not_ the terminal it was launched from.
- Scripts in the scripts folder are indended to be run from one level up.
- Several scripts like `test.icf` are *intended* to have an error.

## Supported commands:

### Basic commands

- `read <file>` - reads contents of `file` treating each line as a command
- `pause` - waits for keypress (with graphical window selected) to continue
- `reset` - sets the image array to black
- `resizewin <w> [<h>]` - resizes window to provided dimensions. If omitted, <h> is copied from <w>
- `exit` - exit the program

### TIFF commands

As a convenience, will try to get image in `./images` directory before failing.

- `tiffstat <file>` - Display info about TIFF file
- `tiffread <file>` - Read file to window
- `tiffwrite <file>`- Write window to file
- `iotest <file>` - test read function with some TIFF values filled in

### Image resize commands

- `resize <factor>` - Resizes by specified factor. Negative values will flip the image
- `resize <x_factor> <y_factor>` Resizes by different factors on `x` and `y` axis
- `scale <factor>` - Resizes by specified factor
- `border <type>` - Changes how the border is handled for multipixel resize filters
    - `zero` - (default) outside pixels are black
    - `freeze`- pixel matches closest edge pixel
    - `circular` - reflects pixels that cross the edge
    - `truncate` - reweight the filter to ignore outside pixels
- `select <method> [<filter_width>]` - sets the resize method. Parameter filter width defaults to 0
    - "lanczos"
    - "gaussian"
    - "mitchell"
    - "hamming"
    - "hann"
    - "tent"
    - "triangle"
    - "box"

### 3D Image Commands

The 3D drawing is performed by mantaining a stack of matrix transformations (initialized with the identity matrix). All lines drawn are treated as a vector and multiplied against the top of the transform stack before being drawn. See the specifications for more details on the various commands. These also contain a fair amount of linear algebra and are not intended to be accessible without some grounding in low-level 3D graphics.

- `push` - Adds duplicate of topmost item to transformation stack
- `pop` - Manipulates the transform matrix stack
- `translate <x> <y> <z>` - Translate by elementary translation matrix
- `scale <sx> <sy> <sz>` - Scale with scaling matrix
- `rotate <angle> <ax> <ay> <az>` - Rotate around axis `(ax, ay, az)` by angle (in degrees)
- `ortho <l> <r> <b> <t> <n> <f>` - Implement orthographic projection on the verticies
    - This is assumed viewing along the negative z-axis, displaying from `n` (near) to `f` (far).
    - The x-axis is bounded by `l` (left) and `r` (right)
    - The y-axis is bounded by `t` (top) and `b` (bottom)
- `orient <a...i>` - Used to create an arbitrary transformation matrix
```
   | a  b  c  0 |
   | d  e  f  0 |
   | g  h  i  0 |
   | 0  0  0  1 |
```
- `perspective <f1> <a> <n> <f2>` - Implement perspective transformation
    - This is assumed from the origin along the negative z-axis
    - View is described in field of view `f1` and aspect ratio `a`
    - The view is clipped from `n` (near) to `f` (far) along the negative z-axis
- `lookat <fx> <fy> <fz> <ax> <ay> <az> <ux> <uy> <uz>` - Change point of view
- `vertex <x> <y> <z>` - Draw line between points
- `printc` - Debug command to show the top of the transformation stack
- `printview` - Debug command to show information about the view (from `perspective` or `ortho`)
- `debug3d` - Toggle debug (verbose) mode

### Dummy commands

**for purposes of having a CLI, will just print out the provided arguments**

Default values provided for move and draw of `(0, 0, 0)`

- `move` x y z
- `draw` x y z
- `color` r g b

## Additional notes

Project submission notes included in `ProjectNotes#.txt` for respective project

Assignment specifications included in `specs` folder

