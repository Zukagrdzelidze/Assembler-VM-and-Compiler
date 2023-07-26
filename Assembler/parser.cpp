#include "parser.h"

parser::parser(){
    compDictionary.clear();
    destDictionary.clear();
    jumpDictionary.clear();

    
    compEntry("0", "0101010");
    compEntry("1", "0111111");
    compEntry("-1", "0111010");
    compEntry("D", "0001100");
    compEntry("A", "0110000");
    compEntry("!D", "0001101");
    compEntry("!A", "0110001");
    compEntry("-D", "0001111");
    compEntry("-A", "0110011");
    compEntry("D+1", "0011111");
    compEntry("1+D", "0011111");  
    compEntry("A+1", "0110111");
    compEntry("1+A", "0110111");  
    compEntry("D-1", "0001110");
    compEntry("A-1", "0110010");
    compEntry("D+A", "0000010");
    compEntry("A+D", "0000010");  
    compEntry("D-A", "0010011");
    compEntry("A-D", "0000111");
    compEntry("D&A", "0000000");
    compEntry("A&D", "0000000");  
    compEntry("D|A", "0010101");
    compEntry("A|D", "0010101"); 
    compEntry("M", "1110000");
    compEntry("!M", "1110001");
    compEntry("-M", "1110011");
    compEntry("M+1", "1110111");
    compEntry("1+M", "1110111");
    compEntry("M-1", "1110010");
    compEntry("D+M", "1000010");
    compEntry("M+D", "1000010");
    compEntry("D-M", "1010011");
    compEntry("M-D", "1000111");
    compEntry("D&M", "1000000");
    compEntry("M&D", "1000000");  
    compEntry("D|M", "1010101");
    compEntry("M|D", "1010101");  

    destEntry("null", "000");
    destEntry("M", "001");
    destEntry("D", "010");
    destEntry("MD", "011");
    destEntry("DM", "011");  
    destEntry("A", "100");
    destEntry("AM", "101");
    destEntry("MA", "101");  
    destEntry("AD", "110");
    destEntry("DA", "110");   
    destEntry("AMD", "111");  
    destEntry("ADM", "111");
    destEntry("MAD", "111");
    destEntry("MDA", "111");
    destEntry("DAM", "111");
    destEntry("DMA", "111");

    jumpEntry("null", "000");
    jumpEntry("JGT", "001");
    jumpEntry("JEQ", "010");
    jumpEntry("JGE", "011");
    jumpEntry("JLT", "100");
    jumpEntry("JNE", "101");
    jumpEntry("JLE", "110");
    jumpEntry("JMP", "111");
}

parser::~parser(){
    compDictionary.clear();
    destDictionary.clear();
    jumpDictionary.clear();
}

void parser::compEntry(string key, string val) {
    compDictionary.insert(pair<string, string>(key, val));
}
void parser::destEntry(string key, string val) {
    destDictionary.insert(pair<string, string>(key, val));
}
void parser::jumpEntry(string key, string val) {
    jumpDictionary.insert(pair<string, string>(key, val));
}


string parser::compLookup(string instComp) {
    if (compDictionary.find(instComp) == compDictionary.end()) {
        return compDictionary.find("0")->second;  
    } else {
        return compDictionary.find(instComp)->second;  
    }
}
string parser::destLookup(string instDest) {
    if (destDictionary.find(instDest) == destDictionary.end()) {
        return destDictionary.find("null")->second;  
    } else {
        return destDictionary.find(instDest)->second;  
    }
}
string parser::jumpLookup(string instJump) {
    if (jumpDictionary.find(instJump) == jumpDictionary.end()) {
        return jumpDictionary.find("null")->second; 
    } else {
        return jumpDictionary.find(instJump)->second;  
    }
}

string parser::parseInst(string inst) {
    string tempInst;
    if (inst.find('@') != string::npos) {
        tempInst = inst.substr(1, inst.size() - 1);  
        return parseAInst(tempInst);    
    } else {
        return parseCInst(inst);  
    }
}

string parser::parseAInst(string inst) {
    if (inst.find_first_not_of("0123456789") == string::npos) {
        return prefixA + bitset<15>(stoi(inst)).to_string();  //stoi = string to Integer
    } else {
        if (inst.find_first_of("0123456789") < inst.find_first_not_of("0123456789")) {  
            throw runtime_error("Parser::parseAInst(): invalid A-inst: number first");
        }
        uint16_t addr = symTab.lookupSymbol(inst);
        return prefixA + bitset<15>(addr).to_string();
    }
}

string parser::parseCInst(string inst) {
    string::size_type idxe = inst.find('=');
    string::size_type idxsc = inst.find(';');

    string instDest = (idxe == string::npos) ? "null" : inst.substr(0, idxe);
    string instComp = inst.substr(idxe + 1, idxsc - idxe - 1);
    string instJump = (idxsc == string::npos) ? "null" : inst.substr(idxsc + 1, inst.size() - 1);

    return prefixC + compLookup(instComp) + destLookup(instDest) + jumpLookup(instJump);
}