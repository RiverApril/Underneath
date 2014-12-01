//
//  Utility.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Utility.h"

namespace FileUtility {



    void saveInt(vector<unsigned char>* data, int n){
        data->push_back((unsigned char)((n >> 24) & 0xFF));
        data->push_back((unsigned char)((n >> 16) & 0xFF));
        data->push_back((unsigned char)((n >> 8) & 0xFF));
        data->push_back((unsigned char)(n & 0xFF));
    }

    void saveUnsignedLong(vector<unsigned char>* data, unsigned long n){
        data->push_back((unsigned char)((n >> 48) & 0xFF));
        data->push_back((unsigned char)((n >> 40) & 0xFF));
        data->push_back((unsigned char)((n >> 32) & 0xFF));
        data->push_back((unsigned char)((n >> 24) & 0xFF));
        data->push_back((unsigned char)((n >> 16) & 0xFF));
        data->push_back((unsigned char)((n >> 8) & 0xFF));
        data->push_back((unsigned char)(n & 0xFF));
    }

    void saveUnsignedChar(vector<unsigned char>* data, unsigned char n){
        data->push_back(n);
    }

    void saveInt8Bit(vector<unsigned char>* data, int8_t n){
        data->push_back((unsigned char)n);
    }

    void saveBool(vector<unsigned char>* data, bool n){
        data->push_back((unsigned char)(n?'T':'F'));
    }

    void saveString(vector<unsigned char>* data, string n){
        int count = (int)n.size();
        saveInt(data, count);
        for(int i=0;i<count;i++){
            saveUnsignedChar(data, (unsigned char)n[i]);
        }
    }

	

    int loadInt(unsigned char* data, int* position){
        return ((loadUnsignedChar(data, position)<<24) |
                (loadUnsignedChar(data, position)<<16) |
                (loadUnsignedChar(data, position)<< 8) |
                (loadUnsignedChar(data, position)<< 0));
    }

    unsigned long loadUnsignedLong(unsigned char* data, int* position){
        return ((((unsigned long)loadUnsignedChar(data, position))<<48) |
				(((unsigned long)loadUnsignedChar(data, position))<<40) |
				(((unsigned long)loadUnsignedChar(data, position))<<32) |
                (((unsigned long)loadUnsignedChar(data, position))<<24) |
                (((unsigned long)loadUnsignedChar(data, position))<<16) |
                (((unsigned long)loadUnsignedChar(data, position))<< 8) |
                (((unsigned long)loadUnsignedChar(data, position))<< 0));
    }

    unsigned char loadUnsignedChar(unsigned char* data, int* position){
        unsigned char l = data[*position];
        *position+=1;
        return l;
    }

    int8_t loadInt8Bit(unsigned char* data, int* position){
        int8_t l = (int8_t)(data[*position]);
        *position+=1;
        return l;
    }

    bool loadBool(unsigned char* data, int* position){
        bool l = ((char)(data[*position]))=='T';
        *position+=1;
        return l;
    }

    string loadString(unsigned char* data, int* position){
        int count = loadInt(data, position);
        string l;
        for(int i=0;i<count;i++){
            l+=(char)loadUnsignedChar(data, position);
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


