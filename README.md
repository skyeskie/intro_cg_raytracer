# CS 258 Spring 2011

## Introduction to Computer Graphics

# Project 1: Command Line Interpreter

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

- `read <file>` reads contents of `file` treating each line as a command
- `exit` - exit the program

### TIFF commands

As a convenience, will try to get image in `./images` directory before failing.

- `tiffstat <file>` - Display info about TIFF file
- `tiffread <file>` - Read file to window
- `tiffwrite <file>`- Write window to file
- `iotest <file>` - test read function with some TIFF values filled in

### Dummy commands

**for purposes of having a CLI, will just print out the provided arguments**

Default values provided for move and draw of `(0, 0, 0)`

- `move` x y z
- `draw` x y z
- `color` r g b

## Additional notes

Project submission notes included in `Project1Notes.txt`

Assignment specification included in `specs` folder

