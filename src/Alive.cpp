//
//  Alive.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Alive.h"
#include "Utility.h"
#include "Level.h"


Alive::Alive() : Alive("", ' ', Point2Zero, Ui::C_WHITE){

}

Alive::Alive(string name, char icon, Point2 startPos, Ui::color colorCode, int maxHp) : Entity(name, icon, startPos, colorCode){

    this->maxHp = maxHp;
    this->hp = this->maxHp;

}


bool Alive::update(int tick, shared_ptr<Level> level) {

    if(dead){
        level->deleteEntity(shared_from_this());
    }

    if(regenTick>=10){
        regenTick = 0;
        heal(1);
    }else{
        regenTick++;
    }

    int i;
    forVector(effects, i){

        switch(effects[i].eId){
            case effBleed:
            case effFire:
                hurt(effects[i].power);
                break;
        }

        effects[i].duration--;
        if(effects[i].duration==0){
			effects.erase(effects.begin()+i);
            i--;
            continue;
        }
    }

    return Entity::update(tick, level);
}

void Alive::addEffect(Effect e){
    int i;
    forVector(effects, i){
        if(e.eId == effects[i].eId){
            effects[i].duration = e.duration;
            effects[i].power = e.power;
            return;
        }
    }
    effects.push_back(e);
}


shared_ptr<Alive> Alive::clone(shared_ptr<Alive> oldE, shared_ptr<Alive> newE){

    if(newE == nullptr){
        newE = shared_ptr<Alive>(new Alive());
    }

    Entity::clone(oldE, newE);

    newE->maxHp = oldE->maxHp;
    newE->hp = oldE->hp;
    newE->viewDistance = oldE->viewDistance;

    int activeWeaponIndex = -1;
    for(int i=0;i<oldE->inventory.size();i++){
        newE->inventory.push_back(oldE->inventory[i]);
        if(oldE->inventory[i] == oldE->activeWeapon){
            activeWeaponIndex = i;
        }
    }
    if(activeWeaponIndex != -1){
    	newE->activeWeapon = dynamic_pointer_cast<Weapon>(newE->inventory[activeWeaponIndex]);
    }

    newE->effects = oldE->effects;

    return newE;

}

int Alive::getEntityTypeId(){
    return ENTITY_TYPE_ALIVE;
}

void Alive::save(string* data){
    Entity::save(data);
    Utility::saveBool(data, dead);
    Utility::saveInt(data, maxHp);
    Utility::saveInt(data, hp);
    Utility::saveInt(data, viewDistance);
    Utility::saveInt(data, regenTick);

    //
    int activeWeaponIndex = -1;
    Utility::saveInt(data, (int)inventory.size());
    for(int i=0;i<inventory.size();i++){
        inventory[i]->save(data);
        if(inventory[i] == activeWeapon){
            activeWeaponIndex = i;
        }
    }
    Utility::saveInt(data, activeWeaponIndex);
    //

    Utility::saveInt(data, (int)effects.size());
    for(int i=0;i<effects.size();i++){
        Utility::saveInt(data, effects[i].eId);
        Utility::saveInt(data, effects[i].duration);
        Utility::saveInt(data, effects[i].power);
    }
}

void Alive::load(char* data, int* position){
    Entity::load(data, position);
    dead = Utility::loadBool(data, position);
    maxHp = Utility::loadInt(data, position);
    hp = Utility::loadInt(data, position);
    viewDistance = Utility::loadInt(data, position);
    regenTick = Utility::loadInt(data, position);

    //
    int size = Utility::loadInt(data, position);
    for(int i=0;i<size;i++){
        shared_ptr<Item> item = Item::loadNew(data, position);
        inventory.push_back(item);
    }

    int activeWeaponIndex = Utility::loadInt(data, position);
    if(activeWeaponIndex != -1){
    	activeWeapon = dynamic_pointer_cast<Weapon>(inventory[activeWeaponIndex]);
    }else{
        activeWeapon = nullptr;
    }
    //

    size = Utility::loadInt(data, position);
    for(int i=0;i<size;i++){
        int eId = Utility::loadInt(data, position);
        int duration = Utility::loadInt(data, position);
        int power = Utility::loadInt(data, position);
        effects.push_back(Effect(eId, duration, power));
    }
}



