/**
 * CommandHandle.h
 * @author: Scott Yeskie
 * Class: CS258
 * Project: 1
 * 
 * Description:
 *         Provides a handler to interpret commands
 *         and callback to the appropriate object
 * 
 *         Also implements basic commands READ and EXIT
 */

#ifndef COMMANDHANDLE_H
#define COMMANDHANDLE_H

#include <GL/glut.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <stack>
#include <fstream>
#include <iostream>
#include <memory>
#include "ModuleCommand.h"
#include "FunctionCommand.h"

#define MAX_IMG_WIDTH 1024
#define MAX_IMG_HEIGHT 1024


typedef ModuleCommand MC;

class CommandHandle {
public:
    //Ctor
    CommandHandle();
    
    /**
     * add_command()
     * @arg (char*) OR (string) text of command
     * @arg (MC&) ModuleCommand object to callback to
     * @post command/module mapping added to handler
     */
    void add_command(const char*, MC&);
    void add_command(const std::string, MC&);
    
    /**
     * add_command()
     * @arg (vector<string>) list of aliases for command
     * @arg (MC&) ModuleCommand object to callback to
     * @post all aliases mapped to MC object
     */
    void add_command(std::vector<std::string>, MC&);
    
    /**
     * execute()
     * @arg (char*) OR (string) line of code to interpret
     * @post Interprets function uses map to call exeute() on callback object
     * @return (string) result from callback function
     *         this is "null" if no text is intended to be output
     *         this is "exit" if the program is supposed to exit
     */
    std::string execute(char* line);
    std::string execute(std::string line);
    
    /**
     * help()
     * @arg (char*) OR (string) -- command to get help on
     * @post command help written to output (if exists)
     */
    void help(char* cmd);
    void help(std::string cmd);

    //Calls help for the last executed command
    void help();
    
    /**
     * read_file()
     *         opens and reads file line by line.
     *         Cuts off each line after '#' for commenting purposes
     *         feeds each line to execute()
     * @arg (vector<string>) [0] -- ignored. assumed "read"
     *                          [1] -- filename (case sensitive)
     *         (optional flag)     [2] "-v". if present runs in verbose mode
     *
     * verbose mode: outputs all commands and results
     * normal mode: output only if functions output
     */
    std::string read_file(std::vector<std::string>);

    /**
     * read_loop()
     *          main loop for reading file
     * @pre open $fp on file_stack
     */
    std::string read_loop();

    GLubyte main_window[MAX_IMG_HEIGHT][MAX_IMG_WIDTH][3];

    size_t window_width;
    size_t window_height;
    
    /**
     * str_to_lower()
     * @arg string to convert to lower case
     * @post all characters in string are lower case
     */
    void str_to_lower(std::string&);

    ///Flag to indicate should reenter read
    bool wait_ack;

private:    
    /**
     * split_cmd()
     * @arg line of code to split
     * @returns vector<string> of each entity in line
     *
     * line is broken up by whitespace and commas
     */
    std::vector<std::string> split_cmd(const char* line) const;
    
    //Used to store associations between commands and callback objects
    std::map<std::string, MC*> list;

    //Used to get syntax on last command entered
    std::string last_exec;

    struct file_record {
        file_record(std::string n, std::ifstream* p, bool v=0)
                :fname(n), fp(p), verbose(v)
        {
        };

       // ~file_record();

        file_record(const file_record& rhs)
            :fname(rhs.fname), fp(), verbose(rhs.verbose)
        {};
        
        std::string fname;
        std::unique_ptr<std::ifstream> fp;
        bool verbose;

        //std::ifstream fh;
    };

    void add_fp_to_stack(std::string n, std::ifstream* p, bool v=0);
    
    //File stack -- actual stack used for reentry
    std::stack<file_record> file_stack;

    //Keep track for easy check on recursion
    std::set<std::string> read_stack;
};

#endif // COMMANDHANDLE_H
