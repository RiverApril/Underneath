//
//  Verbalizer.cpp
//  Underneath
//
//  Created by Emily Atlee on 2/27/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#include "Verbalizer.hpp"
#include "Menu.hpp"

namespace Verbalizer {

    using namespace Ui;

    string colorCode(const unsigned char fg, const unsigned char bg = C_BLACK){
        return string("&")+Ui::cc(fg, bg);
    }

    void weakness(EntityAlive* underAttack, Weakness w, double damageWithoutChange, double damageWithChange){

        double damage = abs(damageWithChange - damageWithoutChange);

        EntityPlayer* playerUnderAttack = dynamic_cast<EntityPlayer*>(underAttack);



        string entityIs = "The " + colorCode(C_LIGHT_BLUE) + underAttack->getName() + colorCode(C_WHITE) + " is";
        string weakOrResistant = (w.multiplier>1?"weak":"resistant");

        if(playerUnderAttack){
			entityIs = colorCode(C_LIGHT_BLUE) + "You" + colorCode(C_WHITE) + " are";
        }

        string verbal = entityIs + " " + weakOrResistant + " to %s, " + colorCode(C_LIGHT_GREEN) + "%.2f" + colorCode(C_WHITE) + "%sdamage was%sdealt.";

        consolef(verbal.c_str(), damageTypeName(w.damageType).c_str(), damage, w.multiplier>1?" more ":" ", w.multiplier>1?" ":" not ");

    }

    void attack(EntityAlive* attacker, EntityAlive* underAttack, ItemWeapon* weapon, double damage) {
        
        if(attacker == underAttack){
            return;
        }

        //You deal 2.56 damage to the Golbin Archer
        //The Golbin Archer dealt 2.65 damage to you.

		EntityPlayer* playerAttacker = dynamic_cast<EntityPlayer*> (attacker);

        if(playerAttacker){
            consolef("You deal %s%.2f damage%s to the %s%s%s.", colorCode(C_LIGHT_GREEN).c_str(), damage, colorCode(C_WHITE).c_str(), colorCode(C_LIGHT_BLUE).c_str(), underAttack->getName().c_str(), colorCode(C_WHITE).c_str());
        }else{
            consolef("The %s%s%s dealt %s%.2f damage%s to you.", colorCode(C_LIGHT_BLUE).c_str(), attacker->getName().c_str(), colorCode(C_WHITE).c_str(), colorCode(C_LIGHT_RED).c_str(), damage, colorCode(C_WHITE).c_str());
        }

        /*EntityPlayer* playerAttacker = dynamic_cast<EntityPlayer*> (attacker);
        EntityPlayer* playerUnderAttack = dynamic_cast<EntityPlayer*> (underAttack);

        string attackerString = colorCode(C_LIGHT_BLUE) + attacker->getName();
        string underAttackString = "the " + colorCode(C_LIGHT_BLUE) + underAttack->getName();
        string deals = "deals";
        string their = "their";
        string theSpace = "The ";

        if (playerAttacker) {
            attackerString = colorCode(C_LIGHT_BLUE) + "You";
            deals = "deal";
            their = "your";
            theSpace = "";
            consolef(" ");
        }

        if (playerUnderAttack) {
            underAttackString = colorCode(C_LIGHT_BLUE) + "you";
        }
        string verbal = theSpace + attackerString +
                colorCode(C_WHITE) + " " + deals +
                colorCode(C_LIGHT_GREEN) + " %.2f " +
                colorCode(C_WHITE) + "damage to " + underAttackString +
                colorCode(C_WHITE) + " with " + their + " " +
                colorCode(C_LIGHT_RED) + weapon->getName(false);

        consolef(verbal, damage);*/


    }

    void defeatedEnemy(EntityAlive* dead, int xpGained) {
        consolef("You have defeated the %s%s%s and gained %s%d xp%s.",
                colorCode(C_LIGHT_BLUE).c_str(),
                dead->getName().c_str(),
                colorCode(C_WHITE).c_str(),
                colorCode(C_LIGHT_GREEN).c_str(),
                xpGained,
                colorCode(C_WHITE).c_str());
    }

    void leveledUp(EntityPlayer* player) {
        consolef("%sLevel Up!%s You are now level %s%d%s.",
                colorCode(C_LIGHT_GREEN).c_str(),
                colorCode(C_WHITE).c_str(),
                colorCode(C_LIGHT_GREEN).c_str(),
                player->level,
                colorCode(C_WHITE).c_str());
    }

}
