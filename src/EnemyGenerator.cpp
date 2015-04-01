//
//  EnemyGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/4/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "EnemyGenerator.h"


namespace EnemyGenerator{

    vector<WeightedEnemy*> enemyWeightList;

    int total = 0;

    WeightedEnemy* goblinScout;
    WeightedEnemy* goblinWarrier;
    WeightedEnemy* goblinArcher;
    WeightedEnemy* troll;
    WeightedEnemy* wraith;
    WeightedEnemy* mimic;


    void initEnemies(){

        goblinScout = atl(new WeightedEnemy(100, "Goblin Scout", 'g', aiAttackPlayer, 50, ItemGenerator::wKnife, "", Ui::C_LIGHT_GREEN, 1.0));
        goblinScout->weaknesses.push_back(Weakness(damPoison, 2));

        goblinWarrier = atl(new WeightedEnemy(50, "Goblin Warrier", 'w', aiAttackPlayer, 75, ItemGenerator::wSword, "", Ui::C_DARK_GREEN, 1.0));
        goblinWarrier->weaknesses.push_back(Weakness(damPoison, 2));

        goblinArcher = atl(new WeightedEnemy(50, "Goblin Archer", 'a', aiAttackPlayer, 25, ItemGenerator::wBow, "", Ui::C_LIGHT_GREEN, 1.0));
        goblinArcher->weaknesses.push_back(Weakness(damPoison, 2));

        troll = atl(new WeightedEnemy(20, "Troll", 't', aiAttackPlayer, 100, ItemGenerator::wMace, "", Ui::C_LIGHT_RED, 1.5, 1));
        troll->weaknesses.push_back(Weakness(damFire, 4));

        wraith = atl(new WeightedEnemy(2, "Wraith", ' ', aiAttackPlayer, 200, ItemGenerator::wSword, "", Ui::C_BLACK, 2.0, 2));
        wraith->weaknesses.push_back(Weakness(damFire, 2));
        wraith->weaknesses.push_back(Weakness(damIce, 2));
        wraith->weaknesses.push_back(Weakness(damShock, 2));
        wraith->weaknesses.push_back(Weakness(damSharp, .5));
        wraith->weaknesses.push_back(Weakness(damBlunt, .5));

        mimic = new WeightedEnemy(0, "Mimic", '+', aiAttackPlayer, 75, ItemGenerator::wNatural, "Teeth", Ui::C_LIGHT_GREEN, 0);
        mimic->weaknesses.push_back(Weakness(damFire, 4));




    }

    void cleanupEnemies(){
        for(WeightedEnemy* we : enemyWeightList){
            delete we;
        }
        enemyWeightList.clear();
    }

    WeightedEnemy* atl(WeightedEnemy* we){
        enemyWeightList.push_back(we);
        return we;
    }

    void setIntervals(int difficulty){
        total = 0;
        for(int i=0;i<enemyWeightList.size();i++){
            total += enemyWeightList[i]->weight * enemyWeightList[i]->difficultyWeightMulti * (difficulty+1);
            enemyWeightList[i]->interval = total;
        }
    }

    AiEntity* makeEntity(WeightedEnemy* we, int difficulty){
        AiEntity* e = new AiEntity(we->name, we->ai, we->icon, Point2Zero, we->color, we->maxHp);
        e->weaknesses = we->weaknesses;
        Weapon* weapon = ItemGenerator::applyRandConditionToWeapon(ItemGenerator::createWeaponFromBase(we->weaponBase, difficulty+we->weaponDifficultyAdd), difficulty);
        if(we->weaponName.size() > 0){
            weapon->name = we->weaponName;
        }
        e->setActiveWeapon(weapon);
        return e;
    }

    AiEntity* makeRandomEntity(int difficulty){

        int r = rand()%total;
        WeightedEnemy* last = enemyWeightList[0];
        for(WeightedEnemy* we : enemyWeightList){
            if(we->interval > r){
                last = we;
                break;
            }
            debugf("r: %d  total: %d  we's interval: %d", r, total, we->interval);
        }
        return makeEntity(last, difficulty);
    }

}
