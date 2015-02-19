//
//  Player.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Player.h"
#include "Level.h"
#include "ItemEntity.h"


Player::Player() : Player("", ' ', Point2Zero, Ui::C_WHITE, Abilities<int>()){

}

Player::Player(string name, char icon, Point2 startPos, Ui::Color colorCode, Abilities<int> startAbilities) : Alive(name, icon, startPos, colorCode) {
    levels = startAbilities;
    viewDistance = 14;
    updateVariablesForAbilities();
    for(int i=0;i<abilityCount;i++){
        setNextLevelXp(i);
        debug(to_string(levels.list[i]));
    }
}

Player::~Player() {

}

bool Player::update(double time, Level* level) {
    if(dead){
        level->currentWorld->currentPlayer = nullptr;
    }
    return Alive::update(time, level);
}

double Player::moveAbsalute(Point2 p, Level* level){
    if(tryToMoveAbsalute(p, level)){
        gainXp(iSPD, .1);
        return moveDelay;
    }else{
        return interact(level, p, true, activeWeapon);
    }
    return 0;
}

double Player::moveRelative(Point2 p, Level* level) {
    return moveAbsalute(p+pos, level);
}

double Player::interact(Level* level, Point2 posToInteract, bool needToBeSolid, Item* item){
    int tid = level->indexAt(posToInteract);
    for(Entity* e : level->entityList){
        if(e->uniqueId != uniqueId){
            if(e->pos == posToInteract){
                if(!needToBeSolid || e->isSolid()){
                    double d = interactWithEntity(level, e, posToInteract, item);
                    if(d>0){
                        return d;
                    }
                }
            }
        }
    }
    return interactWithTile(level, tid, posToInteract, item);
}

double Player::interactWithTile(Level* level, int tid, Point2 posOfTile, Item* item){

    if(distanceSquared(posOfTile, pos) <= 1){

        if(tid == Tiles::tileStairDown->getIndex() || tid == Tiles::tileStairUp->getIndex()){

            for(Stair s : level->stairList){
                if(s.p == posOfTile){
                    WorldLoader::changeLevel(level->currentWorld, s.p, s.levelName);
                    return interactDelay;
                }
            }

        }else if(tid == Tiles::tileOpenDoor->getIndex()){
            if(posOfTile != pos){
                level->setTile(posOfTile, Tiles::tileDoor);
                return interactDelay;
            }
        }else if(Tiles::tileList[(size_t)tid]->hasFlag(tileFlagDoor)){
            level->setTile(posOfTile, Tiles::tileOpenDoor);
            return interactDelay;
        }
    }

    return 0;
}

double Player::interactWithEntity(Level* level, Entity* e, Point2 posOfEntity, Item* item){

    if(e->removed){
        return 0;
    }

    Alive* a = dynamic_cast<Alive*>(e);
    if(a){
        if(item != nullptr){
            Weapon* weapon = dynamic_cast<Weapon*>(item);
            Ranged* ranged = dynamic_cast<Ranged*>(item);
            Spell* spell = dynamic_cast<Spell*>(item);

            if(ranged){
                if(distanceSquared(pos, posOfEntity) > ranged->range*ranged->range){
                    console("Out of range!");
                    return 0;
                }
            }

            double x = 1;
            if(weapon){
                if(weapon->damageType == damMelee){
                    x = (double)levels[iSTR] / (double)maxLevels[iSTR];
                    gainXp(iSTR, 1);
                }else if(weapon->damageType == damRanged){
                    x = (double)levels[iDEX] / (double)maxLevels[iDEX];
                    gainXp(iDEX, 1);
                }else if(weapon->damageType == damMagic){
                    x = (double)levels[iINT] / (double)maxLevels[iINT];
                    gainXp(iINT, 1);
                }
            }

            if(spell){
                if(mp >= spell->manaCost){
                    mp -= spell->manaCost;

                    double d = a->hurt(spell, level->currentWorld->worldTime, x+1);
                    consolef("Dealt %.2f damage.", d);

                    return useDelay(item);
                }
                console("Not enough mana.");
            }

            if(weapon){

                double d = a->hurt(weapon, level->currentWorld->worldTime, x+1);
                consolef("Dealt %.2f damage.", d);

                return useDelay(item);
            }

            return 0;
        }else{
            console("No item equiped.");
            return 0;
        }
    }

    if(distanceSquared(posOfEntity, pos) <= 1){

        ItemEntity* ie = dynamic_cast<ItemEntity*>(e);

        if(ie){
            Item* iei = ie->getItem();
            if(iei != nullptr){
                debug("Picking up item: "+iei->name);
                if(pickupItem(iei)){
                    level->removeEntity(e, true);
                    return interactDelay;
                }
            }
            level->removeEntity(e, true);
            return 0;
        }

    }

    return 0;
}

double Player::waitUntilHealed(){
    //TODO
    debug("Not yet done, it just waits 5 time. It should wait until fully healed.");
    return waitDelay;
}


Player* Player::cloneUnsafe(Player* oldE, Player* newE){

    Alive::cloneUnsafe(oldE, newE);

    return newE;
}

int Player::getEntityTypeId(){
    return ENTITY_TYPE_PLAYER;
}

void Player::save(vector<unsigned char>* data){
    Alive::save(data);
    levels.save(data);
    xp.save(data);
    nextLevelXp.save(data);

}

void Player::load(unsigned char* data, int* position){
    Alive::load(data, position);
    levels.load(data, position);
    xp.load(data, position);
    nextLevelXp.load(data, position);
}

double Player::useDelay(Item* item){
    Weapon* weapon = dynamic_cast<Weapon*>(item);
    double d = 0;
    if(weapon){
        return weapon->useDelay;
    }else{
        d = .1;
    }
    return (1-(levels[iAGI] / maxLevels[iAGI]))+d;

}


