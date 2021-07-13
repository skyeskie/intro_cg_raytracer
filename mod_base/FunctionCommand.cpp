/**
 * FunctionCommand.h
 * @author: Scott Yeskie
 * Class: CS258
 * Project: 1
 * 
 * Description:
 * 		Wrapper to add functions as callbacks to CommandHelper
 * 		Does NOT work with methods currently
 */

#include "FunctionCommand.h"
#include <string>
#include <vector>

typedef std::string (*FP)(std::vector<std::string>);

FunctionCommand::FunctionCommand(FP fnp)
	:function(fnp)
{}

std::string FunctionCommand::execute(std::vector<std::string> argv) {
	//Strip command name before passing args
	argv.erase( argv.begin() );
	return (*function)(argv);
}