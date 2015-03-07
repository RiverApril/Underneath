//
//  ItemGenerator.h
//  Underneath
//
//  Created by Braeden Atlee on 1/25/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemGenerator__
#define __Underneath__ItemGenerator__


#include "Spell.h"


namespace ItemGenerator {

    struct WeaponBase{

        WeaponBase(){

        }

        WeaponBase(vector<string> names, double damage, double useDelay, DamageType damageType){
            this->names = names;
            this->damage = damage;
            this->useDelay = useDelay;
            this->damageType = damageType;
        }

        WeaponBase ranged(double range){
            this->range = range;
            return *this;
        }

        WeaponBase magical(double range, double manaCost){
            this->range = range;
            this->manaCost = manaCost;
            return *this;
        }

        WeaponBase setWeight(double weight){
            this->weight = weight;
            return *this;
        }

        double damage = 0;
        double useDelay = 0;
        double weight = 0;
        vector<string> names = {""};
        DamageType damageType = damSharp;
        double range = -1;
        double manaCost = -1;
    };

    WeaponBase atl(WeaponBase w);

    extern vector<WeaponBase> weaponList;

    extern Item* iCoin;

    extern WeaponBase wKnife;
    extern WeaponBase wSword;
    extern WeaponBase wAxe;
    extern WeaponBase wMace;
    extern WeaponBase wSpear;

    extern WeaponBase wBow;
    extern WeaponBase wCrossbow;

    extern WeaponBase wFireSpell;
    extern WeaponBase wFrostSpell;
    extern WeaponBase wShockSpell;


    vector<Item*> createRandLoots(int difficulty);

    Weapon* createWeaponRand(int itemDifficulty);

    WeaponBase getRandWeaponBase(DamageType d);
    WeaponBase getRandWeaponBase();

    Weapon* createWeapon(WeaponBase);
    

    void initItemTemplates();
    void cleanupItemTemplates();


}


#endif /* defined(__Underneath__ItemGenerator__) */
