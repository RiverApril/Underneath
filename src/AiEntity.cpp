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

AiEntity::AiEntity(std::string name, int aiFlags, char icon, Point2 startPos, Ui::Color colorCode, int maxHp) : Alive(name, icon, startPos, colorCode, maxHp) {
    this->ai = aiFlags;
    this->moveDelay = .9;
}

AiEntity::~AiEntity() {
    
}

void AiEntity::runAi(double time, Level* level) {

    if(target == nullptr){
        target = level->currentWorld->currentPlayer;
    }

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
    if(ai & aiFollowPlayerDumb || ai & aiFleeFromPlayerDumb){
        if(level->canSee(pos, target->pos, viewDistance)){

            Point2 playerPos = target->pos;

            speed.x = pos.x>playerPos.x?-1:(pos.x<playerPos.x?1:0);
            speed.y = pos.y>playerPos.y?-1:(pos.y<playerPos.y?1:0);

            if(ai & aiFleeFromPlayerDumb){
                if(speed.x == 0){
                    speed.x = rand()%2==0?1:-1;
                }
                if(speed.y == 0){
                    speed.y = rand()%2==0?1:-1;
                }
                speed = speed*-1;
            }
        }
    }
    if(ai & aiFollowPlayerSmart){
        if(level->canSee(pos, target->pos, viewDistance)){
            lastKnownTargetPos = target->pos;
        }
        if(lastKnownTargetPos != Point2(-1, -1)){
            speed.x = pos.x>lastKnownTargetPos.x?-1:(pos.x<lastKnownTargetPos.x?1:0);
            speed.y = pos.y>lastKnownTargetPos.y?-1:(pos.y<lastKnownTargetPos.y?1:0);
        }

    }


    if(!tryToMoveRelative(speed, level)){
        if(ai & aiAttackPlayer){
            if(target->pos == (pos+speed)){
            	if(activeWeapon != nullptr){
                    while(lastAttackTime + activeWeapon->useDelay <= time){
                		target->hurt(activeWeapon, time);
                        lastAttackTime += activeWeapon->useDelay;
                    }
            	}
            }
        }
    }else if(speed != Point2Zero){
        lastAttackTime = time;
    }


}

bool AiEntity::update(double time, Level* level) {

    while(lastMoveTime+moveDelay<=time){
        runAi(time, level);
        lastMoveTime += moveDelay;
        if(level->canSee(target->pos, pos, level->currentWorld->currentPlayer->viewDistance)){

            level->renderMenuGame(lastMoveTime);
            usleep(10 * 1000);
        }
    }


    return Alive::update(time, level);
}

AiEntity* AiEntity::cloneUnsafe(AiEntity* oldE, AiEntity* newE){

    Alive::cloneUnsafe(oldE, newE);

    newE->ai = oldE->ai;
    
    return newE;
}

int AiEntity::getEntityTypeId(){
    return ENTITY_TYPE_AIENTITY;
}

void AiEntity::save(std::vector<unsigned char>* data){
    Alive::save(data);
    FileUtility::saveInt(data, ai);
    if(ai & aiFollowPlayerSmart){
        Point2::save(lastKnownTargetPos, data);
    }
}

void AiEntity::load(unsigned char* data, int* position){
    Alive::load(data, position);
    ai = FileUtility::loadInt(data, position);
    if(ai & aiFollowPlayerSmart){
        lastKnownTargetPos = Point2::load(data, position);
    }
}


