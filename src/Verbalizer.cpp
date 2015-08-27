//
//  Verbalizer.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/27/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Verbalizer.hpp"
#include "Menu.hpp"

namespace Verbalizer {

    using namespace Ui;

    void weakness(EntityAlive* underAttack, Weakness w, double damageWithoutChange, double damageWithChange){

        double damage = abs(damageWithChange - damageWithoutChange);

        EntityPlayer* playerUnderAttack = dynamic_cast<EntityPlayer*>(underAttack);



        string entityIs = "The " + Ui::colorCode(C_LIGHT_BLUE) + underAttack->getName() + Ui::colorCode(C_WHITE) + " is";
        string weakOrResistant = (w.multiplier>1?"weak":"resistant");

        if(playerUnderAttack){
			entityIs = Ui::colorCode(C_LIGHT_BLUE) + "You" + Ui::colorCode(C_WHITE) + " are";
        }

        string verbal = entityIs + " " + weakOrResistant + " to %s, " + Ui::colorCode(C_LIGHT_GREEN) + "%.2f" + Ui::colorCode(C_WHITE) + " damage was%sdealt.";

        consolef(verbal.c_str(), damageTypeName(w.damageType).c_str(), damage, w.multiplier>1?" ":" not ");

    }

    void attack(EntityAlive* attacker, EntityAlive* underAttack, ItemWeapon* weapon, double damage) {

        EntityPlayer* playerAttacker = dynamic_cast<EntityPlayer*> (attacker);
        EntityPlayer* playerUnderAttack = dynamic_cast<EntityPlayer*> (underAttack);

        string attackerString = Ui::colorCode(C_LIGHT_BLUE) + attacker->getName();
        string underAttackString = Ui::colorCode(C_LIGHT_BLUE) + underAttack->getName();
        string deals = "deals";
        string their = "their";
        string theSpace = "The ";

        if (playerAttacker) {
            attackerString = Ui::colorCode(C_LIGHT_BLUE) + "You";
            deals = "deal";
            their = "your";
            theSpace = "";
            consolef(" ");
        }

        if (playerUnderAttack) {
            underAttackString = Ui::colorCode(C_LIGHT_BLUE) + "you";
        }
        string verbal = theSpace + attackerString +
                Ui::colorCode(C_WHITE) + " " + deals +
                Ui::colorCode(C_LIGHT_GREEN) + " %.2f " +
                Ui::colorCode(C_WHITE) + "damage to " + underAttackString +
                Ui::colorCode(C_WHITE) + " with " + their + " " +
                Ui::colorCode(C_LIGHT_RED) + weapon->name;

        consolef(verbal, damage);


    }

    void defeatedEnemy(EntityAlive* dead, int xpGained) {
        consolef("You have deafeated the %s%s%s and gained %s%d xp%s.",
                Ui::colorCode(C_LIGHT_BLUE).c_str(),
                dead->getName().c_str(),
                Ui::colorCode(C_WHITE).c_str(),
                Ui::colorCode(C_LIGHT_GREEN).c_str(),
                xpGained,
                Ui::colorCode(C_WHITE).c_str());
    }

    void leveledUp(EntityPlayer* player) {
        consolef("%sLevel Up!%s You are now level %s%d%s.",
                Ui::colorCode(C_LIGHT_GREEN).c_str(),
                Ui::colorCode(C_WHITE).c_str(),
                Ui::colorCode(C_LIGHT_GREEN).c_str(),
                player->level,
                Ui::colorCode(C_WHITE).c_str());
    }

}
