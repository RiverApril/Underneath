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
    EntityBase* snake;

    EntityBase* mimic;
    EntityBase* bunny;

    void initEnemies() {

        goblinScout = atl(new EntityBase(100, "Goblin Scout", 'g', aiAttackAndFleeAtLowHealth, 50, ItemGenerator::wKnife, "", C_LIGHT_GREEN, 1.0));
        goblinScout->weaknesses.push_back(Weakness(damPoison, 2));

        goblinWarrier = atl(new EntityBase(50, "Goblin Warrior", 'w', aiAttack, 75, ItemGenerator::wSword, "", C_DARK_GREEN, 1.0));
        goblinWarrier->weaknesses.push_back(Weakness(damPoison, 2));

        goblinArcher = atl(new EntityBase(50, "Goblin Archer", 'a', aiAttackAndFleeAtLowHealth, 25, ItemGenerator::wBow, "", C_LIGHT_GREEN, 1.0));
        goblinArcher->weaknesses.push_back(Weakness(damPoison, 2));
        goblinArcher->weaknesses.push_back(Weakness(damBlunt, 1.25));
        goblinArcher->moveDelay = 1.2;

        ItemGenerator::WeaponBase snakeWeapon = ItemGenerator::wNatural;
        snakeWeapon.damageType = damPierce;
        snakeWeapon.enchs.push_back(ItemGenerator::EnchantmentBase(effDamage, 20, 30, 1, 1, 30, 50, damPoison));
        snake = atl(new EntityBase(20, "Serpant", 's', aiAttack, 75, snakeWeapon, "Fangs", C_LIGHT_BLUE, 1.0));
        snake->attackMultiplier *= .75;

        troll = atl(new EntityBase(20, "Troll", 't', aiAttack, 100, ItemGenerator::wMace, "", C_LIGHT_RED, 1.5, 1));
        troll->weaknesses.push_back(Weakness(damFire, 4));
        troll->moveDelay = 1.8;

        wraith = atl(new EntityBase(2, "Wraith", 'W', aiAttack, 200, ItemGenerator::wSword, "", C_DARK_GRAY, 2.0, 2));
        wraith->weaknesses.push_back(Weakness(damFire, 2));
        wraith->weaknesses.push_back(Weakness(damIce, 2));
        wraith->weaknesses.push_back(Weakness(damShock, 2));
        wraith->weaknesses.push_back(Weakness(damSharp, .5));
        wraith->weaknesses.push_back(Weakness(damBlunt, .5));
        goblinArcher->moveDelay = 1.8;

        slime = atl(new EntityBase(75, "Slime", 's', aiAttack, 40, ItemGenerator::wNatural, "Goo", C_LIGHT_YELLOW, 1.0));
        slime->weaknesses.push_back(Weakness(damSharp, .25));
        slime->weaknesses.push_back(Weakness(damPierce, .25));

        ItemGenerator::WeaponBase myconidWeapon = ItemGenerator::wNatural;
        myconidWeapon.enchs.push_back(ItemGenerator::EnchantmentBase(effLSD, 20, 30, 0, 0, 20, 40));
        myconidWeapon.enchs.push_back(ItemGenerator::EnchantmentBase(effMemory, 20, 30, 0, 0, 50, 100));
        myconid = atl(new EntityBase(20, "Myconid", 'm', aiAttack, 50, myconidWeapon, "Finger", C_LIGHT_MAGENTA, 1.0));
        myconid->weaknesses.push_back(Weakness(damFire, 4));
        myconid->attackMultiplier *= .5;



        mimic = new EntityBase(0, "Mimic", 'M', aiAttack | aiMoveRandom, 75, ItemGenerator::wNatural, "Teeth", C_LIGHT_GREEN, 0);
        mimic->weaknesses.push_back(Weakness(damFire, 4));


        bunny = new EntityBase(0, "Bunny", 'b', aiFlee | aiMoveRandom, 10, ItemGenerator::wNatural, "Claws", C_LIGHT_WHITE, 0);
        bunny->moveDelay = 0.8;




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
        e->attackMultiplier = we->attackMultiplier;
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
