//
//  EntityMoving.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/10/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "EntityMoving.hpp"
#include "Level.hpp"

EntityMoving::EntityMoving() : EntityMoving(Point2Zero, 0) {

}

EntityMoving::EntityMoving(Point2 p, double z) : Entity(' ', p, C_LIGHT_RED) {
    location = Vector3(p, z);
}

EntityMoving::~EntityMoving() {

}

char EntityMoving::getChar(unsigned long tick, Point2 pos, Level* lvl) {
    return '-';
}

double EntityMoving::getFriction(){
    return (location.z>0 ? AIR_FRICTION : GROUND_FRICTION);
}

bool EntityMoving::update(double deltaTime, double time, Level* level) {

    if(velocity != Vector3Zero){

        velocity *= 1-(getFriction() * deltaTime);

        velocity.z -= GRAVITY;

        if(location.z <= 0){
            velocity.z = 0;
        }

        Point2 p = (location.xy() + (velocity.xy() * deltaTime)).floor();

        if(!level->solidAt(p, tileFlagSolidBoth, true)){
            location += velocity * deltaTime;
        }else{
            Point2 p1 = (location.xy() + (Vector2(velocity.x, 0) * deltaTime)).floor();
            Point2 p2 = (location.xy() + (Vector2(0, velocity.y) * deltaTime)).floor();
            if(!level->solidAt(p1, tileFlagSolidBoth, true)){
                velocity.y = 0;
                location += velocity * deltaTime;
                hit(level, hitAngle, p1);
            }else if(!level->solidAt(p2, tileFlagSolidBoth, true)){
                velocity.x = 0;
                location += velocity * deltaTime;
                hit(level, hitAngle, p2);
            }else{
                hit(level, hitNormal, p);
            }
        }

    }

    if(location.z <= 0){
        location.z = 0;
        hit(level, hitGround, location.xy().floor());
    }


    pos = location.xy().floor();

    return Entity::update(deltaTime, time, level);
}

EntityMoving* EntityMoving::cloneUnsafe(EntityMoving* oldE, EntityMoving* newE) {

    Entity::cloneUnsafe(oldE, newE);

    newE->location = oldE->location;
    newE->velocity = oldE->velocity;

    return newE;
}

int EntityMoving::getEntityTypeId() {
    return ENTITY_TYPE_MOVING;
}

void EntityMoving::save(vector<unsigned char>* data) {
    Entity::save(data);
    location.save(data);
    velocity.save(data);
}

void EntityMoving::load(vector<unsigned char>* data, int* position) {
    Entity::load(data, position);
    location = Vector3(data, position);
    velocity = Vector3(data, position);
}


