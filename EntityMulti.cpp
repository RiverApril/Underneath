//
//  EntityMulti.cpp
//  Underneath
//
//  Created by Braeden Atlee on 7/26/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#include "EntityMulti.hpp"
#include "EntityMultiSub.hpp"
#include "Level.hpp"
#include "Animator.hpp"
#include "Verbalizer.hpp"


EntityMulti* EntityMulti::cloneUnsafe(EntityMulti* oldE, EntityMulti* newE){
    EntityAlive::cloneUnsafe(oldE, newE);
    
    for(EntityMultiSub* entity : oldE->subEntities){
        EntityMultiSub* c = new EntityMultiSub();
        c->master = newE;
        newE->subEntities.push_back(dynamic_cast<EntityMultiSub*>(c->clone(entity)));
    }
    newE->spawnedSubs = false;
    
    newE->aiMulti = oldE->aiMulti;
    newE->moveDelay = oldE->moveDelay;
    newE->lastAttackTime = oldE->lastAttackTime;
    newE->lastKnownTargetPos = oldE->lastKnownTargetPos;
    
    forVector(oldE->inventory, i) {
        if (oldE->inventory[i] == oldE->activeItemWeapon) {
            newE->activeItemWeapon = dynamic_cast<ItemWeapon*> (newE->inventory[i]);
            break;
        }
    }
    
    return newE;
}

void EntityMulti::save(vector<unsigned char>* data){
    EntityAlive::save(data);
    
    Utility::saveBool(data, spawnedSubs);
    Utility::saveInt(data, (int)subEntities.size());
    for(EntityMultiSub* e : subEntities){
        Utility::saveInt(data, e->uniqueId);
    }
    Utility::saveInt(data, aiMulti);
    Utility::saveDouble(data, moveDelay);
    Utility::saveDouble(data, lastMoveTime);
    Utility::saveDouble(data, lastAttackTime);
    lastKnownTargetPos.save(data);
    
    int activeItemWeaponIndex = -1;
    
    forVector(inventory, i) {
        Item* ie = inventory[i];
        if (ie == activeItemWeapon) {
            activeItemWeaponIndex = i;
        }
    }
}

void EntityMulti::load(vector<unsigned char>* data, int* position){
    EntityAlive::load(data, position);
    
    spawnedSubs = Utility::loadBool(data, position);
    int count = Utility::loadInt(data, position);
    for(int i=0;i<count;i++){
        subEntitiesAsIds.push_back(Utility::loadInt(data, position));
    }
    aiMulti = Utility::loadInt(data, position);
    moveDelay = Utility::loadDouble(data, position);
    lastMoveTime = Utility::loadDouble(data, position);
    lastAttackTime = Utility::loadDouble(data, position);
    lastKnownTargetPos = Point2(data, position);
    
    
    int activeItemWeaponIndex = Utility::loadInt(data, position);
    if (activeItemWeaponIndex != -1) {
        activeItemWeapon = dynamic_cast<ItemWeapon*> (inventory[activeItemWeaponIndex]);
    } else {
        activeItemWeapon = nullptr;
    }
}

int EntityMulti::getEntityTypeId(){
    return ENTITY_TYPE_MULTI;
}

EntityMulti::EntityMulti(){
    
}

EntityMulti::EntityMulti(string name, int aiMultiFlags, vector<vector<char>> icons, Ui::Color color, Point2 startPos, Point2 relCenter, int maxHp) : EntityAlive(name, icons[relCenter.y][relCenter.x], startPos, color, maxHp){
    
    this->aiMulti = aiMultiFlags;
    
    for(int y = 0; y < icons.size(); y++){
        for(int x = 0; x < icons[y].size(); x++){
            if(Point2(x, y) != relCenter){
                this->subEntities.push_back(new EntityMultiSub(this, formatString("%s %d,%d", name.c_str(), x, y), icons[y][x], color, Point2(x, y)-relCenter));
            }
        }
    }
}

EntityMulti::~EntityMulti(){
    
}

void EntityMulti::lookAi(double time, Level* level){
    if (!target) {
        target = level->currentWorld->currentPlayer;
    }
    
    if(!target || target->dead){
        canSeeTarget = false;
        return;
    }
    
    
    
    canSeeTarget = level->canSee(pos, target->pos, viewDistance);
    
    if (canSeeTarget) {
        lastKnownTargetPos = target->pos;
    }
}

void EntityMulti::attackAi(double time, Level* level){
    bool attackNormal = false;
    
    if(!hasEffect(effStun)){
        if (aiMulti & aiMultiAttackNormal) {
            if (activeItemWeapon != nullptr && target) {
                if (distanceSquared(pos, target->pos) <= 1) {
                    attackNormal = true;
                } else {
                    ItemRanged* r = dynamic_cast<ItemRanged*> (activeItemWeapon);
                    if (r) {
                        if (canSeeTarget && level->canSee(pos, target->pos, r->range)) {
                            attackNormal = true;
                        }
                        
                    }
                    
                }
            }
        }
        if (aiMulti & aiMultiAttackThrow) {
            if(target){
                
            }
        }
    }
    
    if (attackNormal) {
        while (lastAttackTime + activeItemWeapon->useDelay <= time) {
            
            ItemCombatSpell* spell = dynamic_cast<ItemCombatSpell*>(activeItemWeapon);
            BasicIcon* icon = new BasicIcon('*', damageTypeColor(activeItemWeapon->damageType), C_BLACK);
            if(spell){
                Animator::renderRangedAttack(pos, target->pos, icon, level, 8);
            }else{
                Animator::renderRangedAttack(pos, target->pos, icon, level, 1);
            }
            delete icon;
            
            double d = target->hurt(level, activeItemWeapon, 1);
            Verbalizer::attack(this, target, activeItemWeapon, d);
            lastAttackTime += activeItemWeapon->useDelay;
        }
    }
    
    if (activeItemWeapon != nullptr) {
        while (lastAttackTime + activeItemWeapon->useDelay <= time) {
            lastAttackTime += activeItemWeapon->useDelay;
        }
    } else {
        lastAttackTime = time;
    }
}


bool EntityMulti::update(double deltaTime, double time, Level* level){
    if(subEntitiesAsIds.size() > 0){
        for(int id : subEntitiesAsIds){
            Entity* e = level->getEntity(id);
            if(e){
                subEntities.push_back(dynamic_cast<EntityMultiSub*>(e));
            }
        }
        subEntitiesAsIds.clear();
    }
    if(!spawnedSubs){
        
        for(EntityMultiSub* entity : subEntities){
            level->newEntity(entity);
        }
        
        spawnedSubs = true;
    }
    
    
    bool r = EntityAlive::update(deltaTime, time, level);
    
    
    if (dead) {
        for(EntityMultiSub* entity : subEntities){
            entity->die();
        }
        dropLoots(level);
    }else{
        lookAi(time, level);
        attackAi(time, level);
        
    }
    
    return r;
}

void EntityMulti::dropLoots(Level* level){
    //todo
}

double EntityMulti::hurt(Level* level, DamageType damageType, double amount, double damageMultiplier){
    return EntityAlive::hurt(level, damageType, amount, damageMultiplier);
}

double EntityMulti::hurt(Level* level, ItemWeapon* w, double damageMultiplier){
    return EntityAlive::hurt(level, w, damageMultiplier);
}

bool EntityMulti::tryToMoveAbsalute(Point2 p, Level* world, bool force){
    bool can = true;
    for(EntityMultiSub* entity : subEntities){
        if(!entity->canMoveAbsalute(p+entity->relPos, world, force)){
            can = false;
        }
    }
    if(can){
        for(EntityMultiSub* entity : subEntities){
            entity->tryToMoveAbsalute(p+entity->relPos, world, force);
        }
        Entity::tryToMoveAbsalute(p, world, force);
        return true;
    }
    return false;
}




