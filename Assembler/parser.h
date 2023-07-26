#pragma once

#include <bitset>
#include <cstdint>   
#include <map>
#include <stdexcept>  
#include <string>   
#include "symbolTable.h" 

using namespace std;

extern symbolTable symTab;

class parser{

    private:
    map<string, string> compDictionary;
    map<string, string> destDictionary;
    map<string, string> jumpDictionary;
    string prefixA = "0";
    string prefixC = "111";

    void compEntry(string key, string val);
    void destEntry(string key, string val);
    void jumpEntry(string key, string val);
    string compLookup(string instComp);
    string destLookup(string instDest);
    string jumpLookup(string instJump);
    string parseAInst(string inst);
    string parseCInst(string inst);

    public:
    parser();
    ~parser();
    string parseInst(string inst);
};

