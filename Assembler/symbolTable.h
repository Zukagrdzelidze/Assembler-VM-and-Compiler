#pragma once


#include <cstdint>   
#include <map>
#include <stdexcept>  
#include <string>    

using namespace std;

class symbolTable {
private:
    map<string, uint16_t> table;
    int varCounter;
    int lblCounter;
public:
    symbolTable();
    ~symbolTable();
    int incLabelCounter();
    void addLabel(string label);
    void addVariable(string var);
    uint16_t lookupSymbol(string var);
};

