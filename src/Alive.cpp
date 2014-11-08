//
//  Alive.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Alive.h"
#include "Utility.h"


Alive::Alive() : Alive("", ' ', Point2Zero, Ui::C_WHITE){

}

Alive::Alive(std::string name, char icon, Point2 startPos, Ui::color colorCode) : Entity(name, icon, startPos, colorCode){

}


bool Alive::update(int tick, Level* level) {
    return Entity::update(tick, level);
}


Alive* Alive::clone(Alive* oldE, Alive* newE){

    if(newE == nullptr){
        newE = new Alive();
    }

    Entity::clone(oldE, newE);

    newE->maxHp = oldE->maxHp;
    newE->hp = oldE->hp;
    newE->maxMp = oldE->maxMp;
    newE->mp = oldE->mp;
    newE->viewDistance = oldE->viewDistance;

    return newE;

}

void Alive::save(std::string* data){
    Entity::save(data);
    Utility::saveInt(data, maxHp);
    Utility::saveInt(data, hp);
    Utility::saveInt(data, maxMp);
    Utility::saveInt(data, mp);
    Utility::saveInt(data, viewDistance);
}

int Alive::getEntityTypeId(){
    return ENTITY_TYPE_ALIVE;
}

void Alive::load(char* data, int* position){
    Entity::load(data, position);
    maxHp = Utility::loadInt(data, position);
    hp = Utility::loadInt(data, position);
    maxMp = Utility::loadInt(data, position);
    mp = Utility::loadInt(data, position);
    viewDistance = Utility::loadInt(data, position);
}



