//
//  EnemyGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/4/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "EnemyGenerator.h"


namespace EnemyGenerator{

    vector<WeightedEnemy> enemyWeightList;

    int total = 0;


    void initEnemies(){

        atl(WeightedEnemy(100, "Goblin Scout", 'g', aiAttackPlayer, 50, ItemGenerator::wKnife, Ui::C_LIGHT_GREEN, 1.0));

        atl(WeightedEnemy(50, "Goblin Warrier", 'w', aiAttackPlayer, 75, ItemGenerator::wSword, Ui::C_DARK_GREEN, 1.0));

        atl(WeightedEnemy(50, "Goblin Archer", 'a', aiAttackPlayer, 25, ItemGenerator::wBow, Ui::C_LIGHT_GREEN, 1.0));

        atl(WeightedEnemy(20, "Troll", 't', aiAttackPlayer, 100, ItemGenerator::wMace, Ui::C_LIGHT_RED, 1.5, 1));

        atl(WeightedEnemy(2, "Wraith", ' ', aiAttackPlayer, 200, ItemGenerator::wSword, Ui::C_BLACK, 2.0, 2));

        


    }

    void cleanupEnemies(){

    }

    void atl(WeightedEnemy we){
        enemyWeightList.push_back(we);
    }

    void setIntervals(int difficulty){
        total = 0;
        for(int i=0;i<enemyWeightList.size();i++){
            total += (&enemyWeightList[i])->weight * ((&enemyWeightList[i])->difficultyWeightMulti * (difficulty+1));
            (&enemyWeightList[i])->interval = total;
        }
    }

    AiEntity* makeRandomEntity(int difficulty){
        
        int r = rand()%total;
        WeightedEnemy last = enemyWeightList[0];
        for(WeightedEnemy we : enemyWeightList){
            if(we.interval > r){
                last = we;
                break;
            }
            debugf("r: %d  total: %d  we's interval: %d", r, total, we.interval);
        }
        AiEntity* e = new AiEntity(last.name, last.ai, last.icon, Point2Zero, last.color, last.maxHp);
        e->setActiveWeapon(ItemGenerator::createWeaponFromBase(last.weaponBase, difficulty+last.weaponDifficultyAdd));
        return dynamic_cast<AiEntity*>(e);
    }

}

