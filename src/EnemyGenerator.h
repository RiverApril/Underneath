//
//  EnemyGenerator.h
//  Underneath
//
//  Created by Braeden Atlee on 3/4/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__EnemyGenerator__
#define __Underneath__EnemyGenerator__

#include "AiEntity.h"

namespace EnemyGenerator{

    struct WeightedEnemy{
        WeightedEnemy(AiEntity* e, int weight){
            this->e = e;
            this->weight = weight;
        }
        AiEntity* e;
        int weight = 0;
        int interval = 0;
    };

    extern vector<WeightedEnemy> enemyWeightList;

    extern int total;

    extern AiEntity* eGoblinScout;
    extern AiEntity* eGoblinWarrior;
	
    void initEnemies();

    void cleanupEnemies();

    void atl(AiEntity* e, int weight);

    AiEntity* makeRandomEntity();
    
}

#endif /* defined(__Underneath__EnemyGenerator__) */
