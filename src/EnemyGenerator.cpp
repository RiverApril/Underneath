//
//  EnemyGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/4/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "EnemyGenerator.h"
#include "ItemGenerator.h"


namespace EnemyGenerator{

    vector<WeightedEnemy> enemyWeightList;

    int total = 0;

    AiEntity* eGoblinScout;
    AiEntity* eGoblinWarrior;


    void initEnemies(){

        eGoblinScout = new AiEntity ("Goblin Scout", aiAttackPlayer, 'g', Point2Zero, Ui::C_LIGHT_GREEN, 50);
        eGoblinScout->setActiveWeapon(ItemGenerator::createWeaponFromBase(ItemGenerator::wKnife, 0));
        eGoblinScout->setMoveDelay(.5);

        eGoblinWarrior = new AiEntity ("Goblin Warrior", aiAttackPlayer, 'g', Point2Zero, Ui::C_DARK_GREEN, 75);
        eGoblinWarrior->setActiveWeapon(ItemGenerator::createWeaponFromBase(ItemGenerator::wSword, 0));
        eGoblinWarrior->setMoveDelay(.75);

        atl(eGoblinScout, 10);
        atl(eGoblinWarrior, 20);

        total = 0;
        for(WeightedEnemy we : enemyWeightList){
            we.interval = total;
            total += we.weight;
        }

    }

    void cleanupEnemies(){
        for(WeightedEnemy we : enemyWeightList){
            delete we.e;
        }
    }

    void atl(AiEntity* e, int weight){
        enemyWeightList.push_back(WeightedEnemy(e, weight));
    }

    AiEntity* makeRandomEntity(){
        int r = rand()%total;
        WeightedEnemy last = enemyWeightList[0];
        for(WeightedEnemy we : enemyWeightList){
            if(we.interval > r){
                return last.e;
            }
            last = we;
        }
        return dynamic_cast<AiEntity*>(Entity::clone(enemyWeightList[0].e));
    }

}

