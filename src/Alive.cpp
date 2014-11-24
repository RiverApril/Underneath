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

Alive::Alive(string name, char icon, Point2 startPos, Ui::color colorCode, int maxHp) : Entity(icon, startPos, colorCode){

    this->name = name;
    this->maxHp = maxHp;
    this->hp = this->maxHp;
    this->solid = true;

}

Alive::~Alive(){
    int i;
    forVector(inventory, i){
        delete inventory[i];
    }
}


bool Alive::update(int tick, shared_ptr<Level> level) {

    if(dead){
        level->removeEntity(this, true);
    }else{
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


Alive* Alive::clone(Alive* oldE, Alive* newE){

    if(newE == nullptr){
        newE = new Alive();
    }

    Entity::clone(oldE, newE);

    newE->name = oldE->name;
    newE->maxHp = oldE->maxHp;
    newE->hp = oldE->hp;
    newE->maxMp = oldE->maxMp;
    newE->mp = oldE->mp;
    newE->viewDistance = oldE->viewDistance;

    int activeWeaponIndex = -1;
    for(int i=0;i<oldE->inventory.size();i++){
        newE->inventory.push_back(oldE->inventory[i]);
        if(oldE->inventory[i] == oldE->activeWeapon){
            activeWeaponIndex = i;
        }
    }
    if(activeWeaponIndex != -1){
    	newE->activeWeapon = dynamic_cast<Weapon*>(newE->inventory[activeWeaponIndex]);
    }

    newE->effects = oldE->effects;

    return newE;

}

int Alive::getEntityTypeId(){
    return ENTITY_TYPE_ALIVE;
}

void Alive::save(string* data){
    Entity::save(data);
    FileUtility::saveBool(data, dead);
    FileUtility::saveInt(data, maxHp);
    FileUtility::saveInt(data, hp);
    FileUtility::saveInt(data, maxMp);
    FileUtility::saveInt(data, mp);
    FileUtility::saveInt(data, viewDistance);
    FileUtility::saveInt(data, regenTick);
    FileUtility::saveString(data, name);

    //
    int activeWeaponIndex = -1;
    FileUtility::saveInt(data, (int)inventory.size());
    for(int i=0;i<inventory.size();i++){
        inventory[i]->save(data);
        if(inventory[i] == activeWeapon){
            activeWeaponIndex = i;
        }
    }
    FileUtility::saveInt(data, activeWeaponIndex);
    //

    FileUtility::saveInt(data, (int)effects.size());
    for(int i=0;i<effects.size();i++){
        FileUtility::saveInt(data, effects[i].eId);
        FileUtility::saveInt(data, effects[i].duration);
        FileUtility::saveInt(data, effects[i].power);
    }
}

void Alive::load(char* data, int* position){
    Entity::load(data, position);
    dead = FileUtility::loadBool(data, position);
    maxHp = FileUtility::loadInt(data, position);
    hp = FileUtility::loadInt(data, position);
    maxMp = FileUtility::loadInt(data, position);
    mp = FileUtility::loadInt(data, position);
    viewDistance = FileUtility::loadInt(data, position);
    regenTick = FileUtility::loadInt(data, position);
    name = FileUtility::loadString(data, position);

    //
    int size = FileUtility::loadInt(data, position);
    for(int i=0;i<size;i++){
        Item* item = Item::loadNew(data, position);
        inventory.push_back(item);
    }

    int activeWeaponIndex = FileUtility::loadInt(data, position);
    if(activeWeaponIndex != -1){
    	activeWeapon = dynamic_cast<Weapon*>(inventory[activeWeaponIndex]);
    }else{
        activeWeapon = nullptr;
    }
    //

    size = FileUtility::loadInt(data, position);
    for(int i=0;i<size;i++){
        int eId = FileUtility::loadInt(data, position);
        int duration = FileUtility::loadInt(data, position);
        int power = FileUtility::loadInt(data, position);
        effects.push_back(Effect(eId, duration, power));
    }
}



