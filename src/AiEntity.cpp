//
//  AiEntity.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "AiEntity.h"
#include "Math.h"
#include "Utility.h"
#include "Level.h"
#include "Verbalizer.h"
#include "ItemGenerator.h"
#include "ItemEntity.h"

AiEntity::AiEntity() : AiEntity("", aiNone, ' ', Point2Zero, Ui::C_WHITE, 1) {

}

AiEntity::AiEntity(std::string name, int aiFlags, char icon, Point2 startPos, Ui::Color colorCode, int maxHp) : Alive(name, icon, startPos, colorCode, maxHp) {
    this->ai = aiFlags;
}

AiEntity::~AiEntity() {

}

void AiEntity::runAi(double time, Level* level) {


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

    if (ai & aiFleeFromPlayer) {
        if (canSeeTarget) {

            Point2 playerPos = target->pos;

            speed.x = pos.x > playerPos.x ? -1 : (pos.x < playerPos.x ? 1 : 0);
            speed.y = pos.y > playerPos.y ? -1 : (pos.y < playerPos.y ? 1 : 0);

            if (speed.x == 0) {
                speed.x = rand() % 2 == 0 ? 1 : -1;
            }
            if (speed.y == 0) {
                speed.y = rand() % 2 == 0 ? 1 : -1;
            }
            speed = speed*-1;
        }
    }
    if (ai & aiAttackPlayer) {
        if (canSeeTarget) {
            lastKnownTargetPos = target->pos;
        }
        if (lastKnownTargetPos.x >= 0 && lastKnownTargetPos.y >= 0) {
            //console("LKTP: "+lastKnownTargetPos.toString()+"  TP: "+target->pos.toString()+"  P: "+pos.toString());
            Ranged* r = dynamic_cast<Ranged*> (activeWeapon);
            if (r && canSeeTarget && (distanceSquared(target->pos, pos) < (r->range * r->range))) {
                speed.x = 0;
                speed.y = 0;
            } else {
                //speed.x = pos.x > lastKnownTargetPos.x ? -1 : (pos.x < lastKnownTargetPos.x ? 1 : 0);
                //speed.y = pos.y > lastKnownTargetPos.y ? -1 : (pos.y < lastKnownTargetPos.y ? 1 : 0);
                vector<Point2> path = level->getPathTo(pos, lastKnownTargetPos, tileFlagAll, tileFlagSolid);
                if(!path.empty()){
                    console((path[0]-pos).toString());
                    speed.x = pos.x > path[0].x ? -1 : (pos.x < path[0].x ? 1 : 0);
                    speed.y = pos.y > path[0].y ? -1 : (pos.y < path[0].y ? 1 : 0);
                }
                
            }

        }

    }

    bool moved = tryToMoveRelative(speed, level);
    if (!moved) {
        moved = tryToMoveRelative(speed.xOnly(), level);
        if (!moved) {
            moved = tryToMoveRelative(speed.yOnly(), level);
        }
    }
    
    if (ai & aiAttackPlayer) {
        if(!moved && speed != Point2Zero){
            if (lastKnownTargetPos.x >= 0 && lastKnownTargetPos.y >= 0) {
                console("Failed to move");
                if(!canSeeTarget && level->canSee(pos, lastKnownTargetPos, agro ? viewDistance * agroViewDistanceMultiplier : viewDistance, false)){
                    console("Lost Target");
                    lastKnownTargetPos = Point2Neg1;
                }
            }
        }
    }

    bool attack = false;
    if (ai & aiAttackPlayer) {
        if (activeWeapon != nullptr) {
            if (distanceSquared(pos, target->pos) <= 1) {
                attack = true;
            } else {
                Ranged* r = dynamic_cast<Ranged*> (activeWeapon);
                if (r) {
                    if (level->canSee(pos, target->pos, Math::min(r->range, (double) viewDistance), false)) {
                        attack = true;
                    }

                }

            }
        }
    }
    if (attack) {
        while (lastAttackTime + activeWeapon->useDelay <= time) {
            double d = target->hurt(activeWeapon);
            Verbalizer::attack(this, target, activeWeapon, d);
            lastAttackTime += activeWeapon->useDelay;
        }
    }

    if (activeWeapon != nullptr) {
        while (lastAttackTime + activeWeapon->useDelay <= time) {
            lastAttackTime += activeWeapon->useDelay;
        }
    } else {
        lastAttackTime = time;
    }


}

double AiEntity::hurt(DamageType damageType, double amount, double damageMultiplier) {
    agro = true;
    return Alive::hurt(damageType, amount, damageMultiplier);
}

double AiEntity::hurt(Weapon* w, double damageMultiplier) {
    agro = true;
    return Alive::hurt(w, damageMultiplier);
}

bool AiEntity::update(double deltaTime, double time, Level* level) {

    while (lastMoveTime + moveDelay <= time) {
        runAi(time, level);
        lastMoveTime += moveDelay;
        if (level->canSee(target->pos, pos, level->currentWorld->currentPlayer->viewDistance, true)) {

            level->renderMenuGame(lastMoveTime);
            //usleep(10 * 1000);
        }
    }

    if (dead) {
        int xp = rand() % (int) maxHp;
        Verbalizer::defeatedEnemy(this, xp);

        level->currentWorld->currentPlayer->gainXp(xp);

        vector<Item*> drops = ItemGenerator::createRandLoots(level->getDifficulty(), level->getDifficulty() * 100, (rand() % 10) == 0 ? 1 : 0, (rand() % 5) == 0 ? 1 : 0, (rand() % 5) == 0 ? 2 : 0);
        //if(rand()%5==0){
        //drops.push_back(Item::clone(activeWeapon));
        //}
        for (Item* i : drops) {
            level->newEntity(new ItemEntity(i, pos));
        }
    }

    return Alive::update(deltaTime, time, level);
}

AiEntity* AiEntity::cloneUnsafe(AiEntity* oldE, AiEntity* newE) {

    Alive::cloneUnsafe(oldE, newE);

    newE->ai = oldE->ai;
    newE->agro = oldE->agro;
    newE->lastMoveTime = oldE->lastMoveTime;
    newE->lastAttackTime = oldE->lastAttackTime;

    forVector(oldE->inventory, i) {
        if (oldE->inventory[i] == oldE->activeWeapon) {
            newE->activeWeapon = dynamic_cast<Weapon*> (newE->inventory[i]);
            break;
        }
    }

    return newE;
}

int AiEntity::getEntityTypeId() {
    return ENTITY_TYPE_AIENTITY;
}

void AiEntity::save(std::vector<unsigned char>* data) {
    Alive::save(data);
    Utility::saveInt(data, ai);
    Utility::saveBool(data, agro);
    Utility::saveDouble(data, lastMoveTime);
    Utility::saveDouble(data, lastAttackTime);
    Point2::save(lastKnownTargetPos, data);

    int activeWeaponIndex = -1;

    forVector(inventory, i) {
        Item* ie = inventory[i];
        if (ie == activeWeapon) {
            activeWeaponIndex = i;
        }
    }
    Utility::saveInt(data, activeWeaponIndex);
}

void AiEntity::load(unsigned char* data, int* position) {
    Alive::load(data, position);
    ai = Utility::loadInt(data, position);
    agro = Utility::loadBool(data, position);
    lastMoveTime = Utility::loadDouble(data, position);
    lastAttackTime = Utility::loadDouble(data, position);
    lastKnownTargetPos = Point2::load(data, position);


    int activeWeaponIndex = Utility::loadInt(data, position);
    if (activeWeaponIndex != -1) {
        activeWeapon = dynamic_cast<Weapon*> (inventory[activeWeaponIndex]);
    } else {
        activeWeapon = nullptr;
    }

}

bool AiEntity::removeItem(Item* item, bool deleteItem) {
    if (item) {
        if (item == activeWeapon) {
            setActiveWeapon(nullptr);
        }
    }
    return Inventory::removeItem(item, deleteItem);
}

void AiEntity::setActiveWeapon(Weapon* newWeapon) {

    if (newWeapon == nullptr) {
        activeWeapon = nullptr;
        return;
    }

    for (Item* ie : inventory) {
        if (ie == newWeapon) {
            activeWeapon = dynamic_cast<Weapon*> (ie);
            return;
        }
    }
    inventory.push_back(newWeapon);
    activeWeapon = newWeapon;
}
