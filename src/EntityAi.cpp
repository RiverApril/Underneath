//
//  EntityAi.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "EntityAi.hpp"
#include "Math.hpp"
#include "Utility.hpp"
#include "Level.hpp"
#include "Verbalizer.hpp"
#include "ItemGenerator.hpp"
#include "EnemyGenerator.hpp"
#include "EntityItem.hpp"
#include "EntityExplosive.hpp"
#include "ItemExplosive.hpp"
#include "Animator.hpp"
#include "Settings.hpp"
#include "Math.hpp"

EntityAi::EntityAi() : EntityAi("", 0, 0, Point2Zero, 0, 0) {

}

EntityAi::EntityAi(string name, int aiFlags, char icon, Point2 startPos, Ui::Color colorCode, int maxHp) : EntityAlive(name, icon, startPos, colorCode, maxHp) {
    this->ai = aiFlags;
}

EntityAi::~EntityAi() {
    
}

void EntityAi::lookAi(double time, Level* level){
    if (!target) {
        target = level->currentWorld->currentPlayer;
    }

    if(!target || target->dead){
        canSeeTarget = false;
        agro = false;
        return;
    }

    

    canSeeTarget = level->canSee(pos, target->pos, agro ? viewDistance * agroViewDistanceMultiplier : viewDistance);
    if (!canSeeTarget && agro) {
        agro = false;
    }
    if(canSeeTarget && !agro){
        agro = true;
    }

    if (canSeeTarget) {
        lastKnownTargetPos = target->pos;
    }
}

void EntityAi::moveAi(double time, Level* level) {
    
    if(hasEffect(effStun)){
        return;
    }

    Point2 speed;

    if (ai & aiMoveRandom) {
        if (rand() % 2 == 0) {
            speed.x = (rand() % 3 - 1);
            speed.y = 0;
        } else {
            speed.x = 0;
            speed.y = (rand() % 3 - 1);
        }
    }

    if(ai & aiAttackAndFleeAtLowHealth){
        if(ai & aiFlee){
            if((hp/maxHp) > healthPercentUpperBoundry){
                ai -= aiFlee;
                ai |= aiAttack;
            }
        }else if(ai & aiAttack){
            if((hp/maxHp) < healthPercentLowerBoundry){
                ai -= aiAttack;
                ai |= aiFlee;
            }
        }else{
            ai |= aiAttack;
        }
    }

    if (ai & aiFlee) {
        if (canSeeTarget) {

            /*speed.x = pos.x > target->pos.x ? 1 : (pos.x < target->pos.x ? -1 : 0);
            speed.y = pos.y > target->pos.y ? 1 : (pos.y < target->pos.y ? -1 : 0);

            if (speed.x == 0) {
                speed.x = rand() % 2 == 0 ? 1 : -1;
            }
            if (speed.y == 0) {
                speed.y = rand() % 2 == 0 ? 1 : -1;
            }

            if(speed.x != 0 && speed.y != 0){
                if((rand()&1)==0){
                    speed.y = 0;
                }else{
                    speed.x = 0;
                }
            }*/

            Point2 runToPos;

            vector<Point2> path;

            for(int i=0;i<20 && path.empty();i++){
                runToPos.x = pos.x + (rand() % viewDistance)-(viewDistance/2);
                runToPos.y = pos.y + (rand() % viewDistance)-(viewDistance/2);

                path = level->getPathTo(runToPos, pos, tileFlagIsTile, getSolidity(), true);

                if(level->getPathTo(target->pos, runToPos, tileFlagIsTile, getSolidity(), false).size() < path.size()){
                    path.clear();
                }
            };


            if(!path.empty()){
                if(Settings::showFollowPaths){
                    for(Point2 point : path){
                        EntityExplosive* e = new EntityExplosive(new ItemExplosive("path", dudBomb, 2, 0, 0, false), point);
                        e->fgColor = C_LIGHT_GREEN;
                        level->newEntity(e);
                    }
                    EntityExplosive* e = new EntityExplosive(new ItemExplosive("runToPos", dudBomb, 2, 0, 0, false), runToPos);
                    e->fgColor = C_LIGHT_MAGENTA;
                    level->newEntity(e);
                }

                speed = path[0]-pos;

            }else{
                speed.x = pos.x > target->pos.x ? 1 : (pos.x < target->pos.x ? -1 : 0);
                speed.y = pos.y > target->pos.y ? 1 : (pos.y < target->pos.y ? -1 : 0);
            }

            if(speed.x != 0 && speed.y != 0){
                if((rand()&1)==0){
                    speed.y = 0;
                }else{
                    speed.x = 0;
                }
            }
        }
    }

    bool tele = false;

    if(ai & aiTeleport){
        if(ai & aiAttack && activeItemWeapon){
            ItemRanged* ir = dynamic_cast<ItemRanged*>(activeItemWeapon);
            if(ir){
                if(canSeeTarget){
                    double rdis = sqrt(distanceSquared(target->pos, pos));
                    if(rdis > ir->range || rdis < min(3.0, ir->range)){
                        Point2 p;

                        int l = 0;

                        do{
                            p.x = (rand()%((int)ir->range*2)) - ir->range + target->pos.x;
                            p.y = (rand()%((int)ir->range*2)) - ir->range + target->pos.y;
                            //level->setTile(p, Tiles::tileDebug4);
                            if(level->canSee(pos, p, ir->range) && sqrt(distanceSquared(target->pos, p)) >= min(5.0, ir->range)){
                                tryToMoveAbsalute(p, level);
                                tele = true;
                                break;
                            }
                            l++;
                            if(l > 100){
                                break;
                            }

                        }while(true);

                    }
                }
            }
        }else if(ai & aiFlee){

        }
    }

    if (ai & aiAttack && !tele) {

        bool pathEmpty = true;
        

        if (target && lastKnownTargetPos.x >= 0 && lastKnownTargetPos.y >= 0) {
            debugf("Last Known Target Position: %d,%d", lastKnownTargetPos.x, lastKnownTargetPos.y);
            ItemRanged* r = dynamic_cast<ItemRanged*> (activeItemWeapon);
            double dis = distanceSquared(target->pos, pos);
            double rng = r?(r->range * r->range):0;
            if (!r || (dis > rng || !canSeeTarget)) {
                vector<Point2> path = level->getPathTo(lastKnownTargetPos, pos, tileFlagIsTile, getSolidity(), true);
                if(path.empty()){
                    path = level->getPathTo(lastKnownTargetPos, pos, tileFlagIsTile, getSolidity(), false);
                }
                if(!path.empty()){
                    if(Settings::showFollowPaths){
                        for(Point2 point : path){
                            EntityExplosive* e = new EntityExplosive(new ItemExplosive("path", dudBomb, 2, 0, 0, false), point);
                            e->fgColor = C_LIGHT_GREEN;
                            level->newEntity(e);
                        }
                        EntityExplosive* e = new EntityExplosive(new ItemExplosive("lastKnownTargetPos", dudBomb, 2, 0, 0, false), lastKnownTargetPos);
                        e->fgColor = C_LIGHT_MAGENTA;
                        level->newEntity(e);
                    }
                    //debugf("%s: %s", name.c_str(), (path[0]-pos).toString().c_str());

                    speed = path[0]-pos;

                    pathEmpty = false;

                }

            }else if(r && dis < rng){
                speed.x = pos.x > target->pos.x ? 1 : (pos.x < target->pos.x ? -1 : 0);
                speed.y = pos.y > target->pos.y ? 1 : (pos.y < target->pos.y ? -1 : 0);
            }

        }

        if(!canSeeTarget && pathEmpty){
            vector<Point2> possibilities;
            for(int i=-1;i<=1;i++){
                for(int j=-1;j<=1;j++){
                    if(abs(i) != abs(j)){
                        Point2 p = pos+Point2(i, j);
                        if(p != lastPos && level->tileAt(p)->doesNotHaveAnyOfFlags(getSolidity())){
                            possibilities.push_back(Point2(i, j));
                        }
                    }
                }
            }
            if(possibilities.size() > 0){
                speed = possibilities[rand()%possibilities.size()];
                //debugf("possibility size: %d", possibilities.size());
            }

        }

    }

    lastPos = pos;
    bool m = false;
    if(rand() % 2 == 0){
        m = m || tryToMoveRelative(speed.xOnly(), level);
        m = m || tryToMoveRelative(speed.yOnly(), level);
    }else{
        m = m || tryToMoveRelative(speed.yOnly(), level);
        m = m || tryToMoveRelative(speed.xOnly(), level);
    }

    if(!canSeeTarget && (pos == lastKnownTargetPos || !m)){
        lastKnownTargetPos = Point2Neg1;
    }


}

void EntityAi::attackAi(double time, Level* level){


    /*if (ai & aiAttack) {
        if(speed != Point2Zero){
            if (lastKnownTargetPos.x >= 0 && lastKnownTargetPos.y >= 0) {
                debug("Failed to move");
                if(!canSeeTarget && level->canSee(pos, lastKnownTargetPos, agro ? viewDistance * agroViewDistanceMultiplier : viewDistance, false)){
                    debug("Lost Target");
                    lastKnownTargetPos = Point2Neg1;
                }
            }
        }
    }*/

    bool attack = false;
    
    if(!hasEffect(effStun)){
        if (ai & aiAttack) {
            if (activeItemWeapon != nullptr && target) {
                if (distanceSquared(pos, target->pos) <= 1) {
                    attack = true;
                } else {
                    ItemRanged* r = dynamic_cast<ItemRanged*> (activeItemWeapon);
                    if (r) {
                        if (canSeeTarget && level->canSee(pos, target->pos, r->range)) {
                            attack = true;
                        }

                    }

                }
            }
        }
    }
    
    if (attack) {
        while (lastAttackTime + activeItemWeapon->useDelay <= time) {

            ItemCombatSpell* spell = dynamic_cast<ItemCombatSpell*>(activeItemWeapon);
            BasicIcon* icon = new BasicIcon('*', damageTypeColor(activeItemWeapon->damageType), C_BLACK);
            if(spell){
                Animator::renderRangedAttack(pos, target->pos, icon, level, 8);
            }else{
                Animator::renderRangedAttack(pos, target->pos, icon, level, 1);
            }
            delete icon;

            double d = target->hurt(level, activeItemWeapon, getAttackMultiplierFromEffectsAndEquips(activeItemWeapon->damageType));
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

double EntityAi::hurt(Level* level, DamageType damageType, double amount, double damageMultiplier) {
    agro = true;
    return EntityAlive::hurt(level, damageType, amount, damageMultiplier);
}

double EntityAi::hurt(Level* level, ItemWeapon* w, double damageMultiplier) {
    agro = true;
    return EntityAlive::hurt(level, w, damageMultiplier);
}

bool EntityAi::update(double deltaTime, double time, Level* level) {

    if (dead) {
        dropLoots(level);
    }else{
        lookAi(time, level);
        while (lastMoveTime + moveDelay <= time) {
            moveAi(time, level);
            lastMoveTime += moveDelay;
            lookAi(time, level);
        }
        attackAi(time, level);

    }

    return EntityAlive::update(deltaTime, time, level);
}

void EntityAi::dropLoots(Level* level){

    if(!getActiveItemWeapon()){
        return;
    }

    int xp = rand() % (int) maxHp;
    Verbalizer::defeatedEnemy(this, xp);

    if(level->currentWorld->currentPlayer){
    	level->currentWorld->currentPlayer->gainXp(xp);
    }
    //level->newEntity(new EntityItem(ItemGenerator::makeCoins((rand()%(int)maxHp*2)), pos));

    /*vector<Item*> drops = ItemGenerator::createRandLoots(level->getDifficulty(), level->getDifficulty() * 100, (rand() % 10) == 0 ? 1 : 0, (rand() % 10) == 0 ? 1 : 0, (rand() % 5) == 0 ? 2 : 0, 20);
    //if(rand()%5==0){
    //drops.push_back(Item::clone(activeItemWeapon));
    //}
    for (Item* i : drops) {
        level->newEntity(new EntityItem(i, pos));
    }*/

    /*if(drops.size() > 0){
        int dropCount = 0;
        while(rand()%((dropCount+1)*3)==0){
            dropCount++;
        }

        while(dropCount > 0){
            pair<int, ItemGenerator::ItemBase*> p = drops[rand()%drops.size()];

            if(p.first > 0 && (rand()%p.first == 0)){
                Item* it = ItemGenerator::createItemFromBase(p.second, level->getDifficulty());
                if(it){
                    level->newEntity(new EntityItem(it, pos));
                }
                dropCount--;
            }
        }
    }*/
    int ll = 0;
    int er = 0;
    if(ItemGenerator::lootProfileList[lootProfileIndex]->canBeModifiedByRandomness){
        if(level->currentWorld->currentPlayer){
            int l = level->currentWorld->currentPlayer->abilities[iLUK];
            ll = maxLootDrop==0?0:((rand()%max(2, 100-l))==0?min(1, l/10):0);
            er = 3;
        }
    }
    vector<Item*> drops = ItemGenerator::makeLoot(lootProfileIndex, level->getDifficulty(), rand()%(int)(maxHp*max(1, level->getDifficulty())), minLootDrop, maxLootDrop+ll, er);
    for(Item* i : drops){
        level->newEntity(new EntityItem(i, pos));
    }

}

EntityAi* EntityAi::cloneUnsafe(EntityAi* oldE, EntityAi* newE) {

    EntityAlive::cloneUnsafe(oldE, newE);

    newE->ai = oldE->ai;
    newE->agro = oldE->agro;
    newE->moveDelay = oldE->moveDelay;
    newE->lastMoveTime = oldE->lastMoveTime;
    newE->lastAttackTime = oldE->lastAttackTime;
    newE->attackMultiplier = oldE->attackMultiplier;
    newE->lastKnownTargetPos = oldE->lastKnownTargetPos;
    newE->lootProfileIndex = oldE->lootProfileIndex;
    newE->maxLootDrop = oldE->maxLootDrop;
    newE->minLootDrop = oldE->minLootDrop;

    forVector(oldE->inventory, i) {
        if (oldE->inventory[i] == oldE->activeItemWeapon) {
            newE->activeItemWeapon = dynamic_cast<ItemWeapon*> (newE->inventory[i]);
            break;
        }
    }

    return newE;
}

int EntityAi::getEntityTypeId() {
    return ENTITY_TYPE_AIENTITY;
}

void EntityAi::save(vector<unsigned char>* data) {
    EntityAlive::save(data);
    Utility::saveInt(data, ai);
    Utility::saveBool(data, agro);
    Utility::saveDouble(data, moveDelay);
    Utility::saveDouble(data, lastMoveTime);
    Utility::saveDouble(data, lastAttackTime);
    Utility::saveDouble(data, attackMultiplier);
    Utility::saveInt(data, lootProfileIndex);
    Utility::saveInt(data, maxLootDrop);
    Utility::saveInt(data, minLootDrop);
    lastKnownTargetPos.save(data);

    int activeItemWeaponIndex = -1;

    forVector(inventory, i) {
        Item* ie = inventory[i];
        if (ie == activeItemWeapon) {
            activeItemWeaponIndex = i;
        }
    }
    Utility::saveInt(data, activeItemWeaponIndex);
}

void EntityAi::load(vector<unsigned char>* data, int* position) {
    EntityAlive::load(data, position);
    ai = Utility::loadInt(data, position);
    agro = Utility::loadBool(data, position);
    moveDelay = Utility::loadDouble(data, position);
    lastMoveTime = Utility::loadDouble(data, position);
    lastAttackTime = Utility::loadDouble(data, position);
    attackMultiplier = Utility::loadDouble(data, position);
    lootProfileIndex = Utility::loadInt(data, position);
    maxLootDrop = Utility::loadInt(data, position);
    minLootDrop = Utility::loadInt(data, position);
    lastKnownTargetPos = Point2(data, position);


    int activeItemWeaponIndex = Utility::loadInt(data, position);
    if (activeItemWeaponIndex != -1) {
        activeItemWeapon = dynamic_cast<ItemWeapon*> (inventory[activeItemWeaponIndex]);
    } else {
        activeItemWeapon = nullptr;
    }

}

bool EntityAi::removeItem(Item* item, bool deleteItem) {
    if (item) {
        if (item == activeItemWeapon) {
            setActiveItemWeapon(nullptr);
        }
    }
    return Inventory::removeItem(item, deleteItem);
}

void EntityAi::setActiveItemWeapon(ItemWeapon* newItemWeapon) {

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
