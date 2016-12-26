//
//  Entity.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/1/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Entity.hpp"
#include "EntityAlive.hpp"
#include "EntityAi.hpp"
#include "EntityPlayer.hpp"
#include "EntityItem.hpp"
#include "EntityExplosive.hpp"
#include "EntityShop.hpp"
#include "EntityMoving.hpp"
#include "EntityMulti.hpp"
#include "EntityMultiSub.hpp"

#include "Animator.hpp"
#include "Level.hpp"
#include "Math.hpp"
#include "Utility.hpp"
#include "Global.hpp"

Entity::Entity() : Entity(' ', Point2Zero, C_WHITE) {

}

Entity::Entity(char icon, Point2 startPos, Ui::Color color) {
    this->defaultIcon = icon;
    this->fgColor = color;
    //this->bgColor = C_BLACK;

    pos = startPos;
    lastPos = startPos;
}

Entity::~Entity() {

}

bool Entity::tryToMoveAbsalute(Point2 p, Level* level, bool force) {
    if(canMoveAbsalute(p, level, force)){
        lastPos = pos;
        pos = p;
        return true;
    }
    return false;
}

bool Entity::canMoveAbsalute(Point2 p, Level* level, bool force) {
    
    
    if (level->tileAt(p)->doesNotHaveAnyOfFlags(getSolidity())) {
        bool block = false;
        for (Entity* e : level->entityList) {
            if (e->uniqueId == uniqueId) {
                continue;
            }
            if ((bool)(e->getSolidity() & getSolidity())) {
                if (e->pos == p) {
                    block = true;
                    break;
                }
            }
        }
        if (!block || force) {
            if(distanceSquared(pos, p) > 1){
                Animator::renderFlash(pos, level, {defaultIcon, 'O', 'o', '.'}, 1, fgColor, getBgColor(0, pos, level));
                Point2 pp = pos;
                pos = p;
                level->currentWorld->menuGame->viewUpdate();
                level->currentWorld->menuGame->render(level->currentWorld->worldTime);
                Animator::renderFlash(pos, level, {'.', 'o', 'O', defaultIcon}, 1, fgColor, getBgColor(0, pos, level));
                pos = pp;
            }
            return true;
        }
    }
    return false;
}

bool Entity::tryToMoveRelative(Point2 p, Level* level) {
    if (p == Point2Zero) {
        return false;
    }
    return tryToMoveAbsalute(pos + p, level);
}

bool Entity::update(double deltaTime, double time, Level* level) {
    return false;
}

/*char Entity::getIcon(Point2 p, double time, Level* level) {
    return defaultIcon;
}

Ui::Color Entity::getFgColorCode() {
    return fgColorCode;
}

Ui::Color Entity::getBgColorCode() {
    return bgColorCode;
}*/

Entity* Entity::cloneUnsafe(Entity* oldE, Entity* newE) {

    newE->uniqueId = oldE->uniqueId;
    newE->defaultIcon = oldE->defaultIcon;
    newE->pos = oldE->pos;
    newE->lastPos = oldE->lastPos;
    newE->fgColor = oldE->fgColor;
    //newE->bgColor = oldE->bgColor;

    return newE;
}

template<class Super, class Sub>
Sub* Entity::makeNewAndClone(Super* oldT) {
    Sub* newT = new Sub();
    return Sub::cloneUnsafe(dynamic_cast<Sub*> (oldT), newT);
}

Entity* Entity::clone(Entity* oldE) {

    int type = oldE->getEntityTypeId();

    debug("Clone Entity of type: " + to_string(type));

    switch (type) {
        case ENTITY_TYPE_ENTITY:
            return makeNewAndClone<Entity, Entity>(oldE);

        case ENTITY_TYPE_ALIVE:
            return makeNewAndClone<Entity, EntityAlive>(oldE);

        case ENTITY_TYPE_AIENTITY:
            return makeNewAndClone<Entity, EntityAi>(oldE);

        case ENTITY_TYPE_PLAYER:
            return makeNewAndClone<Entity, EntityPlayer>(oldE);

        case ENTITY_TYPE_ITEMENTITY:
            return makeNewAndClone<Entity, EntityItem>(oldE);

        case ENTITY_TYPE_EXPLOSIVE:
            return makeNewAndClone<Entity, EntityExplosive>(oldE);

        case ENTITY_TYPE_MOVING:
            return makeNewAndClone<Entity, EntityMoving>(oldE);
            
        case ENTITY_TYPE_SHOP:
            return makeNewAndClone<Entity, EntityShop>(oldE);
            
        case ENTITY_TYPE_MULTI:
            return makeNewAndClone<Entity, EntityMulti>(oldE);
            
        case ENTITY_TYPE_MULTI_SUB:
            return makeNewAndClone<Entity, EntityMultiSub>(oldE);

        default:
            throw Utility::FileExceptionLoad("Entity type unknown: " + to_string(type));
            return nullptr;
            break;
    }



}

Ui::Color Entity::getBgColor(unsigned long tick, Point2 pos, Level* lvl){
    return lvl->tileAt(pos)->getIcon(true)->getBgColor(tick, pos, lvl);
}

void Entity::save(vector<unsigned char>* data) {
    Utility::saveInt(data, getEntityTypeId());


    Utility::saveInt(data, uniqueId);

    Utility::saveUnsignedChar(data, (unsigned char) defaultIcon);

    pos.save(data);
    lastPos.save(data);

    Utility::saveUnsignedChar(data, (unsigned char) fgColor);
    //Utility::saveUnsignedChar(data, (unsigned char) bgColor);
}

int Entity::getEntityTypeId() {
    return ENTITY_TYPE_ENTITY;
}

void Entity::load(vector<unsigned char>* data, int* position) {
    //Entity::loadNew() loads the entityId

    uniqueId = Utility::loadInt(data, position);

    defaultIcon = (char) Utility::loadUnsignedChar(data, position);

    pos = Point2(data, position);
    lastPos = Point2(data, position);

    fgColor = (char) Utility::loadUnsignedChar(data, position);
    //bgColor = (char) Utility::loadUnsignedChar(data, position);
}

Entity* Entity::loadNew(vector<unsigned char>* data, int* position) {
    Entity* e;

    int type = Utility::loadInt(data, position);

    switch (type) {
        case ENTITY_TYPE_ENTITY:
            e = new Entity();
            break;
        case ENTITY_TYPE_ALIVE:
            e = new EntityAlive();
            break;
        case ENTITY_TYPE_AIENTITY:
            e = new EntityAi();
            break;
        case ENTITY_TYPE_PLAYER:
            e = new EntityPlayer();
            break;
        case ENTITY_TYPE_ITEMENTITY:
            e = new EntityItem();
            break;
        case ENTITY_TYPE_EXPLOSIVE:
            e = new EntityExplosive();
            break;
        case ENTITY_TYPE_MOVING:
            e = new EntityMoving();
            break;
        case ENTITY_TYPE_SHOP:
            e = new EntityShop();
            break;
        case ENTITY_TYPE_MULTI:
            e = new EntityMulti();
            break;
        case ENTITY_TYPE_MULTI_SUB:
            e = new EntityMultiSub();
            break;

        default:
            throw Utility::FileExceptionLoad("Entity type unknown: " + to_string(type));
            return nullptr;
            break;
    }
    e->load(data, position);

    debug("Loaded Entity: " + e->getName() + "(" + to_string(e->getEntityTypeId()) + ")" + ", Pos: " + e->pos.toString());

    return e;
}
