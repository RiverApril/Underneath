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

Alive::Alive(string name, char icon, Point2 startPos, Ui::color colorCode) : Entity(name, icon, startPos, colorCode){

}


bool Alive::update(int tick, shared_ptr<Level> level) {
    return Entity::update(tick, level);
}


shared_ptr<Alive> Alive::clone(shared_ptr<Alive> oldE, shared_ptr<Alive> newE){

    if(newE == nullptr){
        newE = shared_ptr<Alive>(new Alive());
    }

    Entity::clone(oldE, newE);

    newE->maxHp = oldE->maxHp;
    newE->hp = oldE->hp;
    newE->viewDistance = oldE->viewDistance;

    return newE;

}

void Alive::save(string* data){
    Entity::save(data);
    Utility::saveInt(data, maxHp);
    Utility::saveInt(data, hp);
    Utility::saveInt(data, viewDistance);
}

int Alive::getEntityTypeId(){
    return ENTITY_TYPE_ALIVE;
}

void Alive::load(char* data, int* position){
    Entity::load(data, position);
    maxHp = Utility::loadInt(data, position);
    hp = Utility::loadInt(data, position);
    viewDistance = Utility::loadInt(data, position);
}



