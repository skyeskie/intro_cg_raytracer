/** 
 * CommandHandle.cpp
 * @author: Scott Yeskie
 * Class: CS258
 * Project: 1
 * 
 * Description:
 *        Implementation of CommandHandle class
 *         Provides a handler to interpret commands
 *         and callback to the appropriate object
*/


#include <string>
#include "CommandHandle.h"
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <stdexcept>
using namespace std;

CommandHandle::CommandHandle()
    :window_width(250), window_height(250), wait_ack(false)
    //:main_window(mainWindow)
{} //Nothing to do, except if we decide to make Singleton

typedef ModuleCommand MC;

//Associates a command with a handler (char* version)
void CommandHandle::add_command(const char name[], MC& callback) {
    string str(name);
    list[str] = &callback;
}

//Associates a command with a handler (string version)
void CommandHandle::add_command(const string name, MC& callback) {
    list[name] = &callback;
}

//Associates multiple commands with a handler
void CommandHandle::add_command(vector< string > names, MC& callback) {
    vector<string>::iterator it;
    for(it = names.begin(); it!=names.end(); it++) {
        list[*it] = &callback;
    }
}

//Buffer: converts line* to string for execute
string CommandHandle::execute(char* line) {
    string str(line);
    return execute(str);
}

//Takes a line, splits command from params, and executes
string CommandHandle::execute(string line) {
    vector<string> argv;
    argv = split_cmd(line.c_str());
    if(!argv.size()) {
        if(line.length()==0) return "null";
        else throw(runtime_error("Unable to split command."));
    }
    
    str_to_lower(argv[0]);
    
    string cmd(argv.front());
    last_exec = cmd;
    //Uncomment to separate command from args
    //argv.erase(argv.begin());
    
    
    //Hardcode basic calls
    if(cmd=="read")
        return read_file(argv);
    
    //Commands that should propogate through
    if(cmd=="exit" || cmd=="exitFS" || cmd=="close" || cmd=="quit" || cmd=="pause")
        return cmd; //"exit" is exit signal
    
    if(cmd=="wait") {
        cout << "Press enter to continue. (Cmd line must be in focus).";
        char ch;
        ch = cin.get();
        return "null";
    }
    
    //See if command exists
    if(list.find(cmd)==list.end()) {
        throw(runtime_error("Command '"+cmd+"' does not exist."));
    }
    
    //Now demap and execute command
    return list[cmd]->execute(argv);
}

void CommandHandle::help(char* line) {
    vector<string> argv;
    argv = split_cmd(line);
    if(!argv.size()) {
        return;
    }
    
    str_to_lower(argv[0]);
    
    string cmd(argv.front());
    help(cmd);
}

void CommandHandle::help() {
    help(last_exec);
}

void CommandHandle::help(std::string cmd) {
    //See if command exists
    if(cmd=="read") {
        cout << "Usage:\tread Case_SensitiveFile\\ with_space.ext [-v]" << endl;
        cout << "\t\t-v is optional. Outputs each command "
             << "and its results in file.\n" << endl;
        return;
    }
    if(list.find(cmd)==list.end())
        cout << "No help topic for '"+cmd+"'." << std::endl;
    else
        list[cmd]->help(cmd);
}


//Split a command into a vector of strings
vector< string > CommandHandle::split_cmd(const char* line) const {
    int i=-1;
    string cur = "";
    vector<string> out;
    bool is_space = false,
         is_comma = false,
         in_escape = false;
    
    while( line[++i] != '\0' ) {
        is_space = (line[i]==' ' || line[i]=='\t'|| line[i]=='\r'|| line[i]=='\n');
        is_comma = (line[i]==',');
    
        //Word break
        if( !in_escape && (is_space ||  is_comma) ) {
            //Do RTrim here
            if(cur.length()) {
                out.push_back(cur);
                cur = "";
            }
        } else if(line[i]=='\\') {
            in_escape = true;
        } else{
            //Add to cmd. Performs LTrim
            cur += line[i];
            in_escape = false;
        }
    }
    //Now, if string in cur, add to output
    //Do RTri
    if(cur.length()) {
        out.push_back(cur);
    }
    
    return out;
}

string CommandHandle::read_file(vector<string> file) {
    //Takes one parameter: file name
    if(file.size()<2) {//Param 0 is "read"
        read_stack.clear();
        throw(invalid_argument("Invalid number of arguments for read"));
    }
    
    bool verbose = false;
    if(file.size()==3) {
        if(file[2]=="-v") verbose = true;
    }
    
    //Make sure we're not in some sort of recursive call
    if(read_stack.count(file[1])) {
        read_stack.clear();
        throw runtime_error("Tried to recursively read files.");
    }
    read_stack.insert(file[1]);

    //file_record cur_file(file[1], verbose);
    ifstream* fp = new ifstream;
    fp->open(file[1].c_str());
    
    //Try to prepend scripts/ to file name
    if(!fp->is_open()) {        
        string tmp( "scripts/" );
        tmp.append(file[1]);

        fp->open(tmp.c_str());
    }

    //Try to prepend scripts/p4 to file name
    if(!fp->is_open()) {
        string tmp( "scripts/p4/" );
        tmp.append(file[1]);

        fp->open(tmp.c_str());
    }

    if(!fp->is_open()) {
        while(! file_stack.empty() )
            file_stack.pop();
        read_stack.clear();
        throw(runtime_error("Error opening file '"+file[1]+"'."));
        return "null";
    }
        
    if(verbose) cout << "Reading file..." << endl;
    add_fp_to_stack(file[1], fp, verbose);

    return read_loop();
}

string CommandHandle::read_loop()
{
    if(file_stack.empty()) {
        cout << "Finished reading files." << endl;
        return "null";
    }
    
    file_record f = file_stack.top();
    ifstream* fp = file_stack.top().fp.get();
    ifstream& fh = *fp;
    bool verbose = f.verbose;
    string line, cmd, result;
    //Debug
    
    while( getline(fh, line) ) {
        cmd = line.substr( 0, line.find("#"));
        if(cmd.size()!=0) { //Ignore # / blank lines
            if(verbose) cout<< "{" << cmd << " : ";
            try {
                result = execute(cmd);
            }
            catch(runtime_error &excpt) {
                cerr << "Error in file '"<<f.fname<<"':" << endl;
                cerr << "\t" << excpt.what() << endl;
                cerr << "\tExiting all read operations.\n" << endl;
                result = "exitFS";
            }
            if(verbose) cout<< result << "}" << endl;
            //If command is exit, exit program
            if(result=="exit") {
                printf("Exiting...\n");
                exit(0);
            }

            //Propogate through a pause
            if(result=="pause") {
                wait_ack = true;
                return "pause";
            }
            
            //If command is exitFS, exit all read commands
            //If command is close, only exit this read function
            if(result=="exitFS") {
                read_stack.clear();
                while(! file_stack.empty() )
                    file_stack.pop();
                return "null";
            }
            if(result=="close") {
                read_stack.erase(f.fname);
                file_stack.pop();
                return "null";
            }
            if(!verbose && result!="null") cout << result << endl;
        }
    }
    file_stack.pop();
    read_stack.erase(f.fname);
    return "null";
}

void CommandHandle::add_fp_to_stack(string n, ifstream* p, bool v) {
    file_stack.push(file_record(n,NULL,v));
    file_stack.top().fp.reset(p);
}


//Convert string to lower case
void CommandHandle::str_to_lower(string& txt) {
    for(unsigned int i=0; i<txt.length(); i++) {
        txt[i] = tolower(txt[i]);
    }
}
