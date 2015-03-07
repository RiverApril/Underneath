//
//  ItemGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 1/25/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Ui.h"
#include "Random.h"
#include "ItemGenerator.h"

namespace ItemGenerator {

    Item* iCoin;

	
    vector<WeaponBase> weaponList;

    WeaponBase wKnife;
    WeaponBase wSword;
    WeaponBase wAxe;
    WeaponBase wMace;
    WeaponBase wSpear;

    WeaponBase wBow;
    WeaponBase wCrossbow;

    WeaponBase wFireSpell;
    WeaponBase wFrostSpell;
    WeaponBase wShockSpell;

    void initItemTemplates(){

        iCoin = new Item("Coin", .01);


        wKnife = atl(WeaponBase({"Knife", "Dagger", "Cleaver"}, 0.25, 0.5, damSharp).setWeight(1));
        wSword = atl(WeaponBase({"Longsword", "Cutlass", "Katana", "Machete", "Gladius", "Scimitar", "Rapier", "Shortsword", "Broadsword", "Saber", "Claymore"}, 1, 1, damSharp).setWeight(2));
        wAxe = atl(WeaponBase({"Axe", "Hatchet", "Battleaxe"}, 1.2, 1.2, damSharp));
        wMace = atl(WeaponBase({"Mace", "Flail", "Hammer"}, 1.5, 1.5, damBlunt));
        wSpear = atl(WeaponBase({"Spear", "Halberd"}, 1.3, 1.3, damPierce).ranged(1.8));

        wBow = atl(WeaponBase({"Longbow", "Shortbow", "Recurve Bow"}, 1, 1, damPierce).ranged(20));
        wCrossbow = atl(WeaponBase({"Crossbow", "Scorpion", ""}, 1.2, 0.8, damPierce).ranged(10));

        wFireSpell = atl(WeaponBase({"Ignite", "Scorch", "Burn"}, 1, .1, damFire).magical(8, 5));

        wFrostSpell = atl(WeaponBase({"Freeze", "Chill"}, 1, .1, damIce).magical(8, 5));

        wShockSpell = atl(WeaponBase({"Electrocute", "Shock", "Zap"}, 1, .1, damIce).magical(8, 5));


    }

    void cleanupItemTemplates(){
        delete iCoin;
    }

    WeaponBase atl(WeaponBase w){//add to list
        weaponList.push_back(w);
        return w;
    }

    WeaponBase getRandWeaponBase(DamageType d){
        while(true){
            size_t i = (rand())%weaponList.size();
            if(weaponList[i].damageType == d){
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
            items.push_back(createWeaponRand(itemDifficulty));
        }
        
        return items;
    }

    Weapon* createWeapon(WeaponBase base){
        size_t ni = rand()%base.names.size();
        string name = base.names[ni];

        Weapon* w;

        if(base.range != -1){
            if(base.manaCost != -1){
                w = new Spell();
                ((Spell*)w)->manaCost = base.manaCost;
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

        return w;
    }

    Weapon* createWeaponRand(int itemDifficulty){
        return createWeapon(getRandWeaponBase());
    }


}
