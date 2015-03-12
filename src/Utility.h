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


namespace Utility {

    struct FileExceptionLoad{
        FileExceptionLoad(string description){
            this->description = description;
        }
        string description;
    };

    


	void saveDouble(vector<unsigned char>* data, double n);
    void saveInt(vector<unsigned char>* data, int n);
    void saveUnsignedLong(vector<unsigned char>* data, unsigned long n);
    void saveUnsignedChar(vector<unsigned char>* data, unsigned char n);
    void saveInt8Bit(vector<unsigned char>* data, int8_t n);
    void saveBool(vector<unsigned char>* data, bool n);
    void saveString(vector<unsigned char>* data, string n);
    
    template <typename T>
    void saveType(vector<unsigned char>* data, T n){
        union{
            T d;
            unsigned char b[sizeof(T)];
        } uTypeBytes;

        uTypeBytes.d = n;

        for(size_t i=0;i<sizeof(T);i++){
            data->push_back((unsigned char)(((uTypeBytes.b[i])/* >> (i*8)*/) & 0xFF));
        }
    }



	double loadDouble(unsigned char* data, int* position);
    int loadInt(unsigned char* data, int* position);
    unsigned long loadUnsignedLong(unsigned char* data, int* position);
    unsigned char loadUnsignedChar(unsigned char* data, int* position);
    int8_t loadInt8Bit(unsigned char* data, int* position);
    bool loadBool(unsigned char* data, int* position);
    string loadString(unsigned char* data, int* position);

    template <typename T>
    T loadType(unsigned char* data, int* position){

        union{
            T d;
            unsigned char b[sizeof(T)];
        } uTypeBytes;

        for(size_t i=0;i<sizeof(T);i++){
            uTypeBytes.b[i] = ((unsigned char)loadUnsignedChar(data, position));
        }
        return uTypeBytes.d;
    }

    string readTextFile(string path, string defaultString = "");





    string makeBar(int progress, int maxProgress, size_t size, char filled = '=', char empty = '-');

    string repeatString(string s, size_t qty);



    
    
    template <typename T>
    void swap(T& a, T& b){
        T& temp = a;
        b = a;
        a = temp;
    }


    template <typename T>
    bool vectorContains(vector<T> v, T c){
        for(T i : v){
            if(c == i){
                return true;
            }
        }
        return false;
    }
    


    struct ParseException{
        ParseException(string reason){
            this->reason = reason;
        }
        string reason;
    };

    int getNumberFromSymbol(char symbol);

    double parseDouble(string s, int base = 10, double defaultIfException = 0);

    double parseDoubleUnstable(string s, int base = 10);

    int parseInt(string s, int base = 10, int defaultIfException = 0);

    int parseIntUnstable(string s, int base = 10);

    string intToRomanNumerals(int a);
}


#endif /* defined(__Underneath__Utility__) */
