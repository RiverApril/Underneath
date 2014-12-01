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


namespace FileUtility {

    struct ExceptionLoad{
        ExceptionLoad(string description){
            this->description = description;
        }
        string description;
    };


    void saveInt(vector<unsigned char>* data, int n);
    void saveUnsignedLong(vector<unsigned char>* data, unsigned long n);
    void saveUnsignedChar(vector<unsigned char>* data, unsigned char n);
    void saveInt8Bit(vector<unsigned char>* data, int8_t n);
    void saveBool(vector<unsigned char>* data, bool n);
    void saveString(vector<unsigned char>* data, string n);


    int loadInt(unsigned char* data, int* position);
    unsigned long loadUnsignedLong(unsigned char* data, int* position);
    unsigned char loadUnsignedChar(unsigned char* data, int* position);
    int8_t loadInt8Bit(unsigned char* data, int* position);
    bool loadBool(unsigned char* data, int* position);
    string loadString(unsigned char* data, int* position);

}

namespace DrawingUtility {


    string makeBar(int progress, int maxProgress, unsigned int size, char filled = '=', char empty = '-');
}


#endif /* defined(__Underneath__Utility__) */
