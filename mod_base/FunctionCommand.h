/**
 * FunctionCommand.h
 * @author: Scott Yeskie
 * Class: CS258
 * Project: 1 (though not actually used in project 1 main)
 * 
 * Description:
 * 		Wrapper to add functions as callbacks to CommandHelper
 * 		Does NOT work with methods currently
 */

#ifndef FUNCTIONCOMMAND_H
#define FUNCTIONCOMMAND_H
#include "ModuleCommand.h"
#include <string>
#include <vector>

typedef std::string (*FP)(std::vector<std::string>);

class FunctionCommand : public ModuleCommand {
public:
	FunctionCommand(FP fp);
	
	//Implements required execute()
	std::string execute(std::vector<std::string> argv);
	
private:
	//Store function pointer for callback
	FP function;
};

#endif // FUNCTIONCOMMAND_H
