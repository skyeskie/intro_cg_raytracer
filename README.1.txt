CS 258 Spring 2011
Project 1: Command Line Interpreter
Author: Scott Yeskie

Comments:
    I have Sabayon (Gentoo based) linux distro, so I did not test it on Knoppix.
    
    The Makefile did not like using subdirectories, so I had to modify it.
    I'm not quite sure if I did it correctly, but it does compile for me with just 'make'
    from a clean base.
    
    Also, I included a CmakeLists.txt file in main, since I used KDevelop (which likes cmake).
    It compiled fine with Cmake, which should be more portable than the Makefile if any
    problems arise compiling that.
    
    I included two test files test.icf and test2.icf in the root directory for use
    with the READ command.    The provided test file scripts/test.cli works only if it
    is run with scripts as the current working directory.
    
    Type sanitization (eg. int or float) is not implemented yet, as it was not in the project 1 spec.
    
    Typing "command help" will print out the syntax for the command (if it exists).