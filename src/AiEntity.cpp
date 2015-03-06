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

AiEntity::AiEntity() : AiEntity("", aiNone, ' ', Point2Zero, Ui::C_WHITE){

}

AiEntity::AiEntity(std::string name, int aiFlags, char icon, Point2 startPos, Ui::Color colorCode, int maxHp) : Alive(name, icon, startPos, colorCode, maxHp) {
    this->ai = aiFlags;
    this->moveDelay = .9;
}

AiEntity::~AiEntity() {
    
}

void AiEntity::runAi(double time, Level* level) {


    if(target == nullptr){
        target = level->currentWorld->currentPlayer;
    }

    Point2 speed;

    if(ai & aiMoveRandom) {
        if(rand()%2==0){
        	speed.x = (rand()%3-1);
            speed.y = 0;
        }else{
            speed.x = 0;
        	speed.y = (rand()%3-1);
        }
    }
    bool canSeeTarget = level->canSee(pos, target->pos, agro?viewDistance*agroViewDistanceMultiplier:viewDistance, false);
    if(!canSeeTarget && agro){
        agro = false;
    }

    if(ai & aiFleeFromPlayer){
        if(canSeeTarget){

            Point2 playerPos = target->pos;

            speed.x = pos.x>playerPos.x?-1:(pos.x<playerPos.x?1:0);
            speed.y = pos.y>playerPos.y?-1:(pos.y<playerPos.y?1:0);

            if(speed.x == 0){
                speed.x = rand()%2==0?1:-1;
            }
            if(speed.y == 0){
                speed.y = rand()%2==0?1:-1;
            }
            speed = speed*-1;
        }
    }
    if(ai & aiAttackPlayer){
        if(canSeeTarget){
            lastKnownTargetPos = target->pos;
        }
        if(lastKnownTargetPos != Point2(-1, -1)){
            Ranged* r = dynamic_cast<Ranged*>(activeWeapon);
            if(r && canSeeTarget && (distanceSquared(target->pos, pos) < (r->range * r->range))){
                speed.x = 0;
                speed.y = 0;
            }else{
            	speed.x = pos.x>lastKnownTargetPos.x?-1:(pos.x<lastKnownTargetPos.x?1:0);
            	speed.y = pos.y>lastKnownTargetPos.y?-1:(pos.y<lastKnownTargetPos.y?1:0);
            }

        }

    }

    bool moved = tryToMoveRelative(speed, level);
    if(!moved){
        moved = tryToMoveRelative(speed.xOnly(), level);
        if(!moved){
            moved = tryToMoveRelative(speed.yOnly(), level);
        }
    }

    bool attack = false;
    if(!moved){
        if(ai & aiAttackPlayer){
            if(activeWeapon != nullptr){
                if(distanceSquared(pos, target->pos) <= 1){
                    
                    attack = true;
                }else{
                    Ranged* r = dynamic_cast<Ranged*>(activeWeapon);
                    if(r){
                        if(level->canSee(pos, target->pos, Math::min(r->range, (double)viewDistance), false)){
                            attack = true;
                        }
                        
                    }
                    
                }
            }
        }
        if(attack){
            while(lastAttackTime + activeWeapon->useDelay <= time){
                double d = target->hurt(activeWeapon, time);
                Verbalizer::attack(this, target, activeWeapon, d);
                lastAttackTime += activeWeapon->useDelay;
            }
        }
    }

    if(!attack){
        if(activeWeapon != nullptr){
            while(lastAttackTime + activeWeapon->useDelay <= time){
                lastAttackTime += activeWeapon->useDelay;
            }
        }else{
            while(lastAttackTime + 1 <= time){
                lastAttackTime += 1;
            }
        }
    }


}

double AiEntity::hurt(double amount, double damageMultiplier){
    agro = true;
    return Alive::hurt(amount, damageMultiplier);
}

double AiEntity::hurt(Weapon* w, double time, double damageMultiplier){
    agro = true;
    return Alive::hurt(w, time, damageMultiplier);
}

bool AiEntity::update(double deltaTime, double time, Level* level) {

    while(lastMoveTime+moveDelay<=time){
        runAi(time, level);
        lastMoveTime += moveDelay;
        if(level->canSee(target->pos, pos, level->currentWorld->currentPlayer->viewDistance, true)){

            level->renderMenuGame(lastMoveTime);
            //usleep(10 * 1000);
        }
    }

    if(dead){
        int xp = rand()%(int)maxHp;
        Verbalizer::defeatedEnemy(this, xp);
        level->currentWorld->currentPlayer->gainXp(xp);
    }

    return Alive::update(deltaTime, time, level);
}

AiEntity* AiEntity::cloneUnsafe(AiEntity* oldE, AiEntity* newE){

    Alive::cloneUnsafe(oldE, newE);

    newE->ai = oldE->ai;
    newE->agro = oldE->agro;
    
    return newE;
}

int AiEntity::getEntityTypeId(){
    return ENTITY_TYPE_AIENTITY;
}

void AiEntity::save(std::vector<unsigned char>* data){
    Alive::save(data);
    FileUtility::saveInt(data, ai);
    FileUtility::saveBool(data, agro);
    Point2::save(lastKnownTargetPos, data);
}

void AiEntity::load(unsigned char* data, int* position){
    Alive::load(data, position);
    ai = FileUtility::loadInt(data, position);
    agro = FileUtility::loadBool(data, position);
    lastKnownTargetPos = Point2::load(data, position);
}


