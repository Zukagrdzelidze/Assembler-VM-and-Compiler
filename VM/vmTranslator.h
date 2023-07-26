#pragma once

#include <bitset>     
#include <cstdint>    
#include <iostream>   
#include <map>       
#include <regex>      
#include <stdexcept>  
#include <string>    

#include "asmBuilder.h"

using namespace std;

class vmTranslator{
    public: 
        vmTranslator();
        ~vmTranslator();
        string parse(string vmLine);
};