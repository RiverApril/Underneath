//
//  Utility.hpp
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Utility__
#define __Underneath__Utility__

#include "Global.hpp"
#include "Geometry.hpp"

class Level;

namespace Utility {

    struct FileExceptionLoad {

        FileExceptionLoad(string description) {
            this->description = description;
        }
        string description;
    };




    void saveDouble(vector<unsigned char>* data, double n);
    void saveInt(vector<unsigned char>* data, int n);
    void saveUnsignedInt(vector<unsigned char>* data, unsigned int n);
    void saveUnsignedLong(vector<unsigned char>* data, unsigned long n);
    void saveUnsignedChar(vector<unsigned char>* data, unsigned char n);
    void saveUInt8Bit(vector<unsigned char>* data, uint8_t n);
    void saveBool(vector<unsigned char>* data, bool n);
    void saveString(vector<unsigned char>* data, string n);
    void save8Bits(vector<unsigned char>* data, bool b[8]);

    template <typename T>
    void saveType(vector<unsigned char>* data, T n) {

        union {
            T d;
            unsigned char b[sizeof (T)];
        } uTypeBytes;

        uTypeBytes.d = n;

        for (size_t i = 0; i<sizeof (T); i++) {
            data->push_back((unsigned char) (((uTypeBytes.b[i])/* >> (i*8)*/) & 0xFF));
        }
    }



    double loadDouble(vector<unsigned char>* data, int* position);
    int loadInt(vector<unsigned char>* data, int* position);
    unsigned int loadUnsignedInt(vector<unsigned char>* data, int* position);
    unsigned long loadUnsignedLong(vector<unsigned char>* data, int* position);
    unsigned char loadUnsignedChar(vector<unsigned char>* data, int* position);
    int8_t loadInt8Bit(vector<unsigned char>* data, int* position);
    bool loadBool(vector<unsigned char>* data, int* position);
    string loadString(vector<unsigned char>* data, int* position);
    void load8Bits(vector<unsigned char>* data, int* position, bool b[8]);

    template <typename T>
    T loadType(vector<unsigned char>* data, int* position) {

        union {
            T d;
            unsigned char b[sizeof (T)];
        } uTypeBytes;

        for (size_t i = 0; i<sizeof (T); i++) {
            uTypeBytes.b[i] = ((unsigned char) loadUnsignedChar(data, position));
        }
        return uTypeBytes.d;
    }

    string readTextFile(string path, string defaultString = "");





    string makeBar(int progress, int maxProgress, int size, char filled = '=', char empty = '-');

    string repeatString(string s, int qty);

    template <typename T>
    void swap(T& a, T& b) {
        T& temp = a;
        b = a;
        a = temp;
    }

    template <typename T>
    bool vectorContains(vector<T> v, T c) {
        for (T i : v) {
            if (c == i) {
                return true;
            }
        }
        return false;
    }

    struct ParseException {

        ParseException(string reason) {
            this->reason = reason;
        }
        string reason;
    };

    int getNumberFromSymbol(char symbol);

    char getSymbolFromNumber(int number);

    //string toString(int num, int base = 10);

    double parseDouble(string s, int base = 10, double defaultIfException = 0);

    double parseDoubleUnstable(string s, int base = 10);

    int parseInt(string s, int base = 10, int defaultIfException = 0);

    int parseIntUnstable(string s, int base = 10);

    string intToRomanNumerals(int a);

    void executeBorder(Point2 min, Point2 max, function<void(int, int)> process);

    void executeGrid(Point2 min, Point2 max, function<void(int, int)> process);

    void executeOval(Point2 center, Point2 radii, function<void(int, int)> process);

    void execute4Around(int x, int y, function<void(int, int)> process);

    void spreadUnexploredTileExecute(Level* level, Point2 pos, int tileIndex, function<void(int, int)> process);

    void spreadUnexploredTileExecute_SUB_ORDER(Level* level, Point2 pos, int tileIndex, function<void(int, int)> process, vector<vector<char>>& grid);

    vector<Point2> plotLine(Point2 a, Point2 b);

    string capitalize(string s);
}


#endif /* defined(__Underneath__Utility__) */
