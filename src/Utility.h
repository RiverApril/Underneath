//
//  Utility.h
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Utility__
#define __Underneath__Utility__

#include "Global.h"


namespace Utility{

    void saveInt(std::string* data, int n);
    void saveChar(std::string* data, char n);
    void saveInt8Bit(std::string* data, int8_t n);
    void saveBool(std::string* data, bool n);
    void saveString(std::string* data, std::string n);


    int loadInt(char* data, int* position);
    char loadChar(char* data, int* position);
    int8_t loadInt8Bit(char* data, int* position);
    bool loadBool(char* data, int* position);
    std::string loadString(char* data, int* position);
}


#endif /* defined(__Underneath__Utility__) */
