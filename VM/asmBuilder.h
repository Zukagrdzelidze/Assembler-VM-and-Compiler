#pragma once

#include <bitset>
#include <cstdint>
#include <functional>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

extern string name;

class asmBuilder
{
public:
    asmBuilder();
    ~asmBuilder();

    string addFunction();
    string subFunction();
    string negFunction();
    string eqFunction();
    string gtFunction();
    string ltFunction();
    string andFunction();
    string orFunction();
    string notFunction();
    string pushFunction(string segment, int offset);
    string popFunction(string segment, int offset);

    string labelFunction(string label);
    string gotoFunction(string label);
    string if_gotoFunction(string label);
    string function(string function_name, int n_vars);
    string callFunction(string function_name, int n_args);
    string returnFunction();

    
    string endFunction();

private: 
    void write(string vmCode);
    string registerName(string segment, int index);
    int symbolCounter; 
    enum segments
    {
        seg_constant,
        seg_static,
        seg_temp,
        seg_pointer,
        seg_local,
        seg_argument,
        seg_this,
        seg_that
    };
    map<string, segments> map_segments;
    stringstream asmStr;
};