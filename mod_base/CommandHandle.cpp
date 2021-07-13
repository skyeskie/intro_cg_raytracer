/** 
 * CommandHandle.cpp
 * @author: Scott Yeskie
 * Class: CS258
 * Project: 1
 * 
 * Description:
 *		Implementation of CommandHandle class
 * 		Provides a handler to interpret commands
 * 		and callback to the appropriate object
*/


#include <string>
#include "CommandHandle.h"
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <stdexcept>
using namespace std;

CommandHandle::CommandHandle()
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
string CommandHandle::execute(string &line) {
	vector<string> argv;
	argv = split_cmd(line.c_str());
	if(!argv.size()) {
		if(line.length()==0) return "null";
		else throw(runtime_error("Unable to split command."));
	}
	
	str_to_lower(argv[0]);
	
	string cmd(argv.front());
	//Uncomment to separate command from args
	//argv.erase(argv.begin()); 
	
	
	//Hardcode basic calls
	if(cmd=="read")
		return read_file(argv);
	if(cmd=="exit" || cmd=="exitFS" || cmd=="close")
		return cmd; //"exit" is exit signal
	
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

void CommandHandle::help(std::string cmd) {
	//See if command exists
	if(cmd=="read") {
		cout << "Usage:\tread Case_SensitiveFile\\ with_space.ext [-v]" << endl;
		cout << "\t\t-v is optional. Outputs each command and its results in file.\n" << endl;
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
		throw(invalid_argument("Invalid number of arguments for read"));
	}
	
	bool verbose = false;
	if(file.size()==3) {
		if(file[2]=="-v") verbose = true;
	}
	
	
	ifstream fp;
	fp.open(file[1].c_str());
	if(!fp.is_open()) {
		throw(runtime_error("Error opening file '"+file[1]+"'."));
		return "null";
	}
	
	string line, cmd, result;
	//Debug
	if(verbose) cout << "Reading file..." << endl;
	while( getline(fp, line) ) {
		cmd = line.substr( 0, line.find("#"));
		if(cmd.size()!=0) { //Ignore # / blank lines
			if(verbose) cout<< "{" << cmd << " : ";
			try { 
				result = execute(cmd);
			}
			catch(runtime_error excpt) {
				cerr << "Error in file '"<<file[1]<<"':" << endl;
				cerr << "\t" << excpt.what() << endl;
				cerr << "\tExiting all read operations.\n" << endl;
				result = "exitFS";
			}
			if(verbose) cout<< result << "}" << endl;
			//If command is exit, exit program
			//If command is exitFS, exit all read commands
			//If command is close, only exit this read function
			if(result=="exit" || result=="exitFS" || result=="close") {
				fp.close();
				return (result=="close")?"null":result;
			}
			if(result=="close") {
				fp.close();
				return "null";
			}
		}
	}
	fp.close();
	return "null";
}


//Convert string to lower case
void CommandHandle::str_to_lower(string& txt) {
	for(unsigned int i=0; i<txt.length(); i++) {
		txt[i] = tolower(txt[i]);
	}
}

