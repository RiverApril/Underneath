//
//  Utility.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Utility.hpp"
#include "Ui.hpp"


namespace Utility {

    void saveDouble(vector<unsigned char>* data, double n) {

        union {
            double d;
            unsigned char b[sizeof (double)];
        } uDoubleBytes;

        uDoubleBytes.d = n;
        for (size_t i = 0; i<sizeof (double); i++) {
            data->push_back((unsigned char) (((uDoubleBytes.b[i])/* >> (i*8)*/) & 0xFF));
        }
    }

    void saveInt(vector<unsigned char>* data, int n) {
        data->push_back((unsigned char) ((n >> 24) & 0xFF));
        data->push_back((unsigned char) ((n >> 16) & 0xFF));
        data->push_back((unsigned char) ((n >> 8) & 0xFF));
        data->push_back((unsigned char) (n & 0xFF));
    }

    void saveUnsignedLong(vector<unsigned char>* data, unsigned long n) {
        data->push_back((unsigned char) ((n >> 48) & 0xFF));
        data->push_back((unsigned char) ((n >> 40) & 0xFF));
        data->push_back((unsigned char) ((n >> 32) & 0xFF));
        data->push_back((unsigned char) ((n >> 24) & 0xFF));
        data->push_back((unsigned char) ((n >> 16) & 0xFF));
        data->push_back((unsigned char) ((n >> 8) & 0xFF));
        data->push_back((unsigned char) (n & 0xFF));
    }

    void saveUnsignedChar(vector<unsigned char>* data, unsigned char n) {
        data->push_back(n);
    }

    void saveUInt8Bit(vector<unsigned char>* data, uint8_t n) {
        data->push_back((unsigned char) n);
    }

    void saveBool(vector<unsigned char>* data, bool n) {
        data->push_back((unsigned char) (n ? 'T' : 'F'));
    }

    void saveString(vector<unsigned char>* data, string n) {
        size_t count = n.size();
        saveInt(data, (int) count);
        for (size_t i = 0; i < count; i++) {
            saveUnsignedChar(data, (unsigned char) n[i]);
        }
    }

    double loadDouble(unsigned char* data, int* position) {

        union {
            double d;
            unsigned char b[sizeof (double)];
        } uDoubleBytes;

        for (size_t i = 0; i<sizeof (double); i++) {
            uDoubleBytes.b[i] = ((unsigned char) loadUnsignedChar(data, position));
        }
        return uDoubleBytes.d;
    }

    int loadInt(unsigned char* data, int* position) {
        return ((loadUnsignedChar(data, position) << 24) |
                (loadUnsignedChar(data, position) << 16) |
                (loadUnsignedChar(data, position) << 8) |
                (loadUnsignedChar(data, position) << 0));
    }

    unsigned long loadUnsignedLong(unsigned char* data, int* position) {
        return ((((unsigned long) loadUnsignedChar(data, position)) << 48) |
                (((unsigned long) loadUnsignedChar(data, position)) << 40) |
                (((unsigned long) loadUnsignedChar(data, position)) << 32) |
                (((unsigned long) loadUnsignedChar(data, position)) << 24) |
                (((unsigned long) loadUnsignedChar(data, position)) << 16) |
                (((unsigned long) loadUnsignedChar(data, position)) << 8) |
                (((unsigned long) loadUnsignedChar(data, position)) << 0));
    }

    unsigned char loadUnsignedChar(unsigned char* data, int* position) {
        unsigned char l = data[*position];
        *position += 1;
        return l;
    }

    int8_t loadInt8Bit(unsigned char* data, int* position) {
        int8_t l = (int8_t) (data[*position]);
        *position += 1;
        return l;
    }

    bool loadBool(unsigned char* data, int* position) {
        bool l = ((char) (data[*position])) == 'T';
        *position += 1;
        return l;
    }

    string loadString(unsigned char* data, int* position) {
        int count = loadInt(data, position);
        string l;
        for (int i = 0; i < count; i++) {
            l += (char) loadUnsignedChar(data, position);
        }
        return l;
    }

    string readTextFile(string path, string defaultString) {

        FILE* file;
        file = fopen(path.c_str(), "r");
        if (file != NULL) {
            string s;
            int c;
            do {
                c = fgetc(file);
                s += (char) c;
            } while (c != EOF);

            fclose(file);
            return s;
        } else {
            return defaultString;
        }
    }

    //Strings

    string makeBar(int progress, int maxProgress, int size, char filled, char empty) {
        if(size <= 0){
            return "";
        }
        string s = "";
        int l = (((double) size / maxProgress) * progress);
        for (int i = 0; i < size; i++) {
            s += i >= l ? empty : filled;
        }
        return s;
    }

    string repeatString(string s, int qty) {
        string v = "";
        for (size_t i = 0; i < qty; i++) {
            v += s;
        }
        return v;
    }



    //Parsing

    int getNumberFromSymbol(char symbol) {
        if (symbol >= 48 && symbol <= 57) { // 0 - 9
            return symbol - 48;
        } else if (symbol >= 65 && symbol <= 90) { // A - Z
            return symbol - 65 + 10 + 26;
        } else if (symbol >= 97 && symbol <= 122) { // a - z
            return symbol - 97 + 10;
        } else {
            throw ParseException("Invalid symbol: '" + to_string(symbol) + "'");
        }
    }

    char getSymbolFromNumber(int number){
        if(number >= 0 && number <= 9){
            return (char)(number + 48);
        }else if(number >= 10 && number < 10 + 26){
            return (char)(number - 10 + 65);
        }else if(number >= 10 + 26 && number < 10 + 26 + 26){
            return (char)(number - 10 - 26 + 97);
        }
        throw ParseException("Number out of range: " + to_string(number));
    }

    string toString(int num, int base){
        if(base < 1 || base > 62){
            throw ParseException("Base out of range: " + to_string(base));
        }else{
            int maxDigit = 0;
            for(int i=0;true;i++){
                if(num < pow(base, i)){
                    maxDigit = i-1;
                    break;
                }
            }

            vector<int> digitList= {};
            int digitPos = 0;
            for(int i=maxDigit;i>=0;i--){
                int digit = pow(base, i);
                digitList.push_back(0);
                while(num >= digit){
                    num -= digit;
                    digitList[digitPos] ++;
                }
                digitPos++;
            }

            string final = "";

            for(int digit : digitList){
                final += getSymbolFromNumber(digit);
            }


            return final;
        }
    }

    double parseDouble(string s, int base, double defaultIfException) {
        try {
            return parseDoubleUnstable(s, base);
        } catch (ParseException) {
            return defaultIfException;
        }
    }

    double parseDoubleUnstable(string s, int base) {
        if (base < 1 || base > 62) {
            throw ParseException("Base out of range: " + to_string(base));
        } else {
            bool addToLeft = true;
            vector<int> left;
            vector<int> right;
            for (size_t i = 0; i < s.length(); i++) {
                if (s[i] == '.') {
                    if (addToLeft) {
                        addToLeft = false;
                    } else {
                        throw ParseException("Too many decimal points.");
                    }
                } else {
                    int n = getNumberFromSymbol(s[i]);
                    if (n >= base) {
                        throw ParseException("Symbol too large for specified base.");
                    } else {
                        if (addToLeft) {
                            left.push_back(n);
                        } else {
                            right.push_back(n);
                        }
                    }
                }
            }
            double final = 0;
            int d = 0;
            for (int i = (int) left.size() - 1; i >= 0; i--) {
                final += pow(base, d) * left[i];
                d++;
            }

            d = -1;

            for (size_t i = 0; i < right.size(); i++) {
                final += pow(base, d) * right[i];
                d--;
            }

            return final;
        }
    }

    int parseInt(string s, int base, int defaultIfException) {
        try {
            return parseIntUnstable(s, base);
        } catch (ParseException) {
            return defaultIfException;
        }
    }

    int parseIntUnstable(string s, int base) {
        if (base < 1 || base > 62) {
            throw ParseException("Base out of range (1-62): " + to_string(base));
        } else {
            vector<int> left;
            for (size_t i = 0; i < s.length(); i++) {
                int n = getNumberFromSymbol(s[i]);
                if (n >= base) {
                    throw ParseException("Symbol too large for specified base.");
                } else {
                    left.push_back(n);
                }
            }
            int final = 0;
            int d = 0;
            for (int i = (int) left.size() - 1; i >= 0; i--) {
                final += pow(base, d) * left[i];
                d++;
            }

            return final;
        }
    }

    string intToRomanNumerals(int a) {
        string s = "";
        if(a == 0){
            s = "N";
        }
        while (a > 0) {
            if (a >= 1000) {
                s += "M";
                a -= 1000;
            } else if (a >= 999) {
                s += "IM";
                a -= 999;
            } else if (a >= 995) {
                s += "VM";
                a -= 995;
            } else if (a >= 990) {
                s += "XM";
                a -= 990;
            } else if (a >= 950) {
                s += "LM";
                a -= 950;
            } else if (a >= 900) {
                s += "CM";
                a -= 900;
            } else if (a >= 500) {
                s += "D";
                a -= 500;
            } else if (a >= 499) {
                s += "ID";
                a -= 499;
            } else if (a >= 495) {
                s += "VD";
                a -= 495;
            } else if (a >= 490) {
                s += "XD";
                a -= 490;
            } else if (a >= 450) {
                s += "LD";
                a -= 450;
            } else if (a >= 400) {
                s += "CD";
                a -= 400;
            } else if (a >= 100) {
                s += "C";
                a -= 100;
            } else if (a >= 99) {
                s += "IC";
                a -= 99;
            } else if (a >= 95) {
                s += "VC";
                a -= 95;
            }  else if (a >= 90) {
                s += "XC";
                a -= 90;
            } else if (a >= 50) {
                s += "L";
                a -= 50;
            } else if (a >= 49) {
                s += "IL";
                a -= 49;
            }  else if (a >= 45) {
                s += "VL";
                a -= 45;
            } else if (a >= 40) {
                s += "XL";
                a -= 40;
            } else if (a >= 10) {
                s += "X";
                a -= 10;
            } else if (a >= 9) {
                s += "IX";
                a -= 9;
            } else if (a >= 5) {
                s += "V";
                a -= 5;
            } else if (a >= 4) {
                s += "IV";
                a -= 4;
            } else if (a >= 1) {
                s += "I";
                a -= 1;
            }
        }
        return s;
    }

    void executeBorder(Point2 min, Point2 max, std::function<void(int, int)> process){
        for(int i=min.x;i<=max.x;i++){
            process(i, min.y);
            process(i, max.y);
        }
        for(int i=min.y;i<=max.y;i++){
            process(min.x, i);
            process(max.x, i);
        }
    }

    void executeGrid(Point2 min, Point2 max, std::function<void(int, int)> process){
        for(int i=min.x;i<=max.x;i++){
            for(int j=min.y;j<=max.y;j++){
                process(i, j);
            }
        }
    }

    vector<Point2> plotLine(Point2 a, Point2 b){
        vector<Point2> line = vector<Point2>();


        int w = b.x - a.x ;
        int h = b.y - a.y ;
        int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0 ;
        if (w<0) dx1 = -1 ; else if (w>0) dx1 = 1 ;
        if (h<0) dy1 = -1 ; else if (h>0) dy1 = 1 ;
        if (w<0) dx2 = -1 ; else if (w>0) dx2 = 1 ;
        int longest = abs(w) ;
        int shortest = abs(h) ;
        if (!(longest>shortest)) {
            longest = abs(h) ;
            shortest = abs(w) ;
            if (h<0) dy2 = -1 ; else if (h>0) dy2 = 1 ;
            dx2 = 0 ;
        }
        int numerator = longest >> 1 ;
        for (int i=0;i<=longest;i++) {
            line.push_back(Point2(a.x, a.y));
            numerator += shortest ;
            if (!(numerator<longest)) {
                numerator -= longest ;
                a.x += dx1 ;
                a.y += dy1 ;
            } else {
                a.x += dx2 ;
                a.y += dy2 ;
            }
        }

        return line;
    }

}
