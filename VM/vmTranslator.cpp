#include "vmTranslator.h"
asmBuilder assembly;
vmTranslator::vmTranslator(){}
vmTranslator::~vmTranslator(){}

string vmTranslator::parse(string vmLine){
    regex endl_re("\\r*\\n+");
    regex space_re("\\s+");
    vmLine = regex_replace(vmLine, endl_re, "");
    vmLine = regex_replace(vmLine, space_re, " ");
    vector<string> tokens;
    stringstream ss(vmLine);
    string token;

    while (getline(ss, token, ' '))
    {
        tokens.push_back(token);
    }

    if (tokens.size() == 1)
    {
        if (tokens[0] == "add")
        {
            return assembly.addFunction();
        }
        else if (tokens[0] == "sub")
        {
            return assembly.subFunction();
        }
        else if (tokens[0] == "neg")
        {
            return assembly.negFunction();
        }
        else if (tokens[0] == "eq")
        {
            return assembly.eqFunction();
        }
        else if (tokens[0] == "gt")
        {
            return assembly.gtFunction();
        }
        else if (tokens[0] == "lt")
        {
            return assembly.ltFunction();
        }
        else if (tokens[0] == "and")
        {
            return assembly.andFunction();
        }
        else if (tokens[0] == "or")
        {
            return assembly.orFunction();
        }
        else if (tokens[0] == "not")
        {
            return assembly.notFunction();
        }
        else if (tokens[0] == "end")
        {
            return assembly.endFunction();
        }
        else if (tokens[0] == "return")
        {
            return assembly.returnFunction();
        }
    }
    else if (tokens.size() == 2)
    {
        if (tokens[0] == "label")
        {
            return assembly.labelFunction(tokens[1]);
        }
        else if (tokens[0] == "goto")
        {
            return assembly.gotoFunction(tokens[1]);
        }
        else if (tokens[0] == "if-goto")
        {
            return assembly.if_gotoFunction(tokens[1]);
        }
    }
    else if (tokens.size() == 3)
    {
        int t2;
        try
        {
            t2 = stoi(tokens[2]);
        }
        catch (const invalid_argument &ia)
        {
            cerr << "Unable to parse int." << tokens[2];
        }
        if (tokens[0] == "push")
        {
            return assembly.pushFunction(tokens[1], t2);
        }
        else if (tokens[0] == "pop")
        {
            return assembly.popFunction(tokens[1], t2);
        }
        else if (tokens[0] == "function")
        {
            return assembly.function(tokens[1], t2);
        }
        else if (tokens[0] == "call")
        {
            return assembly.callFunction(tokens[1], t2);
        }
    }
    return "";
}