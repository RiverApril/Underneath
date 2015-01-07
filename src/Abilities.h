//
//  Abilities.h
//  Underneath
//
//  Created by Braeden Atlee on 12/26/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Abilities__
#define __Underneath__Abilities__

#include "Global.h"
#include "Utility.h"

namespace AbilityDetails{

    static const int iSTR = 0;
    static const int iDEX = 1;
    static const int iINT = 2;

    static const int iAGI = 3;
    static const int iSPD = 4;

    static const int iCON = 5;
    static const int iWIS = 6;

    static const int abilityCount = 7;

    static const string abilityAbr[abilityCount] = {
        "STR",
        "DEX",
        "INT",
        "AGI",
        "SPD",
        "CON",
        "WIS"
    };

    static const string abilityNames[abilityCount] = {
        "Strength",
        "Dexterity",
        "Inteligence",
        "Agility",
        "Speed",
        "Constitution",
        "Wisdom"
    };

    static const string abilityNamesRightAligned[abilityCount] = {
        "    Strength",
        "   Dexterity",
        " Inteligence",
        "     Agility",
        "       Speed",
        "Constitution",
        "      Wisdom"
    };

    static const string abilityInfo[abilityCount] = {
        "Strength: Affects Melee attack damage.",
        "Dexterity: Affects Ranged attack damage.",
        "Inteligence: Affects Magic attack damage.",
        "Agility: Affects Dodge Chance and Attack Speed.",
        "Speed: Affects Movement Speed.",
        "Constitution: Affects Health Regeneration Speed.",
        "Wisdom: Affects Mana Regeneration Speed."
    };




}

using namespace AbilityDetails;

template <typename T>
struct Abilities{

    Abilities(){}

    Abilities(T a[abilityCount]){
        list[iSTR] = a[iSTR];
        list[iDEX] = a[iDEX];
        list[iINT] = a[iINT];
        list[iAGI] = a[iAGI];
        list[iSPD] = a[iSPD];
        list[iCON] = a[iCON];
        list[iWIS] = a[iWIS];
    }
    
    Abilities(T str, T dex, T inte, T agi, T spd, T con, T wis){
        list[iSTR] = str;
        list[iDEX] = dex;
        list[iINT] = inte;
        list[iAGI] = agi;
        list[iSPD] = spd;
        list[iCON] = con;
        list[iWIS] = wis;
    }

    T list[abilityCount] = {0};

    T* STR = &list[iSTR]; //Melee Attack Power
    T* DEX = &list[iDEX]; //Ranged Attack Power
    T* INT = &list[iINT]; //Magic Attack Power

    T* AGI = &list[iAGI]; //Dodge Chance & Attack Speed
    T* SPD = &list[iSPD]; //Movement Speed

    T* CON = &list[iCON]; //Health Regeneration Speed
    T* WIS = &list[iWIS]; //Mana Regeneration Speed

    void save(vector<unsigned char>* data){
        for(int i=0;i<abilityCount;i++){
            FileUtility::saveType<T>(data, list[i]);
        }
    }

    void load(unsigned char* data, int* position){
        for(int i=0;i<abilityCount;i++){
            list[i] = FileUtility::loadType<T>(data, position);
        }
    }
};

#endif /* defined(__Underneath__Abilities__) */
