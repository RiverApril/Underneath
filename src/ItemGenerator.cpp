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

	
    vector<Weapon*> weaponList;

    //DamageMultiplier, Name, Weight, Use Delay
    //DamageMultiplier, Name, Weight, Use Delay, Range
    //DamageMultiplier, Name, Weight, Mana Cost, Use Delay, Range

    Item* iCoin;

    Weapon* wKnife;
    Weapon* wShortSword;
    Weapon* wLongSword;
    Weapon* wBattleAxe;
    Weapon* wMase;
    Weapon* wSpear;

    Ranged* wRecurveBow;
    Ranged* wLongbow;
    Ranged* wCrossbow;

    Spell* wCombatSpell;

    void initItemTemplates(){

        iCoin = new Item("Coin", .01);

        wKnife = atl(new Weapon(materialNone, .5, "Knife", 1, .5));
        wShortSword = atl(new Weapon(materialNone, 1, "Short Sword", 1, 1));
        wLongSword = atl(new Weapon(materialNone, 1.5, "Long Sword", 1.5, 1.5));
        wBattleAxe = atl(new Weapon(materialNone, 1.2, "Battle Axe", 1.2, 1.2));
        wMase = atl(new Weapon(materialNone, 1.4, "Mase", 1.4, 1.4));
        wSpear = setDamageType(atl(new Ranged(materialNone, 1, "Spear", 2, 1.5, 1.8)), damMelee);

        wRecurveBow = dynamic_cast<Ranged*>(atl(new Ranged(materialNone, .5, "Recurve Bow", 1.5, 1, 8)));
        wLongbow = dynamic_cast<Ranged*>(atl(new Ranged(materialNone, .4, "Longbow", 2, .8, 10)));
        wCrossbow = dynamic_cast<Ranged*>(atl(new Ranged(materialNone, .3, "Crossbow", 2, .6, 6)));


        wCombatSpell = dynamic_cast<Spell*>(atl(new Spell(1, "Combat Spell", .1, 6, .2, 10)));
    }

    void cleanupItemTemplates(){
        for(Weapon* w : weaponList){
            delete w;
        }
    }



    Weapon* atl(Weapon* w){//add to list
        weaponList.push_back(w);
        return w;
    }

    Weapon* setDamageType(Weapon* w, DamageType d){
        w->damageType = d;
        return w;
    }

    Weapon* createWeaponBase(DamageType d){
        Weapon* w = nullptr;
        while(w == nullptr){
            size_t i = (rand())%weaponList.size();
            if(weaponList[i]->damageType == d){
                w = weaponList[i];
            }
        }

        return dynamic_cast<Weapon*>(w->clone(w));

    }

    vector<Item*> createLoots(int difficulty){
        vector<Item*> items;

        if(rand()%10!=0){
            Item* item = Item::clone(iCoin);
            item->qty = (rand()%(difficulty*100))+1;
            items.push_back(item);
        }

        int itemDifficulty = 1;

        for(int i=0;i<difficulty;i++){
            if(rand()%10 == 0){
                itemDifficulty++;
            }
        }

        int lootQty = rand()%5+1;

        for(int i = 0; i < lootQty; i++){
            items.push_back(createWeapon(itemDifficulty));
        }
        
        return items;
    }
	
    Weapon* createWeapon(int itemDifficulty){
        DamageType damageType = damMelee;

        if(rand()%2 == 0){
            damageType = damRanged;
        }else if(rand()%4 == 0){
            damageType = damMagic;
        }

        return createWeapon(itemDifficulty, damageType);
    }

    Weapon* createWeapon(int itemDifficulty, DamageType damageType){


        Material* material = materialNone;

        if(damageType != damMagic){
            bool materialExists = false;
            for(Material* m : materialList){
                if(((int)m->getMultipler()) == itemDifficulty){
                    materialExists = true;
                }
            }
            while(materialExists){
                material = materialList[rand()%materialList.size()];
                if(((int)material->getMultipler()) == itemDifficulty){
                    if(damageType == damMelee && material->hasUse(useBlades)){
                        break;
                    }
                    if(damageType == damRanged && material->hasUse(useBows)){
                        break;
                    }
                }
            }
            if(!materialExists){
                material = materialEtherial;
            }
        }

        return createWeapon(material, damageType, rand()%20==0);
    }

    Weapon* createWeapon(Material* material, DamageType damageType, bool enchanted){

        double combatLevelMultiplier = (material->getMultipler()) * 0.5;

        Weapon* weapon = createWeaponBase(damageType);

        weapon->baseDamage *= Random::randDouble(.8, 1.2);
        //weapon->weight *= Random::randDouble(.6, 1.4);
        //weapon->useDelay *= Random::randDouble(.9, 1.1);

        weapon->material = material;
        weapon->baseDamage *= combatLevelMultiplier;
        weapon->weight *= combatLevelMultiplier;

        if(weapon->damageType == damRanged){
            Ranged* ranged = dynamic_cast<Ranged*>(weapon);
            if(ranged){
            	ranged->range *= combatLevelMultiplier;
            }
        }
        if(weapon->damageType == damMagic){
            Spell* spell = dynamic_cast<Spell*>(weapon);
            if(spell){
                spell->manaCost *= combatLevelMultiplier;
            }
        }

        if(enchanted){
            int eid = rand()%2==0?enchBleed:enchFire;
            weapon->addEnchantment(eid, 10, 1);
        }
        
        return weapon;
    }


}
