#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include "parser.h"
#include "symbolTable.h"

using namespace std;

symbolTable symTab;
parser prs;

int main(int argc, char const * argv[]){
    string in, out;
    ifstream fIn;
    ofstream fOut;
    queue<string> q1, q2;
    if (argc < 2) {
        throw runtime_error("main(): plseas specify input file MyAssembler *.asm [*.hack]");
    } else {
        in = string(argv[1]);
        // check the input file is a .asm?
        if (in.find(".asm") == string::npos)
            throw runtime_error("main(): input fime must be .asm");
        fIn.open(in);
        // check the specified file is able to open?
        if (!fIn.is_open()) {
            throw runtime_error("main(): unable to open file " + in);
        }

        if (argc == 2) {
            string::size_type idx = in.find_last_of('.');  // find the base name
            out = in.substr(0, idx) + ".hack";        // change to .hack
        } else if (argc == 3) {
            out = string(argv[2]);
        } else {
            throw runtime_error("main(): too many parameters");
        }
        fOut.open(out);
        if (!fOut.is_open()) {
            throw runtime_error("main(): unable to open file " + out);
        }

        cout << "output to " << out << endl;

        /* Pre pass: read source file clean-up the code */
        cout << "\t======\tPre-pass\t=====\t" << endl;
        string line;
        /* read each line from the .asm file */
        while (getline(fIn, line)) {
            cout << line << endl;
            /* remove comments */
            string::size_type idx = line.find("//");  // find start of "//"
            string lineRmComm = line.substr(0, idx);
            if (lineRmComm.size() == 0) continue;  // skip empty line
            /* remove spaces */
            string::iterator str_iter = remove(lineRmComm.begin(), lineRmComm.end(), ' ');
            lineRmComm.erase(str_iter, lineRmComm.end());
            q1.push(lineRmComm);  // push to queue
        }

        /* First pass read lables build symbol table*/
        cout << "\t======\tFirst-pass\t======\t" << endl;
        while (!q1.empty()) {
            line = q1.front();
            q1.pop();
            cout << line << endl;
            /* Is this line a L-instruction (Label) */
            if (line.find('(') != string::npos && line.find(')') != string::npos) {
                string::size_type idx_L = line.find_last_of('(');
                string::size_type idx_R = line.find_first_of(')');
                line = line.substr(idx_L + 1, idx_R - 1);
                symTab.addLabel(line);
                continue;  // skip the push to the q2
            } else {
                symTab.incLabelCounter();  // move the label counter to the next line (ROM Address)
            }
            q2.push(line);
        }

        /* Second pass Translate instructions */
        cout << "\t======\tSecond-pass\t======\t" << endl;
        while (!q2.empty()) {
            line = q2.front();
            q2.pop();
            string macCode = prs.parseInst(line);  // translate into machine code
            cout << line << "\t->\t" << macCode << endl;
            fOut << macCode << endl;
        }
        cout << "second pass finished" << endl;
        fIn.close();
        fOut.close();
        cout << "file closed" << endl;
    }
    return 0;
}