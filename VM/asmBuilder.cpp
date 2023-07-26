#include "asmBuilder.h"

asmBuilder::asmBuilder(){
    asmStr.str(string());
    name = "MyModule";
    symbolCounter = 0;
    map_segments.clear();
    map_segments.insert(pair<string, segments>("constant", seg_constant));
    map_segments.insert(pair<string, segments>("static", seg_static));
    map_segments.insert(pair<string, segments>("temp", seg_temp));
    map_segments.insert(pair<string, segments>("pointer", seg_pointer));
    map_segments.insert(pair<string, segments>("local", seg_local));
    map_segments.insert(pair<string, segments>("argument", seg_argument));
    map_segments.insert(pair<string, segments>("this", seg_this));
    map_segments.insert(pair<string, segments>("that", seg_that));
}

asmBuilder::~asmBuilder(){
    map_segments.clear();
}

string asmBuilder::addFunction(){
    asmStr.str(string());
    write("@SP // add");
    write("AM=M-1");
    write("D=M");
    write("A=A-1");
    write("M=D+M");
    return asmStr.str() + "\n";
}

string asmBuilder::subFunction(){
    asmStr.str(string());
    write("@SP // sub");
    write("AM=M-1");
    write("D=M");
    write("A=A-1");
    write("M=M-D");
    return asmStr.str() + "\n";
}

string asmBuilder::negFunction(){
    asmStr.str(string());
    write("@SP // neg");
    write("A=M");
    write("A=A-1");
    write("M=-M");
    return asmStr.str() + "\n";
}

string asmBuilder::eqFunction(){
    asmStr.str(string());
    string labelTrue("JGT_TRUE_" + name + "_" + to_string(symbolCounter));
    string labelFalse("JGT_FALSE_" + name + "_" + to_string(symbolCounter));

    write("@SP // eq");
    write("AM=M-1");
    write("D=M");
    write("@SP");
    write("AM=M-1");
    write("D=M-D");
    write("@" + labelTrue);
    write("D;JEQ");
    write("D=0");
    write("@" + labelFalse);
    write("0;JMP");
    write("(" + labelTrue + ")");
    write("D=-1");
    write("(" + labelFalse + ")");
    write("@SP");
    write("A=M");
    write("M=D");
    write("@SP");
    write("M=M+1");

    symbolCounter++;
    return asmStr.str() + "\n";
}

string asmBuilder::gtFunction(){
    asmStr.str(string());
    string labelTrue("JGT_TRUE_" + name + "_" + to_string(symbolCounter));
    string labelFalse("JGT_FALSE_" + name + "_" + to_string(symbolCounter));

    write("@SP // gt");
    write("AM=M-1");
    write("D=M");
    write("@SP");
    write("AM=M-1");
    write("D=M-D");
    write("@" + labelTrue);
    write("D;JGT");
    write("D=0");
    write("@" + labelFalse);
    write("0;JMP");
    write("(" + labelTrue + ")");
    write("D=-1");
    write("(" + labelFalse + ")");
    write("@SP");
    write("A=M");
    write("M=D");
    write("@SP");
    write("M=M+1");

    symbolCounter++;
    return asmStr.str() + "\n";
}

string asmBuilder::ltFunction(){
    asmStr.str(string());
    string labelTrue("JLT_TRUE_" + name + "_" + to_string(symbolCounter));
    string labelFalse("JLT_FALSE_" + name + "_" + to_string(symbolCounter));

    write("@SP // lt");
    write("AM=M-1");
    write("D=M");
    write("@SP");
    write("AM=M-1");
    write("D=M-D");
    write("@" + labelTrue);
    write("D;JLT");
    write("D=0");
    write("@" + labelFalse);
    write("0;JMP");
    write("(" + labelTrue + ")");
    write("D=-1");
    write("(" + labelFalse + ")");
    write("@SP");
    write("A=M");
    write("M=D");
    write("@SP");
    write("M=M+1");

    symbolCounter++;
    return asmStr.str() + "\n";
}

string asmBuilder::andFunction(){
    asmStr.str(string());
    write("@SP // and");
    write("AM=M-1");
    write("D=M");
    write("A=A-1");
    write("M=D&M");
    return asmStr.str() + "\n";
}

string asmBuilder::orFunction(){
    asmStr.str(string());
    write("@SP // or");
    write("AM=M-1");
    write("D=M");
    write("A=A-1");
    write("M=D|M");
    return asmStr.str() + "\n";
}


string asmBuilder::notFunction(){
    asmStr.str(string());
    write("@SP // not");
    write("A=M");
    write("A=A-1");
    write("M=!M");
    return asmStr.str() + "\n";
}

string asmBuilder::pushFunction(string segment, int offset){
    asmStr.str(string());
    string indexStr = to_string(offset); 
    string registerStr = registerName(segment, offset);
    switch (map_segments[segment])
    {
    case seg_constant:
        write("@" + indexStr + " // push " + segment + " " + indexStr);
        write("D=A");
        write("@SP");
        write("A=M");
        write("M=D");
        write("@SP");
        write("M=M+1");
        break;
    case seg_static:
    case seg_temp:
    case seg_pointer:
        write("@" + registerStr + " // push " + segment + " " + indexStr);
        write("D=A");
        write("@" + indexStr);
        write("A=D+A");
        write("D=M");
        write("@SP");
        write("A=M");
        write("M=D");
        write("@SP");
        write("M=M+1");
        break;
    case seg_argument:
    case seg_local:
    case seg_this:
    case seg_that:
        write("@" + registerStr + " // push " + segment + " " + indexStr);
        write("D=M");
        write("@" + indexStr);
        write("A=D+A");
        write("D=M");
        write("@SP");
        write("A=M");
        write("M=D");
        write("@SP");
        write("M=M+1");
        break;
    default:
        throw runtime_error("vm_push(): Invalid segment");
    }
    return asmStr.str() + "\n";
}

string asmBuilder::popFunction(string segment, int offset){
    asmStr.str(string());
    string indexStr = to_string(offset);
    string registerStr = registerName(segment, offset);
    switch (map_segments[segment])
    {
    case seg_constant:
        throw runtime_error("vm_pop(): cannot pop to constant");
        break;
    case seg_static:
        write("@" + registerStr + " // pop " + segment + " " + indexStr);
        write("D=A");
        write("@" + indexStr);
        write("D=D+A");
        write("@R13");
        write("M=D");
        write("@SP");
        write("AM=M-1");
        write("D=M");
        write("@R13");
        write("A=M");
        write("M=D");
        break;
    case seg_argument:
    case seg_local:
    case seg_this:
    case seg_that:
    case seg_temp:
    case seg_pointer:
        write("@" + registerStr + " // pop " + segment + " " + indexStr);
        write("D=M");
        write("@" + indexStr);
        write("D=D+A");
        write("@R13");
        write("M=D");
        write("@SP");
        write("AM=M-1");
        write("D=M");
        write("@R13");
        write("A=M");
        write("M=D");
        break;
    default:
        throw runtime_error("vm_pop(): Invalid segment");
    }
    return asmStr.str() + "\n";
}

string asmBuilder::labelFunction(string label){
    asmStr.str(string());
    write("(" + label + ") // label " + label);
    return asmStr.str() + "\n";
}

string asmBuilder::gotoFunction(string label){
    asmStr.str(string());
    write("@" + label + "// goto " + label);
    write("0;JMP");
    return asmStr.str() + "\n";
}

string asmBuilder::if_gotoFunction(string label){
    asmStr.str(string());
    write("@SP // if-goto " + label);
    write("AM=M-1");
    write("D=M");
    write("@" + label);
    write("D;JNE");
    return asmStr.str() + "\n";
}

string asmBuilder::function(string function_name, int n_vars){
    asmStr.str(string());
    write("(" + function_name + ") // function " + function_name + " " + to_string(n_vars));
    for (int n = n_vars; n > 0; n--)
    {
        write("@SP");
        write("AM=M+1");
        write("A=A-1");
        write("M=0");
    }
    return asmStr.str() + "\n";
}

string asmBuilder::callFunction(string function_name, int n_args){
    asmStr.str(string());
    write("@return_address // call " + function_name + " " + to_string(n_args));
    write("D=A");
    write("@SP");
    write("AM=M+1");
    write("A=A-1");
    write("M=D");

    write("@LCL");
    write("D=M");
    write("@SP");
    write("AM=M+1");
    write("A=A-1");
    write("M=D");

    write("@ARG");
    write("D=M");
    write("@SP");
    write("AM=M+1");
    write("A=A-1");
    write("M=D");

    write("@THIS");
    write("D=M");
    write("@SP");
    write("AM=M+1");
    write("A=A-1");
    write("M=D");

    write("@THAT");
    write("D=M");
    write("@SP");
    write("AM=M+1");
    write("A=A-1");
    write("M=D");

    write("@SP");
    write("D=M");
    write("@5");
    write("D=D-A");
    write("@" + to_string(n_args));
    write("D=D-A");
    write("@ARG");
    write("M=D");

    write("@SP");
    write("D=M");
    write("@LCL");
    write("M=D");

    write("@funcName");
    write("0;JMP");

    write("(return_address)");
    return asmStr.str() + "\n";
}

string asmBuilder::returnFunction(){
    asmStr.str(string());
    write("@LCL // return");
    write("D=M");
    write("@R13");
    write("M=D");

    write("@R13");
    write("D=M");
    write("@5");
    write("A=D-A");
    write("D=M");
    write("@R14");
    write("M=D");

    write("@SP");
    write("AM=M-1");
    write("D=M");
    write("@ARG");
    write("A=M");
    write("M=D");

    write("@ARG");
    write("D=M+1");
    write("@SP");
    write("M=D");

    write("@R13");
    write("D=M");
    write("@1");
    write("A=D-A");
    write("D=M");
    write("@THAT");
    write("M=D");

    write("@R13");
    write("D=M");
    write("@2");
    write("A=D-A");
    write("D=M");
    write("@THIS");
    write("M=D");

    write("@R13");
    write("D=M");
    write("@3");
    write("A=D-A");
    write("D=M");
    write("@ARG");
    write("M=D");

    write("@R13");
    write("D=M");
    write("@4");
    write("A=D-A");
    write("D=M");
    write("@LCL");
    write("M=D");

    write("@R14");
    write("A=M");
    write("0;JMP");

    return asmStr.str() + "\n";
}

string asmBuilder::endFunction(){
    asmStr.str(string());
    write("(END) // END loop");
    write("@END");
    write("0;JMP");
    return asmStr.str() + "\n";
}

void asmBuilder::write(string vmCode){
    if (vmCode.find("(") == string::npos)
        asmStr << "\t"; 
    asmStr << vmCode << endl;
}

string asmBuilder::registerName(string segment, int index){
    if (segment == "static")
        return "16";
    if (segment == "local")
        return "LCL";
    if (segment == "argument")
        return "ARG";
    if (segment == "this")
        return "THIS";
    if (segment == "that")
        return "THAT";
    if (segment == "pointer")
        return "R" + to_string(3 + index);
    if (segment == "temp")
        return "R" + to_string(5 + index);

    return name + "." + to_string(index); 
}