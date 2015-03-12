//
//  ItemGenerator.h
//  Underneath
//
//  Created by Braeden Atlee on 1/25/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemGenerator__
#define __Underneath__ItemGenerator__


#include "CombatSpell.h"


namespace ItemGenerator {

    struct Condition{
        
        Condition(){}

        Condition(vector<string> names, double damMin, double damMax, double udMin, double udMax, double wMin, double wMax, vector<WeaponType> weaponTypes){
            this->names = names;
            this->damage.x = damMin;
            this->damage.y = damMax;
            this->useDelay.x = udMin;
            this->useDelay.y = udMax;
            this->weight.x = wMin;
            this->weight.y = wMax;
            this->applicableWeaponTypes = weaponTypes;
        }
        vector<string> names = {""};
        Vector2 damage;
        Vector2 useDelay;
        Vector2 weight;
        vector<WeaponType> applicableWeaponTypes;
    };

    struct WeaponBase{

        WeaponBase(){}

        WeaponBase(vector<string> names, double damage, double useDelay, DamageType damageType, WeaponType weaponType){
            this->names = names;
            this->damage = damage;
            this->useDelay = useDelay;
            this->damageType = damageType;
            this->weaponType = weaponType;
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
        double weight = 1;
        vector<string> names = {""};
        DamageType damageType = damSharp;
        WeaponType weaponType = wepMelee;
        double range = -1;
        double manaCost = -1;
    };

    Condition atl(Condition w);
    WeaponBase atl(WeaponBase w);


    extern vector<Condition> conditionList;
    extern vector<WeaponBase> weaponList;

    extern Item* iCoin;

    extern WeaponBase wKnife;
    extern WeaponBase wSword;
    extern WeaponBase wAxe;
    extern WeaponBase wMace;
    extern WeaponBase wSpear;

    extern WeaponBase wBow;
    extern WeaponBase wCrossbow;

    extern WeaponBase wFireCombatSpell;
    extern WeaponBase wFrostCombatSpell;
    extern WeaponBase wShockCombatSpell;


    vector<Item*> createRandLoots(int difficulty);

    Weapon* createRandWeapon(int itemDifficulty);

    WeaponBase getRandWeaponBase(DamageType d);
    WeaponBase getRandWeaponBase();

    Weapon* createWeaponFromBase(WeaponBase base, int itemDifficulty);

    Weapon* createWeaponFromType(WeaponType w, int itemDifficulty);

    Weapon* applyConditionToWeapon(Weapon* w, Condition c, int itemDifficulty, bool prependName = true);

    Weapon* applyRandConditionToWeapon(Weapon * w, int itemDifficulty, bool prependName = true);

    Weapon* createRandWeapon(int itemDifficulty);
    

    void initItemTemplates();
    void cleanupItemTemplates();


}


#endif /* defined(__Underneath__ItemGenerator__) */
