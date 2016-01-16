//
//  Abilities.hpp
//  Underneath
//
//  Created by Braeden Atlee on 12/26/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Abilities__
#define __Underneath__Abilities__

#include "Global.hpp"
#include "Utility.hpp"

#define iSTR 0
#define iDEX 1
#define iINT 2

#define iAGI 3

#define iCON 4
#define iWIS 5

#define iLUK 6

#define abilityCount 7

using namespace std;

static const string abilityAbr[abilityCount] = {
    "STR",
    "DEX",
    "INT",
    "AGI",
    "CON",
    "WIS",
    "LUK"
};

static const string abilityNames[abilityCount] = {
    "Strength",
    "Dexterity",
    "Intelligence",
    "Agility",
    "Constitution",
    "Wisdom",
    "Luck"
};

static const string abilityNamesRightAligned[abilityCount] = {
    "     Strength",
    "    Dexterity",
    " Intelligence",
    "      Agility",
    " Constitution",
    "       Wisdom",
    "         Luck"
};

static const string abilityInfo[abilityCount] = {
    "Melee attack damage.",
    "Ranged attack damage.",
    "Magic attack damage.",
    "Dodge Chance, Attack Speed, and Movement Speed.",
    "Max Health.",
    "Max Mana.",
    "General Luck."
};

template <typename T>
struct Abilities {

    Abilities() {
        for (int i = 0; i < abilityCount; i++) {
            list[i] = 0;
        }
    }

    Abilities(T a) {
        for (int i = 0; i < abilityCount; i++) {
            list[i] = a;
        }
    }

    Abilities(T a[abilityCount]) {
        for (int i = 0; i < abilityCount; i++) {
            list[i] = a[i];
        }
    }

    Abilities(T str, T dex, T inte, T agi, T con, T wis, T luk) {
        list[iSTR] = str;
        list[iDEX] = dex;
        list[iINT] = inte;
        list[iAGI] = agi;
        list[iCON] = con;
        list[iWIS] = wis;
        list[iLUK] = luk;
    }

    T list[abilityCount];

    void save(vector<unsigned char>* data) {
        for (int i = 0; i < abilityCount; i++) {
            Utility::saveType<T>(data, list[i]);
        }
    }

    void load(vector<unsigned char>* data, int* position) {
        for (int i = 0; i < abilityCount; i++) {
            list[i] = Utility::loadType<T>(data, position);
        }
    }

    T operator[](size_t index) {
        return list[index];
    }
};

template <typename T>
bool operator==(const Abilities<T> a, const Abilities<T> b) {
    return a.list == b.list;
}

template <typename T>
bool operator!=(const Abilities<T> a, const Abilities<T> b) {
    return !(a == b);
}

#endif /* defined(__Underneath__Abilities__) */
