# CS 258 Spring 2011

## Introduction to Computer Graphics

# Project 1: Command Line Interpreter

Updated July 2021 to make sure still runs (using Manjaro linux).

Build system switched entirely to CMake

## Building

* Requires OpenGL, GLUT, and X11 libraries (installed so CMake can find)
    **Run on Manjaro 21.1.0, didn't verify which packages provided the libraries**

1. Run `cmake`
1. Run `make`
1. Execute binary `assign1`

## Notes on running

- Keep in mind need to make sure the graphics window needs to be selected for keyboard input, _not_ the terminal it was launched from.
- To run scripts from the `scripts` directory, the binary needs to be started from there (eg `cd scripts && ../assign1`)
- The `test.icf` scripts are *intended* to have an error.

## Supported commands:

### Basic commands

- `read <file>` reads contents of `file` treating each line as a command
- `exit` - exit the program

### Dummy commands

**for purposes of having a CLI, will just print out the provided arguments**

Default values provided for move and draw of `(0, 0, 0)`

- `move` x y z
- `draw` x y z
- `color` r g b

## Additional notes

Project submission notes included in `Project1Notes.txt`

Assignment specification included in `specs` folder

