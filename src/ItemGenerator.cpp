//
//  ItemGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 1/25/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Ui.h"
#include "ItemGenerator.h"

namespace ItemGenerator {


    vector<CombatLevel> combatLevelList;
    vector<Weapon*> weaponList;

    CombatLevel combat0Abysmal = atl(CombatLevel(0, "Abysmal"));
    CombatLevel combat1Shitty = atl(CombatLevel(.5, "Shitty"));
    CombatLevel combat2Training = atl(CombatLevel(1, "Training"));
    CombatLevel combat3Basic = atl(CombatLevel(1.5, "Basic"));
    CombatLevel combat4Apprentice = atl(CombatLevel(2, "Apprentice"));
    CombatLevel combat5Skillful = atl(CombatLevel(2.5, "Skillful"));
    CombatLevel combat6Good = atl(CombatLevel(3, "Good"));

    //DamageMultiplier, Name, Weight, Use Delay
    //DamageMultiplier, Name, Weight, Use Delay, Range

    Weapon* wKnife = atl(new Weapon(.5, "Knife", 1, .5));
    Weapon* wShortSword = atl(new Weapon(1, "Short Sword", 1, 1));
    Weapon* wLongSword = atl(new Weapon(1.5, "Long Sword", 1.5, 1.5));
    Weapon* wBattleAxe = atl(new Weapon(1.2, "Battle Axe", 1.2, 1.2));
    Weapon* wMase = atl(new Weapon(1.4, "Mase", 1.4, 1.4));
    Weapon* wSpear = setDamageType(atl(new Ranged(1, "Spear", 2, 1.5, 1.8)), damMelee);

    Ranged* wRecurveBow = dynamic_cast<Ranged*>(atl(new Ranged(2, "Recurve Bow", 1.8, 1.8, 8)));
    Ranged* wLongbow = dynamic_cast<Ranged*>(atl(new Ranged(2, "Longbow", 2, 1.5, 10)));
    Ranged* wCrossbow = dynamic_cast<Ranged*>(atl(new Ranged(1.5, "Crossbow", 2, 1, 6)));



    Weapon* atl(Weapon* w){//add to list
        weaponList.push_back(w);
        return w;
    }
    CombatLevel atl(CombatLevel c){//add to list
        combatLevelList.push_back(c);
        return c;
    }

    Weapon* setDamageType(Weapon* w, DamageType d){
        w->damageType = d;
        return w;
    }

    Weapon* createWeaponBase(DamageType d){
        Weapon* w = nullptr;
        while(w == nullptr){
            int i = rand()%weaponList.size();
            if(weaponList[i]->damageType == d){
                w = weaponList[i];
            }
        }

        return dynamic_cast<Weapon*>(w->clone(w));

    }

    Weapon* createWeapon(string name, CombatLevel combatLevel, DamageType damageType, bool enchanted){

        Weapon* weapon = createWeaponBase(damageType);
        weapon->name = (name.size()>0?(name + " "):"") + combatLevel.name + " " + weapon->name;
        weapon->baseDamage *= combatLevel.multiplier;
        weapon->weight *= combatLevel.multiplier;
        if(weapon->damageType == damRanged){
            Ranged* ranged = dynamic_cast<Ranged*>(weapon);
            if(ranged){
                debug("Multiplied range");
            	ranged->range *= combatLevel.multiplier;
            }
        }

        if(enchanted){
			//TODO
        }
        
        return weapon;
    }


}
