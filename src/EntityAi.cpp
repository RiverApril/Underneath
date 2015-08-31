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
#include "EntityItem.hpp"
#include "EntityTimeActivated.hpp"
#include "Animator.hpp"
#include "Settings.hpp"

EntityAi::EntityAi() : EntityAi("", aiNone, ' ', Point2Zero, Ui::C_WHITE, 1) {

}

EntityAi::EntityAi(std::string name, int aiFlags, char icon, Point2 startPos, Ui::Color colorCode, int maxHp) : EntityAlive(name, icon, startPos, colorCode, maxHp) {
    this->ai = aiFlags;
}

EntityAi::~EntityAi() {

}

void EntityAi::runAi(double time, Level* level) {


    if (target == nullptr) {
        target = level->currentWorld->currentPlayer;
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
    bool canSeeTarget = level->canSee(pos, target->pos, agro ? viewDistance * agroViewDistanceMultiplier : viewDistance, false);
    if (!canSeeTarget && agro) {
        agro = false;
    }
    if(canSeeTarget && !agro){
        agro = true;
    }

    if (ai & aiFleeFromEntityPlayer) {
        if (canSeeTarget) {

            speed.x = pos.x > target->pos.x ? 1 : (pos.x < target->pos.x ? -1 : 0);
            speed.y = pos.y > target->pos.y ? 1 : (pos.y < target->pos.y ? -1 : 0);

            if (speed.x == 0) {
                speed.x = rand() % 2 == 0 ? 1 : -1;
            }
            if (speed.y == 0) {
                speed.y = rand() % 2 == 0 ? 1 : -1;
            }
        }
    }
    if (ai & aiAttackEntityPlayer) {
        if (canSeeTarget) {
            lastKnownTargetPos = target->pos;
        }
        if (lastKnownTargetPos.x >= 0 && lastKnownTargetPos.y >= 0) {
            //debug("LKTP: "+lastKnownTargetPos.toString()+"  TP: "+target->pos.toString()+"  P: "+pos.toString());
            ItemRanged* r = dynamic_cast<ItemRanged*> (activeItemWeapon);
            if (r && canSeeTarget && (distanceSquared(target->pos, pos) < (r->range * r->range))) {
                speed.x = 0;
                speed.y = 0;
            } else {
                //speed.x = pos.x > lastKnownTargetPos.x ? -1 : (pos.x < lastKnownTargetPos.x ? 1 : 0);
                //speed.y = pos.y > lastKnownTargetPos.y ? -1 : (pos.y < lastKnownTargetPos.y ? 1 : 0);
                vector<Point2> path = level->getPathTo(lastKnownTargetPos, pos, tileFlagAll, tileFlagSolid, true);
                if(!path.empty()){
                    if(Settings::showFollowPaths){
                        for(Point2 point : path){
                            EntityTimeActivated* e = new EntityTimeActivated("path", timeActivatedDud, 2, 0, 0, point);
                            e->fgColor = Ui::C_LIGHT_GREEN;
                            level->newEntity(e);
                        }
                        EntityTimeActivated* e = new EntityTimeActivated("lastKnownTargetPos", timeActivatedDud, 2, 0, 0, lastKnownTargetPos);
                        e->fgColor = Ui::C_LIGHT_MAGENTA;
                        level->newEntity(e);
                    }
                    debugf("%s: %s", name.c_str(), (path[0]-pos).toString().c_str());

                    speed = path[0]-pos;

                    if (speed.x == 0 && speed.y == 0) {
                        speed.x = rand() & 1 ? 1 : -1;
                        speed.y = rand() & 1 ? 1 : -1;
                    }

                }

            }

        }

    }
    bool movedOrDontNeedToMove = true;
    while (lastMoveTime + moveDelay <= time) {
        movedOrDontNeedToMove = tryToMoveRelative(speed.xOnly(), level) || movedOrDontNeedToMove;
        movedOrDontNeedToMove = tryToMoveRelative(speed.yOnly(), level) || movedOrDontNeedToMove;
        lastMoveTime += moveDelay;
    }

    if (ai & aiAttackEntityPlayer) {
        if(!movedOrDontNeedToMove && speed != Point2Zero){
            if (lastKnownTargetPos.x >= 0 && lastKnownTargetPos.y >= 0) {
                debug("Failed to move");
                if(!canSeeTarget && level->canSee(pos, lastKnownTargetPos, agro ? viewDistance * agroViewDistanceMultiplier : viewDistance, false)){
                    debug("Lost Target");
                    lastKnownTargetPos = Point2Neg1;
                }
            }
        }
    }

    bool attack = false;
    if (ai & aiAttackEntityPlayer) {
        if (activeItemWeapon != nullptr) {
            if (distanceSquared(pos, target->pos) <= 1) {
                attack = true;
            } else {
                ItemRanged* r = dynamic_cast<ItemRanged*> (activeItemWeapon);
                if (r) {
                    if (level->canSee(pos, target->pos, Math::min(r->range, (double) viewDistance), false)) {
                        attack = true;
                    }

                }

            }
        }
    }
    if (attack) {
        while (lastAttackTime + activeItemWeapon->useDelay <= time) {
            double d = target->hurt(activeItemWeapon, getAttackMultiplierFromEffects(activeItemWeapon->damageType));
            Verbalizer::attack(this, target, activeItemWeapon, d);
            ItemCombatSpell* spell = dynamic_cast<ItemCombatSpell*>(activeItemWeapon);
            BasicIcon* icon;
            if(spell){
                icon = new BasicIcon('*', damageTypeColor(activeItemWeapon->damageType), Ui::C_BLACK);
            	Animator::renderRangedAttack(pos, target->pos, icon, level, 8);
            }else{
                icon = new BasicIcon('*', damageTypeColor(activeItemWeapon->damageType), Ui::C_BLACK);
                Animator::renderRangedAttack(pos, target->pos, icon, level, 1);
            }
            delete icon;
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

double EntityAi::hurt(DamageType damageType, double amount, double damageMultiplier) {
    agro = true;
    return EntityAlive::hurt(damageType, amount, damageMultiplier);
}

double EntityAi::hurt(ItemWeapon* w, double damageMultiplier) {
    agro = true;
    return EntityAlive::hurt(w, damageMultiplier);
}

bool EntityAi::update(double deltaTime, double time, Level* level) {

    if (dead) {
        dropLoots(level);
    }else{
        runAi(time, level);
        
    }

    return EntityAlive::update(deltaTime, time, level);
}

void EntityAi::dropLoots(Level* level){

    if(!isHostile()){
        return;
    }

    int xp = rand() % (int) maxHp;
    Verbalizer::defeatedEnemy(this, xp);

    level->currentWorld->currentPlayer->gainXp(xp);

    vector<Item*> drops = ItemGenerator::createRandLoots(level->getDifficulty(), level->getDifficulty() * 100, (rand() % 10) == 0 ? 1 : 0, (rand() % 10) == 0 ? 1 : 0, (rand() % 5) == 0 ? 2 : 0, 20);
    //if(rand()%5==0){
    //drops.push_back(Item::clone(activeItemWeapon));
    //}
    for (Item* i : drops) {
        level->newEntity(new EntityItem(i, pos));
    }
}

EntityAi* EntityAi::cloneUnsafe(EntityAi* oldE, EntityAi* newE) {

    EntityAlive::cloneUnsafe(oldE, newE);

    newE->ai = oldE->ai;
    newE->agro = oldE->agro;
    newE->lastMoveTime = oldE->lastMoveTime;
    newE->lastAttackTime = oldE->lastAttackTime;

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

void EntityAi::save(std::vector<unsigned char>* data) {
    EntityAlive::save(data);
    Utility::saveInt(data, ai);
    Utility::saveBool(data, agro);
    Utility::saveDouble(data, lastMoveTime);
    Utility::saveDouble(data, lastAttackTime);
    Point2::save(lastKnownTargetPos, data);

    int activeItemWeaponIndex = -1;

    forVector(inventory, i) {
        Item* ie = inventory[i];
        if (ie == activeItemWeapon) {
            activeItemWeaponIndex = i;
        }
    }
    Utility::saveInt(data, activeItemWeaponIndex);
}

void EntityAi::load(unsigned char* data, int* position) {
    EntityAlive::load(data, position);
    ai = Utility::loadInt(data, position);
    agro = Utility::loadBool(data, position);
    lastMoveTime = Utility::loadDouble(data, position);
    lastAttackTime = Utility::loadDouble(data, position);
    lastKnownTargetPos = Point2::load(data, position);


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
