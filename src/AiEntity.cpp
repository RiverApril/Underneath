//
//  AiEntity.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "AiEntity.h"
#include "Utility.h"

AiEntity::AiEntity() : AiEntity("", aiNone, ' ', Point2Zero, Ui::C_WHITE){

}

AiEntity::AiEntity(std::string name, int aiFlags, char icon, Point2 startPos, Ui::color colorCode) : Alive(name, icon, startPos, colorCode) {
    this->ai = aiFlags;
}

AiEntity::~AiEntity() {
    
}

void AiEntity::runAi(int tick, Level* level) {

    Point2 speed;

    if(ai & aiMoveRandom) {
        speed.x = (rand()%3-1);
        speed.y = (rand()%3-1);
    }

    tryToMove(speed, level);

}

bool AiEntity::update(int tick, Level* level) {

    runAi(tick, level);
    
    return Alive::update(tick, level);
}

AiEntity* AiEntity::clone(AiEntity* oldE, AiEntity* newE){

    if(newE == nullptr){
        newE = new AiEntity();
    }

    Alive::clone(oldE, newE);

    newE->ai = oldE->ai;
    
    return newE;
}

void AiEntity::save(std::string* data){
    Alive::save(data);
    Utility::saveInt(data, ai);
}

int AiEntity::getEntityTypeId(){
    return ENTITY_TYPE_AIENTITY;
}

void AiEntity::load(char* data, int* position){
    Alive::load(data, position);
    ai = Utility::loadInt(data, position);
}


