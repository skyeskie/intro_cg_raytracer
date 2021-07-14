/**
 * IFD_Entry.h
 * @author: Scott Yeskie
 * Class: CS258
 * Project: 2
 *
 * Description:
 *         Defines an object to contain all the information for an IFD entry
 *              from a TIFF file
 *         Provides comparison operators (which sort on identifier)
 *              and an insertion operator
 *         No data hiding since it's limited use
 */
#ifndef IFD_ENTRY_H
#define IFD_ENTRY_H
#include <string>
#include <vector>
#include <iostream>

class IFD_Entry
{
public:
    friend std::ostream& operator<<(std::ostream&, IFD_Entry&);
    IFD_Entry();
    IFD_Entry(int type);
    
    int identifier;
    
    //Operators
    bool operator<(IFD_Entry&);
    bool operator>(IFD_Entry&);
    
    //Gives name of this tag
    std::string get_name();
    //Gives name of tag <id>
    std::string get_name(int id);

    //Type
    int type;
   
    int address;

    int size;
    
    std::vector<int> intdata; //Short, Long, Byte stored here
    std::vector<int> intdata2; //Rational Denom stored here
    std::string chardata; //ASCII goes here
};

std::ostream& operator<<(std::ostream& out, IFD_Entry& ifde);

#endif // IFD_ENTRY_H
