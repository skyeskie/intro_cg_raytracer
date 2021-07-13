/**
 * DummyHandle.cpp
 * @author: Scott Yeskie
 * Class: CS258
 * Project: 1
 * 
 * Description:
 *         Implements DummyHandle class
 *         Provides a dummy handler for functions that
 *             really don't do anything.
 *         Intended for purposes of Project 1 and testing
 */


#include "DummyHandle.h"
#include <string>
#include <iostream>
#include <stdexcept>
using namespace std;

enum DUMMY_COMMANDS { DC_Unknown, DC_Move, DC_Draw, DC_Color };

DummyHandle::DummyHandle(CommandHandle& ch): ModuleCommand() {
    //Register functions with handler
    ch.add_command("move", *this);
    ch.add_command("draw", *this);
    ch.add_command("color", *this);
}

DummyHandle::DummyHandle() : ModuleCommand()
{}


string DummyHandle::execute(vector< string > argv)
{
    if(argv.size()==0)
        return "Must provide function, even for dummies.";
    
    //Split off command
    string cmd = argv[0];
    argv.erase( argv.begin() );
    

    
    int cmd_num = DC_Unknown;
    if(cmd=="move") cmd_num = DC_Move;
    if(cmd=="draw") cmd_num = DC_Draw;
    if(cmd=="color") cmd_num = DC_Color;
    
    if(argv.size()==1 && argv[0]=="help") {
        help(cmd_num);
        return "null";
    }
    
    switch( cmd_num ) {
        case DC_Move:
        case DC_Draw:
            //Add default args for Move and Draw
            while(argv.size()>=0 && argv.size()<3) argv.push_back("0");
            
            if(argv.size() != 3)
                throw(invalid_argument("Invalid number of arguments for "+cmd));

            //Sanitize and output            
            cout << "Called "+cmd+"("
                    << sanitize_float(argv[0]) << "," //Take FLOATs
                    << sanitize_float(argv[1]) << ","
                    << sanitize_float(argv[2]) << ")"
                << endl;
            break;
        case DC_Color:
            if(argv.size() != 3)
                throw(invalid_argument("Invalid number of arguments for "+cmd));
    
            cout << "Called "+cmd+"("
                    << sanitize_int(argv[0]) << "," //Take INTs
                    << sanitize_int(argv[1]) << ","
                    << sanitize_int(argv[2]) << ")"
                << endl;
            break;
        case DC_Unknown:
        default:
            throw(runtime_error("Unknown command '"+cmd+"' passed as dummy."));
            break;
    }
    return "null";
}


void DummyHandle::help(const string cmd) const {
    int cmd_num = DC_Unknown;
    if(cmd=="move") cmd_num = DC_Move;
    if(cmd=="draw") cmd_num = DC_Draw;
    if(cmd=="color") cmd_num = DC_Color;
    help(cmd_num);
}

void DummyHandle::help(int cmd_num) const {
    cout <<  "Usage:\t";
    switch( cmd_num ) {
        case DC_Move:
            cout << "move x, y, z";
            break;
        case DC_Draw:
            cout << "draw x, y, z";
            break;
        case DC_Color:
            cout << "color v, h, s";
            break;
    }
    cout << "\n" << endl;
}
