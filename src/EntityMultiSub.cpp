//
//  EntityMultiSub.cpp
//  Underneath
//
//  Created by Emily Atlee on 7/26/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#include "EntityMultiSub.hpp"
#include "Level.hpp"

void EntityMultiSub::save(vector<unsigned char>* data){
    EntityAlive::save(data);
    
    relPos.save(data);
}

void EntityMultiSub::load(vector<unsigned char>* data, int* position){
    EntityAlive::load(data, position);
    
    relPos = Point2(data, position);
}

int EntityMultiSub::getEntityTypeId(){
    return ENTITY_TYPE_MULTI_SUB;
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
    return EntityAlive::update(deltaTime, time, level);
}







