//
//  EntityTimeActivated.cpp
//  Underneath
//
//  Created by Braeden Atlee on 4/11/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "EntityTimeActivated.hpp"
#include "Level.hpp"
#include "Animator.hpp"

EntityTimeActivated::EntityTimeActivated() : EntityTimeActivated("", 0, 1, 1, 2, Point2Zero) {

}

EntityTimeActivated::EntityTimeActivated(ItemTimeActivated* item, Point2 p) : EntityTimeActivated(item->name, item->timeActivatedType, item->time, item->power, item->radius, p) {

}

EntityTimeActivated::EntityTimeActivated(string name, TimeActivatedType timeActivatedType, double time, double power, double radius, Point2 p) : Entity('!', p, Ui::C_LIGHT_RED) {
    this->name = name;
    this->timeActivatedType = timeActivatedType;
    this->timeLeft = time;
    this->power = power;
    this->radius = radius;
}

EntityTimeActivated::~EntityTimeActivated() {

}

char EntityTimeActivated::getChar(unsigned long tick, Point2 pos, Level* lvl) {
    switch (timeActivatedType) {
        case timeActivatedBomb:
        case timeActivatedWallBomb:
            return tick%4==0?'*':((timeLeft<10)?(to_string((int)timeLeft)[0]):'!');

        case timeActivatedDud:
            return '*';
        
        default:
            return '!';
    }
}

bool EntityTimeActivated::update(double deltaTime, double time, Level* level) {
    timeLeft -= deltaTime;
    if(!removed && timeLeft <= 0){
        switch (timeActivatedType) {
            case timeActivatedBomb:
                level->explode(pos, radius, power, false);
                Animator::renderExposion(pos, radius, level, 1);
                break;
            case timeActivatedWallBomb:
                level->explode(pos, radius, power, true);
                Animator::renderExposion(pos, radius, level, 1);
                break;

        }
        level->removeEntity(this, true);
    }
    return Entity::update(deltaTime, time, level);
}

EntityTimeActivated* EntityTimeActivated::cloneUnsafe(EntityTimeActivated* oldE, EntityTimeActivated* newE) {

    Entity::cloneUnsafe(oldE, newE);

    newE->name = oldE->name;
    newE->timeActivatedType = oldE->timeActivatedType;
    newE->timeLeft = oldE->timeLeft;
    newE->power = oldE->power;
    newE->radius = oldE->radius;

    return newE;

}

int EntityTimeActivated::getEntityTypeId() {
    return ENTITY_TYPE_TIME_ACTIVATED;
}

void EntityTimeActivated::save(vector<unsigned char>* data) {
    Entity::save(data);
    Utility::saveString(data, name);
    Utility::saveInt(data, timeActivatedType);
    Utility::saveDouble(data, timeLeft);
    Utility::saveDouble(data, power);
    Utility::saveDouble(data, radius);
}

void EntityTimeActivated::load(unsigned char* data, int* position) {
    Entity::load(data, position);
    name = Utility::loadString(data, position);
    timeActivatedType = Utility::loadInt(data, position);
    timeLeft = Utility::loadDouble(data, position);
    power = Utility::loadDouble(data, position);
    radius = Utility::loadDouble(data, position);
}
