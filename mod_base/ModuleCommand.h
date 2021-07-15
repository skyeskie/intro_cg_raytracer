#ifndef MODULECOMMAND_H
#define MODULECOMMAND_H

#include <string>
#include <vector>
#include <iostream>


class ModuleCommand
{
public:
    ModuleCommand() {}; //Nothing to init

    /**
     * virtual execute()
     * this is where the CommandHandler calls in
     */
    virtual std::string execute(std::vector<std::string> argv) = 0;

    /**
     * converts char* to string so only one command needed
     * by children classes for <help()>
     */
    virtual void  help(const char* cmd) const {
        help( std::string(cmd) );
    }

    /**
     * Lookup help for particular function
     * Can be used to supply help for functions
     */
    virtual void help(const std::string cmd) const {
        std::cout << "No help topic for '"+cmd+"'." << std::endl;
    }

    /**
     * Input sanitization function (float version)
     * @throws invalid_argument on fail
     * @returns <str> converted to a float
     * equivalent to RegEx ^wht*(-)?wht*([0-9]+\.?[0-9]*)wht*$
     * where (wht) is short for [ \t\r\n]
     */
    float sanitize_float(std::string str);
    
    /**
     * Input sanitization function (int version)
     * @throws invalid_argument on fail
     * @returns <str> converted to an int
     * equivalent to RegEx ^wht*(-)?wht*([0-9]+)wht*$
     * where (wht) is short for [ \t\r\n] (whitespace)
     */
    int sanitize_int(std::string str);

protected:
    //Use to make sure right number of args
    void check_args(size_t, std::vector<std::string>&,
                    std::string fill = "{NULL}");
    void check_args(size_t, size_t, std::vector<std::string>&,
                    std::string fill = "{NULL}");
};

#endif // MODULECOMMAND_H
