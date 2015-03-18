//
//  EnemyGenerator.h
//  Underneath
//
//  Created by Braeden Atlee on 3/4/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__EnemyGenerator__
#define __Underneath__EnemyGenerator__

#include "ItemGenerator.h"
#include "AiEntity.h"

namespace EnemyGenerator{

    struct WeightedEnemy{
        WeightedEnemy(int weight, string name, char icon, AiType ai, int maxHp, ItemGenerator::WeaponBase weaponBase, Ui::Color color, double difficultyWeightMulti, int weaponDifficultyAdd = 0){
            this->weight = weight;
            this->name = name;
            this->icon = icon;
            this->ai = ai;
            this->maxHp = maxHp;
            this->weaponBase = weaponBase;
            this->color = color;
            this->difficultyWeightMulti = difficultyWeightMulti;

        }
        string name = "";
        char icon = '?';
        AiType ai = 0;
        int maxHp = 10;
        int color = Ui::C_LIGHT_RED;
        ItemGenerator::WeaponBase weaponBase;
        int weaponDifficultyAdd = 0;
        double difficultyWeightMulti = 1;
        vector<Weakness> weaknesses;

        int weight = 0;
        int interval = 0;
    };

    extern vector<WeightedEnemy*> enemyWeightList;

    extern int total;
	
    void initEnemies();

    void cleanupEnemies();

    WeightedEnemy* atl(WeightedEnemy* we);

    void setIntervals(int difficulty);

    AiEntity* makeRandomEntity(int difficulty);
    
}

#endif /* defined(__Underneath__EnemyGenerator__) */
