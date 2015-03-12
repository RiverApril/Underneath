//
//  Utility.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Utility.h"


namespace Utility {

    void saveDouble(vector<unsigned char>* data, double n){

        union{
            double d;
            unsigned char b[sizeof(double)];
        } uDoubleBytes;

        uDoubleBytes.d = n;
        for(size_t i=0;i<sizeof(double);i++){
            data->push_back((unsigned char)(((uDoubleBytes.b[i])/* >> (i*8)*/) & 0xFF));
        }
    }

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
        size_t count = n.size();
        saveInt(data, (int)count);
        for(size_t i=0;i<count;i++){
            saveUnsignedChar(data, (unsigned char)n[i]);
        }
    }


    double loadDouble(unsigned char* data, int* position){
        union{
            double d;
            unsigned char b[sizeof(double)];
        } uDoubleBytes;

        for(size_t i=0;i<sizeof(double);i++){
            uDoubleBytes.b[i] = ((unsigned char)loadUnsignedChar(data, position));
        }
        return uDoubleBytes.d;
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

    string readTextFile(string path, string defaultString){
        
        FILE* file;
        file = fopen(path.c_str(), "r");
        if(file != NULL){
            string s;
            int c;
            do{
                c = fgetc(file);
                s += (char)c;
            }while(c != EOF);

            fclose(file);
            return s;
        }else{
            return defaultString;
        }
    }

    //Strings

    string makeBar(int progress, int maxProgress, size_t size, char filled, char empty){
        string s = "";
        size_t l = (((double)size/maxProgress)*progress);
        for(size_t i=0;i<size;i++){
            s+=i>=l?empty:filled;
        }
        return s;
    }

    string repeatString(string s, size_t qty){
        string v = "";
        for(size_t i=0;i<qty;i++){
            v += s;
        }
        return v;
    }



    //Parsing

    int getNumberFromSymbol(char symbol){
        if(symbol >= 48 && symbol <= 57){
            return symbol - 48;
        }else if(symbol >= 65 && symbol <= 90){
            return symbol - 65 + 10;
        }else if(symbol >= 97 && symbol <= 122){
            return symbol - 97 + 10;
        }else{
            throw ParseException("Invalid symbol: '"+to_string(symbol)+"'");
        }
    }

    double parseDouble(string s, int base, double defaultIfException){
        try{
            return parseDoubleUnstable(s, base);
        }catch(ParseException){
            return defaultIfException;
        }
    }

    double parseDoubleUnstable(string s, int base){
        if(base < 1 || base > 36){
            throw ParseException("Base out of range: "+to_string(base));
        }else{
            bool addToLeft = true;
            vector<int> left;
            vector<int> right;
            for(size_t i=0;i<s.length();i++){
                if(s[i] == '.'){
                    if(addToLeft){
                        addToLeft = false;
                    }else{
                        throw ParseException("Too many decimal points.");
                    }
                }else{
                    int n = getNumberFromSymbol(s[i]);
                    if(n >= base){
                        throw ParseException("Symbol too large for specified base.");
                    }else{
                        if(addToLeft){
                            left.push_back(n);
                        }else{
                            right.push_back(n);
                        }
                    }
                }
            }
            double final = 0;
            int d = 0;
            for(int i=(int)left.size()-1;i>=0;i--){
                final += pow(base, d)*left[i];
                d++;
            }

            d = -1;

            for(size_t i=0;i<right.size();i++){
                final += pow(base, d)*right[i];
                d--;
            }

            return final;
        }
    }

    int parseInt(string s, int base, int defaultIfException){
        try{
            return parseIntUnstable(s, base);
        }catch(ParseException){
            return defaultIfException;
        }
    }

    int parseIntUnstable(string s, int base){
        if(base < 1 || base > 36){
            throw ParseException("Base out of range: "+to_string(base));
        }else{
            vector<int> left;
            for(size_t i=0;i<s.length();i++){
                int n = getNumberFromSymbol(s[i]);
                if(n >= base){
                    throw ParseException("Symbol too large for specified base.");
                }else{
                    left.push_back(n);
                }
            }
            int final = 0;
            int d = 0;
            for(int i=(int)left.size()-1;i>=0;i--){
                final += pow(base, d)*left[i];
                d++;
            }

            return final;
        }
    }

    string intToRomanNumerals(int a){
        string s = "";
        while(a > 0){
            if(a >= 1000){
            	s+="M";
                a-=1000;
            }else if(a >= 900){
                s+="CM";
                a-=900;
            }else if(a >= 500){
                s+="D";
                a-=500;
            }else if(a >= 400){
                s+="CD";
                a-=400;
            }else if(a >= 100){
                s+="C";
                a-=100;
            }else if(a >= 90){
                s+="XC";
                a-=90;
            }else if(a >= 50){
                s+="L";
                a-=50;
            }else if(a >= 40){
                s+="XL";
                a-=40;
            }else if(a >= 10){
                s+="X";
                a-=10;
            }else if(a >= 9){
                s+="IX";
                a-=9;
            }else if(a >= 5){
                s+="V";
                a-=5;
            }else if(a >= 4){
                s+="IV";
                a-=4;
            }else if(a >= 1){
                s+="I";
                a-=1;
            }
        }
        return s;
    }

}

