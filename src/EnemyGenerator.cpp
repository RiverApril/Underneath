//
//  EnemyGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/4/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "EnemyGenerator.hpp"


namespace EnemyGenerator {

    vector<EntityBase*> enemyWeightList;

    int total = 0;

    EntityBase* goblinScout;
    EntityBase* goblinWarrier;
    EntityBase* goblinArcher;
    EntityBase* troll;
    EntityBase* wraith;
    EntityBase* slime;
    EntityBase* myconid;

    EntityBase* mimic;
    EntityBase* bunny;

    void initEnemies() {

        goblinScout = atl(new EntityBase(100, "Goblin Scout", 'g', aiAttackAndFleeAtLowHealth, 50, ItemGenerator::wKnife, "", C_LIGHT_GREEN, 1.0));
        goblinScout->weaknesses.push_back(Weakness(damPoison, 2));

        goblinWarrier = atl(new EntityBase(50, "Goblin Warrior", 'w', aiAttack, 75, ItemGenerator::wSword, "", C_DARK_GREEN, 1.0));
        goblinWarrier->weaknesses.push_back(Weakness(damPoison, 2));

        goblinArcher = atl(new EntityBase(50, "Goblin Archer", 'a', aiAttack, 25, ItemGenerator::wBow, "", C_LIGHT_GREEN, 1.0));
        goblinArcher->weaknesses.push_back(Weakness(damPoison, 2));

        troll = atl(new EntityBase(20, "Troll", 't', aiAttack, 100, ItemGenerator::wMace, "", C_LIGHT_RED, 1.5, 1));
        troll->weaknesses.push_back(Weakness(damFire, 4));

        wraith = atl(new EntityBase(2, "Wraith", ' ', aiAttack, 200, ItemGenerator::wSword, "", C_BLACK, 2.0, 2));
        wraith->weaknesses.push_back(Weakness(damFire, 2));
        wraith->weaknesses.push_back(Weakness(damIce, 2));
        wraith->weaknesses.push_back(Weakness(damShock, 2));
        wraith->weaknesses.push_back(Weakness(damSharp, .5));
        wraith->weaknesses.push_back(Weakness(damBlunt, .5));

        slime = atl(new EntityBase(75, "Slime", 's', aiAttack, 40, ItemGenerator::wNatural, "Goo", C_LIGHT_YELLOW, 1.0));
        slime->weaknesses.push_back(Weakness(damSharp, .4));
        slime->weaknesses.push_back(Weakness(damPierce, .4));

        ItemGenerator::WeaponBase myconidWepon = ItemGenerator::wNatural;
        myconidWepon.damage *= .5;
        myconidWepon.enchs.push_back(Enchantment(effLSD, 30, 0, 50));
        myconidWepon.enchs.push_back(Enchantment(effMemory, 10, 0, 100));
        myconid = atl(new EntityBase(20, "Myconid", 'm', aiAttack, 50, myconidWepon, "Finger", C_LIGHT_MAGENTA, 1.0));
        myconid->weaknesses.push_back(Weakness(damFire, 4));



        mimic = new EntityBase(0, "Mimic", '+', aiAttack | aiMoveRandom, 75, ItemGenerator::wNatural, "Teeth", C_LIGHT_GREEN, 0);
        mimic->weaknesses.push_back(Weakness(damFire, 4));


        bunny = new EntityBase(0, "Bunny", 'b', aiFlee | aiMoveRandom, 10, ItemGenerator::wNatural, "Claws", C_LIGHT_WHITE, 0);
        bunny->moveDelay = 1.0;




    }

    void cleanupEnemies() {
        for (EntityBase* we : enemyWeightList) {
            delete we;
        }
        enemyWeightList.clear();
    }

    EntityBase* atl(EntityBase* we) {
        enemyWeightList.push_back(we);
        return we;
    }

    void setIntervals(int difficulty) {
        total = 0;
        for (int i = 0; i < enemyWeightList.size(); i++) {
            total += enemyWeightList[i]->weight * enemyWeightList[i]->difficultyWeightMulti * (difficulty + 1);
            enemyWeightList[i]->interval = total;
        }
    }

    EntityAi* makeEntity(EntityBase* we, int difficulty) {
        EntityAi* e = new EntityAi(we->name, we->ai, we->icon, Point2Zero, we->color, we->maxHp);
        e->weaknesses = we->weaknesses;
        e->moveDelay = we->moveDelay;
        ItemWeapon* weapon = ItemGenerator::applyRandConditionToItemWeapon(ItemGenerator::createItemWeaponFromBase(we->weaponBase, difficulty + we->weaponDifficultyAdd), difficulty);
        if (we->weaponName.size() > 0) {
            weapon->getName(false) = we->weaponName;
        }
        e->setActiveItemWeapon(weapon);
        return e;
    }

    EntityAi* makeRandomEntity(int difficulty) {

        int r = rand() % total;
        EntityBase* last = enemyWeightList[0];
        for (EntityBase* we : enemyWeightList) {
            if (we->interval > r) {
                last = we;
                break;
            }
            //debugf("r: %d  total: %d  we's interval: %d", r, total, we->interval);
        }
        return makeEntity(last, difficulty);
    }

}
