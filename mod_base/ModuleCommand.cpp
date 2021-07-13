#include "ModuleCommand.h"
#include <string>
#include <stdexcept>
#include <cmath>
using namespace std;

float ModuleCommand::sanitize_float(string str) {
    // 

    //Flags
    bool can_neg = true;
    bool neg_num = false;
    bool end_space = false;
    bool can_dec = true;
    int divide = 0;

    //Tests calculated each loop
    bool is_space = false;
    bool is_num = false;
    bool in_num = false;

    //Accumulator
    int num_tmp = 0;
    int numer = 0;
    int cur_num = 0;

    for(string::iterator it = str.begin(); it!=str.end(); it++) {
        if(divide) ++divide;
        is_space = (*it==' ' || *it=='\t'|| *it=='\r'|| *it=='\n');
        num_tmp = *it - '0';
        is_num = (num_tmp>=0 && num_tmp<10);

        if(is_space) {
            //Ignore spaces
            //Set end_space flag if we were in a number
            end_space = in_num;
            in_num = false;
        } else if(end_space) {
            throw invalid_argument(string("Unexpected character in float.\n")
                + "  In <end_space> encountered '"+*it+"'");
        } else if(*it=='-') { // - for negative numbers
                if(!can_neg) {
                    throw invalid_argument("Invalid use of '-' in float.");
                }
                can_neg = false;
                neg_num = true;
        } else if(is_num) {
            in_num = true;
            can_neg = false;
            cur_num*=10;
            cur_num+=num_tmp;
        } else if(*it=='.') { //For decimal
            can_dec = false;
            can_neg = false;
            divide = 1;
            numer = cur_num;
            cur_num = 0;
        } else {
            throw invalid_argument(string("Unexpected character '")+*it+"'");
        }
    }

    float res = cur_num;
    if(divide) {
        res /= pow(10, divide-1);
    }

    res += (float) numer;
    if(neg_num) res *= -1;

    return res;
}

int ModuleCommand::sanitize_int(string str) {
    //Flags
    bool can_neg = true;
    bool neg_num = false;
    bool end_space = false;

    //Tests calculated each loop
    bool is_space = false;
    bool is_num = false;
    bool in_num = false;

    //Accumulator
    int num_tmp = 0;
    int cur_num = 0;

    char ch;
    
    for(int i=0; i<(int) str.length(); i++) {
        ch = str.at(i);
        is_space = (ch==' ' || ch=='\t'|| ch=='\r'|| ch=='\n');
        num_tmp = ch - '0';
        is_num = (num_tmp>=0 && num_tmp<10);
        //cout << ch << " -- " << is_space << "|" << is_num << "+" << num_tmp;
        if(is_space) {
            //Ignore spaces
            //Set end_space flag if we were in a number
            end_space = in_num;
            in_num = false;
        } else if(end_space) {
            throw invalid_argument(string("Unexpected character in int.\n")
            + "  In <end_space> encountered '"+ch+"'");
        } else if(ch=='-') { // - for negative numbers
            if(!can_neg) {
                throw invalid_argument("Invalid use of '-' in int.");
            }
            can_neg = false;
            neg_num = true;
        } else if(is_num) {
            in_num = true;
            can_neg = false;
            cur_num*=10;
            cur_num+=num_tmp;
        } else {
            throw invalid_argument(string("Unexpected character '")+ch+"'");
        }
        //cout << ":=" << cur_num << endl;
    }

    return ((neg_num)? cur_num*-1 : cur_num);
}

void ModuleCommand::check_args(size_t n, vector< string >& argv, string fill)
{
    check_args(n, n, argv, fill);
}

void ModuleCommand::check_args(size_t lo, size_t hi, vector< string >& argv, string fill)
{
    if(argv.size()<lo || argv.size()>hi) {
        throw invalid_argument("Invalid number of arguments.");
    }
    while(fill!="{NULL}", argv.size() < hi) {
        argv.push_back(fill);
    }
}
