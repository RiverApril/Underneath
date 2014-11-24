//
//  Utility.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Utility.h"

namespace FileUtility {



    void saveInt(string* data, int n){
        data->push_back((char)((n >> 24) & 0xFF));
        data->push_back((char)((n >> 16) & 0xFF));
        data->push_back((char)((n >> 8) & 0xFF));
        data->push_back((char)(n & 0xFF));
    }

    void saveChar(string* data, char n){
        data->push_back(n);
    }

    void saveInt8Bit(string* data, int8_t n){
        data->push_back((char)n);
    }

    void saveBool(string* data, bool n){
        data->push_back((char)(n?'T':'F'));
    }

    void saveString(string* data, string n){
        int count = (int)n.size();
        saveInt(data, count);
        for(int i=0;i<count;i++){
            saveChar(data, n[i]);
        }
    }



    

    int loadInt(char* data, int* position){
        return ((loadChar(data, position)<<24) |
                (loadChar(data, position)<<16) |
                (loadChar(data, position)<< 8) |
                (loadChar(data, position)<< 0));
    }

    char loadChar(char* data, int* position){
        char l = data[*position];
        *position+=1;
        return l;
    }

    int8_t loadInt8Bit(char* data, int* position){
        int8_t l = (int8_t)(data[*position]);
        *position+=1;
        return l;
    }

    bool loadBool(char* data, int* position){
        bool l = ((char)(data[*position]))=='T';
        *position+=1;
        return l;
    }

    string loadString(char* data, int* position){
        int count = loadInt(data, position);
        string l;
        for(int i=0;i<count;i++){
            l+=loadChar(data, position);
        }
        return l;
    }

}

namespace DrawingUtility {

    string makeBar(int progress, int maxProgress, unsigned int size, char filled, char empty){
        string s = "";
        int l = (int)(((double)size/maxProgress)*progress);
        for(int i=0;i<size;i++){
            s+=i>=l?empty:filled;
        }
        return s;
    }

}


