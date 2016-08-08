//
//  EntityExplosive.cpp
//  Underneath
//
//  Created by Braeden Atlee on 4/11/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "EntityExplosive.hpp"
#include "Level.hpp"
#include "Animator.hpp"

EntityExplosive::EntityExplosive() : EntityExplosive(nullptr, Point2Zero) {

}

EntityExplosive::EntityExplosive(ItemExplosive* item, Point2 start, Point2 target, double z, int ignoreUID) : EntityMoving(start, z) {
    this->expl = item;
    this->ignoreUID = ignoreUID;
    if(start != target){
        velocity = Vector3(Vector2(target - start).normalize()*2, .5);
    }
}

EntityExplosive::EntityExplosive(ItemExplosive* item, Point2 pos) : EntityMoving(pos, 0) {
    this->expl = item;
}

EntityExplosive::~EntityExplosive() {

}

char EntityExplosive::getChar(unsigned long tick, Point2 pos, Level* lvl) {
    if(expl){
        if(expl->iconIndex != -1){
            return iconList[expl->iconIndex]->getChar(tick, pos, lvl);
        }
        switch (expl->explosiveType) {
            case timeActivatedBomb:
                return tick%4==0?'*':((expl->time<10)?('0'+expl->time):'!');

            case pressureBomb:
                return expl->time == 2 ? '.' : '*';

            case throwableBomb:
                switch (tick%4) {
                    case 0: return 'i';
                    case 1: return '-';
                    case 2: return '!';
                    case 3: return '-';
                    default:
                        return '?';
                }
            
            default:
                return '!';
        }
    }
    return '!';
}

Ui::Color EntityExplosive::getFgColor(unsigned long tick, Point2 pos, Level *lvl){
    if(expl){
        if(expl->iconIndex != -1){
            return iconList[expl->iconIndex]->getFgColor(tick, pos, lvl);
        }
        switch (expl->explosiveType) {
            case timeActivatedBomb:
            case pressureBomb:
                return C_LIGHT_RED;

            case throwableBomb:
                return C_LIGHT_YELLOW;

            default:
                return C_LIGHT_RED;
        }
    }
    return C_LIGHT_RED;
}

void EntityExplosive::activate(Level* level){
    if(expl){
        switch (expl->explosiveType) {
            case throwableBomb:
            case timeActivatedBomb:
            case pressureBomb:
                Animator::renderExposion(pos, expl->radius, level, 1);
                level->explode(pos, expl->radius, expl->power, expl->destroysTiles);
                break;
            default:
                break;

        }
    }
    delete expl;
    expl = nullptr;
    level->removeEntity(this, true);
}

void EntityExplosive::hit(Level* level, HitType h, Point2 p){
    if(expl){
        switch (expl->explosiveType) {
            case throwableBomb:{
                if(h != hitGround){
                    if(level->entityAt(ignoreUID, pos)){
                        break;
                    }
                }
                pos = p;
                activate(level);
                break;
            }
            default:
                break;
        }
    }
}

bool EntityExplosive::update(double deltaTime, double time, Level* level) {
    if(expl){
        switch (expl->explosiveType) {
            case dudBomb:
            case timeActivatedBomb: {
                expl->time -= deltaTime;
                if(!removed && expl->time <= 0){
                    activate(level);
                }
                break;
            }
            case pressureBomb:{
                vector<Entity*> v = level->allEntitiesHere(pos);
                bool eh = false;
                for(Entity* e : v){
                    if(e->uniqueId != uniqueId){
                        eh = true;
                    }
                }
                if(eh){
                    if(expl->time == 0){       //nobody was on me
                        expl->time = 1;        //entity is on me
                    }else if(expl->time == 2){ //entity stepped off, now I can explode
                        activate(level);
                    }
                }else{
                    if(expl->time == 1){ //entity was on me
                        expl->time = 2;  //explode next time
                    }
                }
                break;
            }
            case throwableBomb:{
                if(location.z == 0){
                    activate(level);
                }
                break;
            }
            default:
                break;
        }
        
    }
    return EntityMoving::update(deltaTime, time, level);
}

EntityExplosive* EntityExplosive::cloneUnsafe(EntityExplosive* oldE, EntityExplosive* newE) {

    Entity::cloneUnsafe(oldE, newE);

    newE->expl = dynamic_cast<ItemExplosive*>(Item::clone(oldE->expl));

    return newE;

}

int EntityExplosive::getEntityTypeId() {
    return ENTITY_TYPE_EXPLOSIVE;
}

void EntityExplosive::save(vector<unsigned char>* data) {
    Entity::save(data);
    expl->save(data);
}

void EntityExplosive::load(vector<unsigned char>* data, int* position) {
    Entity::load(data, position);
    expl = dynamic_cast<ItemExplosive*>(Item::loadNew(data, position));
}


