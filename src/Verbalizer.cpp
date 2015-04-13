//
//  Verbalizer.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/27/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Verbalizer.h"
#include "Menu.h"

namespace Verbalizer {

    using namespace Ui;

    void attack(Alive* attacker, Alive* underAttack, Weapon* weapon, double damage) {

        Player* playerAttacker = dynamic_cast<Player*> (attacker);
        Player* playerUnderAttack = dynamic_cast<Player*> (underAttack);

        string attackerString = Ui::colorCode(C_LIGHT_BLUE) + attacker->getName();
        string underAttackString = Ui::colorCode(C_LIGHT_BLUE) + underAttack->getName();
        string deals = "deals";
        string their = "their";
        string theSpace = "The ";

        if (playerAttacker) {
            attackerString = "You";
            deals = "deal";
            their = "your";
            theSpace = "";
        }

        if (playerUnderAttack) {
            underAttackString = "you";
        }
        string verbal = theSpace + attackerString +
                Ui::colorCode(C_WHITE) + " " + deals +
                Ui::colorCode(C_LIGHT_GREEN) + " %.2f " +
                Ui::colorCode(C_WHITE) + "damage to " + underAttackString +
                Ui::colorCode(C_WHITE) + " with " + their + " " +
                Ui::colorCode(C_LIGHT_RED) + weapon->name;

        consolef(verbal, damage);


    }

    void defeatedEnemy(Alive* dead, int xpGained) {
        consolef("You have deafeated the %s%s%s and gained %s%d xp%s.",
                Ui::colorCode(C_LIGHT_BLUE).c_str(),
                dead->getName().c_str(),
                Ui::colorCode(C_WHITE).c_str(),
                Ui::colorCode(C_LIGHT_GREEN).c_str(),
                xpGained,
                Ui::colorCode(C_WHITE).c_str());
    }

    void leveledUp(Player* player) {
        consolef("%sLevel Up!%s You are now level %s%d%s.",
                Ui::colorCode(C_LIGHT_GREEN).c_str(),
                Ui::colorCode(C_WHITE).c_str(),
                Ui::colorCode(C_LIGHT_GREEN).c_str(),
                player->level,
                Ui::colorCode(C_WHITE).c_str());
    }

}
