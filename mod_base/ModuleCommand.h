#ifndef MODULECOMMAND_H
#define MODULECOMMAND_H

#include <string>
#include <vector>
#include <iostream>


class ModuleCommand
{
public:
	ModuleCommand() {}; //Nothing to init
	
	virtual std::string execute(std::vector<std::string> argv) = 0;
	
	virtual void  help(const char* cmd) const {
		help( std::string(cmd) );
	}
	virtual void help(const std::string cmd) const { 
		std::cout << "No help topic for '"+cmd+"'." << std::endl;
	}
};

#endif // MODULECOMMAND_H
