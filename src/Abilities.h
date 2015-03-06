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

#define iSTR 0
#define iDEX 1
#define iINT 2

#define iAGI 3
#define iSPD 4

#define iCON 5
#define iWIS 6

#define abilityCount 7

namespace AbilityDetails{

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
        "Intelligence",
        "Agility",
        "Speed",
        "Constitution",
        "Wisdom"
    };

    static const string abilityNamesRightAligned[abilityCount] = {
        "     Strength",
        "    Dexterity",
        " Intelligence",
        "      Agility",
        "        Speed",
        " Constitution",
        "       Wisdom"
    };

    static const string abilityInfo[abilityCount] = {
        "Strength: Affects Melee attack damage.",
        "Dexterity: Affects Ranged attack damage.",
        "Intelligence: Affects Magic attack damage.",
        "Agility: Affects Dodge Chance and Attack Speed.",
        "Speed: Affects Movement Speed.",
        "Constitution: Affects Max Health and Regeneration Speed.",
        "Wisdom: Affects Max Mana and Regeneration Speed."
    };




}

using namespace AbilityDetails;

template <typename T>
struct Abilities{

    Abilities(){
        for(int i=0;i<abilityCount;i++){
            list[i] = 0;
        }
    }

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

    T list[abilityCount];

    /*
    
    T* STR = *(&list+iSTR); //Melee Attack Power
    T* DEX = *(&list+iDEX); //Ranged Attack Power
    T* INT = *(&list+iINT); //Magic Attack Power

    T* AGI = *(&list+iAGI); //Dodge Chance & Attack Speed
    T* SPD = *(&list+iSPD); //Movement Speed

    T* CON = *(&list+iCON); //Health Regeneration Speed
    T* WIS = *(&list+iWIS); //Mana Regeneration Speed

    */

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

    T operator[](size_t index){
        return list[index];
    }
};

#endif /* defined(__Underneath__Abilities__) */
