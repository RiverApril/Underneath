//
//  ItemGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 1/25/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Ui.h"
#include "Random.h"
#include "Utility.h"
#include "ItemGenerator.h"

namespace ItemGenerator {

    Item* iCoin;


    vector<Condition> conditionList;
    vector<WeaponBase> weaponList;

    WeaponBase wKnife;
    WeaponBase wSword;
    WeaponBase wAxe;
    WeaponBase wMace;
    WeaponBase wSpear;

    WeaponBase wBow;
    WeaponBase wCrossbow;

    WeaponBase wFireCombatSpell;
    WeaponBase wFrostCombatSpell;
    WeaponBase wShockCombatSpell;

    void initItemTemplates(){

        iCoin = new Item("Coin", .01);


        wKnife = atl(WeaponBase({"Knife", "Dagger", "Cleaver"}, 0.25, 0.5, damSharp, wepMelee).setWeight(1));
        wSword = atl(WeaponBase({"Longsword", "Cutlass", "Katana", "Machete", "Gladius", "Scimitar", "Rapier", "Shortsword", "Broadsword", "Saber", "Claymore"}, 1, 1, damSharp, wepMelee).setWeight(2));
        wAxe = atl(WeaponBase({"Axe", "Hatchet", "Battleaxe"}, 1.2, 1.2, damSharp, wepMelee).setWeight(2));
        wMace = atl(WeaponBase({"Mace", "Flail", "Hammer"}, 1.5, 1.5, damBlunt, wepMelee));
        wSpear = atl(WeaponBase({"Spear", "Halberd"}, 1.3, 1.3, damPierce, wepMelee).ranged(1.8).setWeight(2.5));

        wBow = atl(WeaponBase({"Longbow", "Shortbow", "Recurve Bow"}, 1, 1, damPierce, wepRanged).ranged(20).setWeight(1.5));
        wCrossbow = atl(WeaponBase({"Crossbow", "Scorpion"}, 1.2, 0.8, damPierce, wepRanged).ranged(10).setWeight(2));

        wFireCombatSpell = atl(WeaponBase({"Ignite", "Scorch", "Burn"}, 1, .1, damFire, wepMagic).magical(8, 5).setWeight(.1));
        wFrostCombatSpell = atl(WeaponBase({"Freeze", "Chill"}, 1, .1, damIce, wepMagic).magical(8, 5).setWeight(.1));
        wShockCombatSpell = atl(WeaponBase({"Electrocute", "Shock", "Zap"}, 1, .1, damShock, wepMagic).magical(8, 5).setWeight(.1));


        Condition cBroken = atl(Condition({"Broken", "Cracked", "Damaged"}, .5, .7, 1, 1, 1, 1, {wepMelee, wepRanged}));
        Condition cNormal = atl(Condition({"Standard", "Normal", "Regular", "Stock"}, 1, 1, 1, 1, 1, 1, {wepMelee, wepRanged}));
        Condition cForged = atl(Condition({"Forged", "Crafted"}, 1.2, 1.4, 1, 1, 1, 1, {wepMelee, wepRanged}));
        Condition cFortified = atl(Condition({"Fortified", "Strengthened", "Reinforced"}, 1.8, 2.2, 1.1, 1.1, 1.3, 1.6, {wepMelee, wepRanged}));
        Condition cHeavy = atl(Condition({"Heavy", "Overweight", "Obese", "Dense"}, 1, 1.1, 1.3, 1.5, 2, 3, {wepMelee, wepRanged}));

        Condition cNormalMagic = atl(Condition({"Normal"}, 1, 1, 1, 1, 1, 1, {wepMagic}));


    }

    void cleanupItemTemplates(){
        delete iCoin;
    }

    WeaponBase atl(WeaponBase w){
        weaponList.push_back(w);
        return w;
    }

    Condition atl(Condition c){
        conditionList.push_back(c);
        return c;
    }

    WeaponBase getRandWeaponBase(const WeaponType w){
        while(true){
            size_t i = (rand())%weaponList.size();
            if(weaponList[i].weaponType == w){
                return weaponList[i];
            }
        }

    }

    WeaponBase getRandWeaponBase(){
        size_t i = (rand())%weaponList.size();
        return weaponList[i];
    }

    vector<Item*> createRandLoots(int difficulty){
        vector<Item*> items;

        if(rand()%10!=0){
            Item* item = Item::clone(iCoin);
            item->qty = (rand()%(difficulty*100))+1;
            items.push_back(item);
        }

        int itemDifficulty = 1;

        for(int i=0;i<difficulty*2;i++){
            if(rand()%10 == 0){
                itemDifficulty++;
            }
        }

        int weaponQty = rand()%5;

		for (int i = 0; i < weaponQty; i++){
            items.push_back(createRandWeapon(itemDifficulty));
        }
        
        return items;
    }

    Weapon* createWeaponFromType(WeaponType w, int itemDifficulty){
        return createWeaponFromBase(getRandWeaponBase(w), itemDifficulty);
    }

    Weapon* createWeaponFromBase(WeaponBase base, int itemDifficulty){
        size_t ni = rand()%base.names.size();
        string name = base.names[ni];

        Weapon* w;

        if(base.range != -1){
            if(base.manaCost != -1){
                w = new CombatSpell();
                ((CombatSpell*)w)->manaCost = base.manaCost;
                w->addEnchantment(Enchantment(effDamage, 10, 1, 6).setMeta(base.damageType));
            }else{
                w = new Ranged();
            }
            ((Ranged*)w)->range = base.range;
        }else{
            w = new Weapon();
        }
        w->baseDamage = base.damage;
        w->useDelay = base.useDelay;
        w->weight = base.weight;
        w->name = name;
        w->damageType = base.damageType;
        w->weaponType = base.weaponType;

        w->baseDamage *= ((itemDifficulty) * .25)+1;
        w->baseDamage *= Random::randDouble(.9, 1.1);

        return w;
    }

    Weapon* applyConditionToWeapon(Weapon* w, Condition c, int itemDifficulty, bool prependName){

        if(prependName){
            w->name = c.names[rand()%c.names.size()] + " " + w->name;
        }

        w->baseDamage *= Random::randDouble(c.damage.x, c.damage.y);
        w->useDelay *= Random::randDouble(c.useDelay.x, c.useDelay.y);
        w->weight *= Random::randDouble(c.weight.x, c.weight.y);

        return w;
    }

    Weapon* applyRandConditionToWeapon(Weapon * w, int itemDifficulty, bool prependName){
        Condition condition;
        do{
            condition = conditionList[rand()%conditionList.size()];
        }while(!Utility::vectorContains(condition.applicableWeaponTypes, w->weaponType));

        applyConditionToWeapon(w, condition, itemDifficulty);
        return w;
    }

    Weapon* createRandWeapon(int itemDifficulty){
        WeaponType wType = Random::choose<WeaponType>(3, wepMelee, wepRanged, wepMagic);
        WeaponBase base = getRandWeaponBase(wType);
        Weapon* w = createWeaponFromBase(base, itemDifficulty);

        applyRandConditionToWeapon(w, itemDifficulty);

        return w;
    }


}
