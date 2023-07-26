#include <algorithm> 
#include <fstream>   
#include <iostream>  
#include <string>    
#include "vmTranslator.h"

using namespace std;

vmTranslator vm;
string moduleName;

int main(int argc, char const *argv[])
{
    string in, out;
    ifstream fIn;
    ofstream fOut;

    if (argc < 2)
    {
        throw runtime_error("main(): plseas specify input file VMTranslator *.vm [*.asm]");
    }
    else
    {
        in = string(argv[1]);
        if (in.find(".vm") == string::npos)
            throw runtime_error("main(): input fime must be .vm");
        fIn.open(in);
        if (!fIn.is_open())
        {
            throw runtime_error("main(): unable to open file " + in);
        }
        string::size_type idx = in.find_last_of('.'); 
        string fName = in.substr(0, idx);
        if (argc == 2)
        {
            out = fName + ".asm";
        }
        else if (argc == 3)
        {
            out = string(argv[2]);
        }
        else
        {
            throw runtime_error("main(): too many parameters");
        }
        fOut.open(out, ofstream::trunc);
        if (!fOut.is_open())
        {
            throw runtime_error("main(): unable to open file " + out);
        }
        string::size_type relPath = in.find_last_of('\\');     
        moduleName = fName.substr(relPath + 1, fName.length() - 1); 

        cout << "output to " << out << endl;
        string line;
        while (getline(fIn, line))
        {
            string wtof = vm.parse(line); 
            fOut << wtof;
        }
        fOut << vm.parse("end");
        fIn.close();
        fOut.close();
    }
    return 0;
}