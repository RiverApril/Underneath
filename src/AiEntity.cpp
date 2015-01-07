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
}

AiEntity::~AiEntity() {
    
}

void AiEntity::runAi(double time, Level* level) {

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
        if(level->canSee(pos, level->currentWorld->currentPlayer->pos, viewDistance)){

            Point2 playerPos = level->currentWorld->currentPlayer->pos;

            speed.x = pos.x>playerPos.x?-1:(pos.x<playerPos.x?1:0);
            speed.y = pos.y>playerPos.y?-1:(pos.y<playerPos.y?1:0);

            if(ai & aiFleeFromPlayerDumb){
                speed = speed*-1;
            }
        }
    }
    if(ai & aiFollowPlayerSmart){
        if(level->canSee(pos, level->currentWorld->currentPlayer->pos, viewDistance)){
            lastKnownPlayerPos = level->currentWorld->currentPlayer->pos;
        }

        if(speed == lastKnownPlayerPos){
            lastKnownPlayerPos.x = -1;
            lastKnownPlayerPos.y = -1;
        }
        if(lastKnownPlayerPos != Point2(-1, -1)){
            speed.x = pos.x>lastKnownPlayerPos.x?-1:(pos.x<lastKnownPlayerPos.x?1:0);
            speed.y = pos.y>lastKnownPlayerPos.y?-1:(pos.y<lastKnownPlayerPos.y?1:0);
        }

    }

    if(!tryToMoveRelative(speed, level)){
        if(ai & aiAttackPlayer){
            if(level->currentWorld->currentPlayer->pos == (pos+speed)){
            	if(activeWeapon != nullptr){
                	level->currentWorld->currentPlayer->hurt(activeWeapon, time);
            	}
            }
        }
    }

}

bool AiEntity::update(double time, Level* level) {

    while(lastMoveTime+moveDelay<=time){
        runAi(time, level);
        lastMoveTime += moveDelay;
        if(level->canSee(level->currentWorld->currentPlayer->pos, pos, level->currentWorld->currentPlayer->viewDistance)){

            level->renderMenuGame(lastMoveTime);
            //usleep(100 * 1000);
        }
    }


    return Alive::update(time, level);
}

AiEntity* AiEntity::clone(AiEntity* oldE, AiEntity* newE){

    if(newE == nullptr){
        newE = new AiEntity();
    }

    Alive::clone(oldE, newE);

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
        Point2::save(lastKnownPlayerPos, data);
    }
}

void AiEntity::load(unsigned char* data, int* position){
    Alive::load(data, position);
    ai = FileUtility::loadInt(data, position);
    if(ai & aiFollowPlayerSmart){
        lastKnownPlayerPos = Point2::load(data, position);
    }
}


