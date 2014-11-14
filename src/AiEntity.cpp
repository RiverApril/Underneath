//
//  AiEntity.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "AiEntity.h"
#include "Utility.h"
#include "Level.h"

AiEntity::AiEntity() : AiEntity("", aiNone, ' ', Point2Zero, Ui::C_WHITE){

}

AiEntity::AiEntity(std::string name, int aiFlags, char icon, Point2 startPos, Ui::color colorCode, int maxHp) : Alive(name, icon, startPos, colorCode, maxHp) {
    this->ai = aiFlags;
}

AiEntity::~AiEntity() {
    
}

void AiEntity::runAi(int tick, shared_ptr<Level> level) {

    Point2 speed;

    if(ai & aiMoveRandom) {
        if(rand()%2==0){
        	speed.x = (rand()%3-1);
            speed.y = 0;
        }else{
            speed.x = 0;
        	speed.y = (rand()%3-1);
        }
    }
    if(ai & aiFollowPlayerDumb){
        if(level->canSee(*pos, level->currentWorld->currentPlayer->getPos(), viewDistance)){

            Point2 playerPos = level->currentWorld->currentPlayer->getPos();

            speed.x = pos->x>playerPos.x?-1:(pos->x<playerPos.x?1:0);
            speed.y = pos->y>playerPos.y?-1:(pos->y<playerPos.y?1:0);
        }
    }

    if(!tryToMoveRelative(speed, level)){
        if(ai & aiAttackPlayer){

            if(level->currentWorld->currentPlayer->getPos() == (*pos+speed)){

            	if(activeWeapon != nullptr){
                    
                	level->currentWorld->currentPlayer->hurt(activeWeapon);
            	}
            }
        }
    }

}

bool AiEntity::update(int tick, shared_ptr<Level> level) {

    runAi(tick, level);
    
    return Alive::update(tick, level);
}

shared_ptr<AiEntity> AiEntity::clone(shared_ptr<AiEntity> oldE, shared_ptr<AiEntity> newE){

    if(newE == nullptr){
        newE = shared_ptr<AiEntity>(new AiEntity());
    }

    Alive::clone(oldE, newE);

    newE->ai = oldE->ai;
    
    return newE;
}

int AiEntity::getEntityTypeId(){
    return ENTITY_TYPE_AIENTITY;
}

void AiEntity::save(std::string* data){
    Alive::save(data);
    Utility::saveInt(data, ai);
}

void AiEntity::load(char* data, int* position){
    Alive::load(data, position);
    ai = Utility::loadInt(data, position);
}


