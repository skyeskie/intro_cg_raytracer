/**
 * DummyHandle.h
 * @author: Scott Yeskie
 * Class: CS258
 * Project: 1
 * 
 * Description:
 * 		Provides a dummy handler for functions that
 * 			really don't do anything.
 * 		Intended for purposes of Project 1 and testing
 * 
 *		Also is documented framework for new modules
 */

#ifndef DUMMYHANDLE_H
#define DUMMYHANDLE_H
#include <string>
#include "../mod_base/ModuleCommand.h"
#include "../mod_base/CommandHandle.h"

//Extend ModuleCommand so that can register with a command handler
class DummyHandle : public ModuleCommand
{
public:
	//Basic format for modules.
	//Preferred Ctor takes a CommandProcessor and registers commands it takes
	DummyHandle(CommandHandle&);
	DummyHandle(); //Still provide default, though
	
	//Command comes into execute function, then is split out to internal calls.
	//Note: processing done in switch here, since dummy.
    virtual std::string execute(std::vector< std::string > argv);
	
	//Sample help option for module
	virtual void help(const std::string cmd) const;
	virtual void help(int cmd_num) const;
};

#endif // DUMMYHANDLE_H
