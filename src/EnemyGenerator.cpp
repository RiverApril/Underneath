//
//  EnemyGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/4/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "EnemyGenerator.hpp"


namespace EnemyGenerator {

    vector<WeightedEnemy*> enemyWeightList;

    int total = 0;

    WeightedEnemy* goblinScout;
    WeightedEnemy* goblinWarrier;
    WeightedEnemy* goblinArcher;
    WeightedEnemy* troll;
    WeightedEnemy* wraith;
    WeightedEnemy* slime;
    WeightedEnemy* mimic;
    WeightedEnemy* myconid;

    void initEnemies() {

        goblinScout = atl(new WeightedEnemy(100, "Goblin Scout", 'g', aiAttackEntityPlayer, 50, ItemGenerator::wKnife, "", Ui::C_LIGHT_GREEN, 1.0));
        goblinScout->weaknesses.push_back(Weakness(damPoison, 2));

        goblinWarrier = atl(new WeightedEnemy(50, "Goblin Warrior", 'w', aiAttackEntityPlayer, 75, ItemGenerator::wSword, "", Ui::C_DARK_GREEN, 1.0));
        goblinWarrier->weaknesses.push_back(Weakness(damPoison, 2));

        goblinArcher = atl(new WeightedEnemy(50, "Goblin Archer", 'a', aiAttackEntityPlayer, 25, ItemGenerator::wBow, "", Ui::C_LIGHT_GREEN, 1.0));
        goblinArcher->weaknesses.push_back(Weakness(damPoison, 2));

        troll = atl(new WeightedEnemy(20, "Troll", 't', aiAttackEntityPlayer, 100, ItemGenerator::wMace, "", Ui::C_LIGHT_RED, 1.5, 1));
        troll->weaknesses.push_back(Weakness(damFire, 4));

        wraith = atl(new WeightedEnemy(2, "Wraith", ' ', aiAttackEntityPlayer, 200, ItemGenerator::wSword, "", Ui::C_BLACK, 2.0, 2));
        wraith->weaknesses.push_back(Weakness(damFire, 2));
        wraith->weaknesses.push_back(Weakness(damIce, 2));
        wraith->weaknesses.push_back(Weakness(damShock, 2));
        wraith->weaknesses.push_back(Weakness(damSharp, .5));
        wraith->weaknesses.push_back(Weakness(damBlunt, .5));

        slime = atl(new WeightedEnemy(75, "Slime", 's', aiAttackEntityPlayer, 40, ItemGenerator::wNatural, "Goo", Ui::C_LIGHT_YELLOW, 1.0));
        slime->weaknesses.push_back(Weakness(damSharp, .4));
        slime->weaknesses.push_back(Weakness(damPierce, .4));

        ItemGenerator::WeaponBase myconidWepon = ItemGenerator::wNatural;
        myconidWepon.damage *= .5;
        myconidWepon.enchs.push_back(Enchantment(effLSD, 30, 0, 50));
        myconidWepon.enchs.push_back(Enchantment(effMemory, 10, 0, 100));
        myconid = atl(new WeightedEnemy(20, "Myconid", 'm', aiAttackEntityPlayer, 50, myconidWepon, "Finger", Ui::C_LIGHT_MAGENTA, 1.0));
        myconid->weaknesses.push_back(Weakness(damFire, 4));



        mimic = new WeightedEnemy(0, "Mimic", '+', aiAttackEntityPlayer, 75, ItemGenerator::wNatural, "Teeth", Ui::C_LIGHT_GREEN, 0);
        mimic->weaknesses.push_back(Weakness(damFire, 4));




    }

    void cleanupEnemies() {
        for (WeightedEnemy* we : enemyWeightList) {
            delete we;
        }
        enemyWeightList.clear();
    }

    WeightedEnemy* atl(WeightedEnemy* we) {
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

    EntityAi* makeEntity(WeightedEnemy* we, int difficulty) {
        EntityAi* e = new EntityAi(we->name, we->ai, we->icon, Point2Zero, we->color, we->maxHp);
        e->weaknesses = we->weaknesses;
        ItemWeapon* weapon = ItemGenerator::applyRandConditionToItemWeapon(ItemGenerator::createItemWeaponFromBase(we->weaponBase, difficulty + we->weaponDifficultyAdd), difficulty);
        if (we->weaponName.size() > 0) {
            weapon->name = we->weaponName;
        }
        e->setActiveItemWeapon(weapon);
        return e;
    }

    EntityAi* makeRandomEntity(int difficulty) {

        int r = rand() % total;
        WeightedEnemy* last = enemyWeightList[0];
        for (WeightedEnemy* we : enemyWeightList) {
            if (we->interval > r) {
                last = we;
                break;
            }
            //debugf("r: %d  total: %d  we's interval: %d", r, total, we->interval);
        }
        return makeEntity(last, difficulty);
    }

}
