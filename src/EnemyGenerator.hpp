//
//  EnemyGenerator.h
//  Underneath
//
//  Created by Braeden Atlee on 3/4/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__EnemyGenerator__
#define __Underneath__EnemyGenerator__

#include "ItemGenerator.hpp"
#include "EntityAi.hpp"

namespace EnemyGenerator {

    struct EntityBase {

        EntityBase(int weight, string name, char icon, AiType ai, int maxHp, ItemGenerator::WeaponBase* weaponBase, string weaponName, Ui::Color color, double difficultyWeightMulti, int weaponDifficultyAdd = 0) {
            this->weight = weight;
            this->name = name;
            this->icon = icon;
            this->ai = ai;
            this->maxHp = maxHp;
            this->weaponBase = weaponBase;
            this->weaponName = weaponName;
            this->color = color;
            this->difficultyWeightMulti = difficultyWeightMulti;
            this->weaponDifficultyAdd = weaponDifficultyAdd;
        }
        string name = "";
        char icon = '?';
        AiType ai = 0;
        int maxHp = 10;
        int color = C_LIGHT_RED;
        ItemGenerator::WeaponBase* weaponBase;
        string weaponName = "";
        int weaponDifficultyAdd = 0;
        double difficultyWeightMulti = 1;
        vector<Weakness> weaknesses;
        double moveDelay = 1.5;
        double attackMultiplier = 1;

        int lootProfileIndex = -1;


        int weight = 0;
        int interval = 0;
    };

    extern EntityBase* goblinScout;
    extern EntityBase* goblinWarrier;
    extern EntityBase* goblinArcher;
    extern EntityBase* troll;
    extern EntityBase* wraith;
    extern EntityBase* slime;
    extern EntityBase* myconid;
    extern EntityBase* snake;

    extern EntityBase* mimic;
    extern EntityBase* bunny;

    extern vector<EntityBase*> enemyWeightList;

    extern int total;

    void initEnemies();

    void cleanupEnemies();

    EntityBase* atl(EntityBase* we);

    void setIntervals(int difficulty);

    EntityAi* makeRandomEntity(int difficulty);

    EntityAi* makeEntity(EntityBase* we, int difficulty);

}

#endif /* defined(__Underneath__EnemyGenerator__) */
