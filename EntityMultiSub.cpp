//
//  EntityMultiSub.cpp
//  Underneath
//
//  Created by Braeden Atlee on 7/26/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "EntityMultiSub.hpp"
#include "Level.hpp"

EntityMultiSub* EntityMultiSub::cloneUnsafe(EntityMultiSub* oldE, EntityMultiSub* newE){
    EntityAlive::cloneUnsafe(oldE, newE);
    
    newE->masterId = oldE->master->uniqueId;
    
    return newE;
}

void EntityMultiSub::save(vector<unsigned char>* data){
    EntityAlive::save(data);
    
    Utility::saveInt(data, master->uniqueId);
    relPos.save(data);
}

void EntityMultiSub::load(vector<unsigned char>* data, int* position){
    EntityAlive::load(data, position);
    
    masterId = Utility::loadInt(data, position);
    relPos = Point2(data, position);
}

int EntityMultiSub::getEntityTypeId(){
    return ENTITY_TYPE_MULTI;
}

EntityMultiSub::EntityMultiSub(){
    
}

EntityMultiSub::EntityMultiSub(EntityMulti* master, string name, char icon, Ui::Color color, Point2 relPos) : EntityAlive(name, icon, master->pos + relPos, color, 1){
    this->master = master;
    this->relPos = relPos;
}

EntityMultiSub::~EntityMultiSub(){
    
}

double EntityMultiSub::hurt(Level* level, DamageType damageType, double amount, double damageMultiplier){
    return master->hurt(level, damageType, amount, damageMultiplier);
}

double EntityMultiSub::hurt(Level* level, ItemWeapon* w, double damageMultiplier){
    return master->hurt(level, w, damageMultiplier);
}

bool EntityMultiSub::update(double deltaTime, double time, Level* level){
    if(masterId != -1){
        master = dynamic_cast<EntityMulti*>(level->getEntity(masterId));
        
        masterId = -1;
    }
    return EntityAlive::update(deltaTime, time, level);
}







