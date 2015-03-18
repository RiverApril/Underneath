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


    void initEnemies(){

        WeightedEnemy* goblinScout = atl(new WeightedEnemy(100, "Goblin Scout", 'g', aiAttackPlayer, 50, ItemGenerator::wKnife, Ui::C_LIGHT_GREEN, 1.0));
        goblinScout->weaknesses.push_back(Weakness(damPoison, 2));

        WeightedEnemy* goblinWarrier = atl(new WeightedEnemy(50, "Goblin Warrier", 'w', aiAttackPlayer, 75, ItemGenerator::wSword, Ui::C_DARK_GREEN, 1.0));
        goblinWarrier->weaknesses.push_back(Weakness(damPoison, 2));

        WeightedEnemy* goblinArcher = atl(new WeightedEnemy(50, "Goblin Archer", 'a', aiAttackPlayer, 25, ItemGenerator::wBow, Ui::C_LIGHT_GREEN, 1.0));
        goblinArcher->weaknesses.push_back(Weakness(damPoison, 2));

        WeightedEnemy* troll = atl(new WeightedEnemy(20, "Troll", 't', aiAttackPlayer, 100, ItemGenerator::wMace, Ui::C_LIGHT_RED, 1.5, 1));
        troll->weaknesses.push_back(Weakness(damFire, 4));

        WeightedEnemy* wraith = atl(new WeightedEnemy(2, "Wraith", ' ', aiAttackPlayer, 200, ItemGenerator::wSword, Ui::C_BLACK, 2.0, 2));
        wraith->weaknesses.push_back(Weakness(damFire, 2));
        wraith->weaknesses.push_back(Weakness(damIce, 2));
        wraith->weaknesses.push_back(Weakness(damShock, 2));
        wraith->weaknesses.push_back(Weakness(damSharp, .5));
        wraith->weaknesses.push_back(Weakness(damBlunt, .5));

        


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
        AiEntity* e = new AiEntity(last->name, last->ai, last->icon, Point2Zero, last->color, last->maxHp);
        e->weaknesses = last->weaknesses;
        e->setActiveWeapon(ItemGenerator::applyRandConditionToWeapon(ItemGenerator::createWeaponFromBase(last->weaponBase, difficulty+last->weaponDifficultyAdd), difficulty/* don't make weapon multiplied because it may drop */));
        return dynamic_cast<AiEntity*>(e);
    }

}

