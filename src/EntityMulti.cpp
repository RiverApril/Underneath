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
#include "EntityExplosive.hpp"
#include "EntityItem.hpp"


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
    newE->lastThrowTime = oldE->lastThrowTime;
    newE->lastKnownTargetPos = oldE->lastKnownTargetPos;
    
    forVector(oldE->inventory, i) {
        if (oldE->inventory[i] == oldE->activeItemWeapon) {
            newE->activeItemWeapon = dynamic_cast<ItemWeapon*> (newE->inventory[i]);
            break;
        }
        if (oldE->inventory[i] == oldE->activeItemExplosive) {
            newE->activeItemExplosive = dynamic_cast<ItemExplosive*> (newE->inventory[i]);
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
    Utility::saveDouble(data, lastThrowTime);
    lastKnownTargetPos.save(data);
    
    int activeItemWeaponIndex = -1;
    int activeItemExplosiveIndex = -1;
    
    forVector(inventory, i) {
        Item* ie = inventory[i];
        if (ie == activeItemWeapon) {
            activeItemWeaponIndex = i;
        }
        if (ie == activeItemExplosive) {
            activeItemExplosiveIndex = i;
        }
    }
    
    Utility::saveInt(data, activeItemWeaponIndex);
    Utility::saveInt(data, activeItemExplosiveIndex);
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
    lastThrowTime = Utility::loadDouble(data, position);
    lastKnownTargetPos = Point2(data, position);
    
    
    int activeItemWeaponIndex = Utility::loadInt(data, position);
    if (activeItemWeaponIndex != -1) {
        activeItemWeapon = dynamic_cast<ItemWeapon*> (inventory[activeItemWeaponIndex]);
    } else {
        activeItemWeapon = nullptr;
    }
    int activeItemExplosiveIndex = Utility::loadInt(data, position);
    if (activeItemExplosiveIndex != -1) {
        activeItemExplosive = dynamic_cast<ItemExplosive*> (inventory[activeItemExplosiveIndex]);
    } else {
        activeItemExplosive = nullptr;
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
    
    canSeeTarget = false;
    
    for(EntityMultiSub* sub : subEntities){
        canSeeTarget |= level->canSee(sub->pos, target->pos, viewDistance);
    }
    
    if (canSeeTarget) {
        lastKnownTargetPos = target->pos;
    }
}

void EntityMulti::attackAi(double time, Level* level){
    bool attackNormal = false;
    bool attackThrow = false;
    
    vector<Point2> normalFrom;
    vector<Point2> throwFrom;
    
    if(!hasEffect(effStun)){
        if (aiMulti & aiMultiAttackNormal) {
            if (activeItemWeapon && target) {
                for(EntityMultiSub* sub : subEntities){
                    if (distanceSquared(sub->pos, target->pos) <= 1) {
                        attackNormal = true;
                        normalFrom.push_back(sub->pos);
                    } else {
                        ItemRanged* r = dynamic_cast<ItemRanged*> (activeItemWeapon);
                        if (r) {
                            if (canSeeTarget && level->canSee(sub->pos, target->pos, r->range)) {
                                attackNormal = true;
                                normalFrom.push_back(sub->pos);
                            }
                            
                        }
                        
                    }
                }
            }
        }
        if (aiMulti & aiMultiAttackThrow) {
            if(activeItemExplosive && target){
                for(EntityMultiSub* sub : subEntities){
                    if (distanceSquared(sub->pos, target->pos) <= 1) {
                        //
                    } else {
                        if (canSeeTarget && level->canSee(sub->pos, target->pos, viewDistance, true)) {
                            attackThrow = true;
                            throwFrom.push_back(sub->pos);
                        }
                    }
                }
            }
        }
    }
    
    if (attackNormal) {
        while (lastAttackTime + activeItemWeapon->useDelay <= time) {
            
            Point2 from = normalFrom[rand()%normalFrom.size()];
            
            ItemCombatSpell* spell = dynamic_cast<ItemCombatSpell*>(activeItemWeapon);
            BasicIcon* icon = new BasicIcon('*', damageTypeColor(activeItemWeapon->damageType), C_BLACK);
            if(spell){
                Animator::renderRangedAttack(from, target->pos, icon, level, 3);
            }else{
                Animator::renderRangedAttack(from, target->pos, icon, level, 1);
            }
            delete icon;
            
            double d = target->hurt(level, activeItemWeapon, 1);
            Verbalizer::attack(this, target, activeItemWeapon, d);
            lastAttackTime += activeItemWeapon->useDelay / 2;
        }
    }
    
    if(attackThrow){
        while (lastThrowTime + 5 <= time) {
            
            
            Point2 from = throwFrom[rand()%throwFrom.size()];
            
            Item* item = Item::clone(activeItemExplosive);
            EntityExplosive* expl = new EntityExplosive(dynamic_cast<ItemExplosive*>(item), from, lastKnownTargetPos, 1, uniqueId);
            level->newEntity(expl);
            lastThrowTime += 5;
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
                dynamic_cast<EntityMultiSub*>(e)->master = this;
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
    
    int xp = rand() % (int) maxHp;
    Verbalizer::defeatedEnemy(this, xp);
    
    if(level->currentWorld->currentPlayer){
        level->currentWorld->currentPlayer->gainXp(xp);
    }
    
    vector<Item*> drops = ItemGenerator::makeLoot(ItemGenerator::lootProfileBoss, level->getDifficulty(), rand()%(int)(maxHp*max(1, level->getDifficulty())), 10, 20, 5);
    for(Item* i : drops){
        level->newEntity(new EntityItem(i, pos));
    }
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

void EntityMulti::setActiveItemWeapon(ItemWeapon* newItemWeapon) {
    
    if (newItemWeapon == nullptr) {
        activeItemWeapon = nullptr;
        return;
    }
    
    for (Item* ie : inventory) {
        if (ie == newItemWeapon) {
            activeItemWeapon = dynamic_cast<ItemWeapon*> (ie);
            return;
        }
    }
    inventory.push_back(newItemWeapon);
    activeItemWeapon = newItemWeapon;
}

void EntityMulti::setActiveItemExplosive(ItemExplosive* newItemExplosive) {
    
    if (newItemExplosive == nullptr) {
        activeItemExplosive = nullptr;
        return;
    }
    
    for (Item* ie : inventory) {
        if (ie == newItemExplosive) {
            activeItemExplosive = dynamic_cast<ItemExplosive*> (ie);
            return;
        }
    }
    inventory.push_back(newItemExplosive);
    activeItemExplosive = newItemExplosive;
}




