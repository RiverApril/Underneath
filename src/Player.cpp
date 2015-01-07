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
    setDelays();
    for(int i=0;i<abilityCount;i++){
        setNextLevelXp(i);
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

    if(tid == tileStairDown->getIndex() || tid == tileStairUp->getIndex()){

        for(Stair s : level->stairList){
            if(s.p == posOfTile){
                WorldLoader::changeLevel(level->currentWorld, s.p, s.levelName);
                return interactDelay;
            }
        }

    }else if(tid == tileOpenDoor->getIndex()){
        if(posOfTile != pos){
            level->setTile(posOfTile, tileDoor);
            return interactDelay;
        }
    }else if(tileList[tid]->hasFlag(tileFlagDoor)){
        level->setTile(posOfTile, tileOpenDoor);
        return interactDelay;
    }

    return 0;
}

double Player::interactWithEntity(Level* level, Entity* e, Point2 posOfEntity, Item* item){

    Alive* a = dynamic_cast<Alive*>(e);
    if(a != nullptr){
        if(item != nullptr){
            Weapon* weapon = dynamic_cast<Weapon*>(item);
            Spell* spell = dynamic_cast<Spell*>(item);
            if(weapon != nullptr){
                int d = a->hurt(weapon, level->currentWorld->worldTime);
                print("Dealt "+to_string(d)+" damage.");
                if(weapon->damageType == damMelee){
                    gainXp(iSTR, 1);
                }else if(weapon->damageType == damRanged){
                    gainXp(iDEX, 1);
                }
                return useDelay(item);
            }else if(spell != nullptr){
                if(mp >= spell->manaCost){
                    mp -= spell->manaCost;
                    int d = a->hurt(spell, level->currentWorld->worldTime);
                    print("Dealt "+to_string(d)+" damage.");
                    gainXp(iINT, 1);
                    return useDelay(item);
                }
                print("Not enough mana.");
            }
            return 0;
        }else{
            print("No item equiped.");
            return 0;
        }
    }

    ItemEntity* ie = dynamic_cast<ItemEntity*>(e);
    if(ie != nullptr){
        inventory.push_back(ie->getItem());
        level->removeEntity(e, true);
        print("Picked up item.");
        return interactDelay;
    }

    return 0;
}

double Player::waitUntilHealed(){
    //TODO
    debug("Not yet done, it just waits 5 time. It should wait until fully healed.");
    return waitDelay;
}


Player* Player::clone(Player* oldE, Player* newE){

    if(newE == nullptr){
        newE = new Player();
    }

    Alive::clone(oldE, newE);

    return newE;
}

int Player::getEntityTypeId(){
    return ENTITY_TYPE_PLAYER;
}

void Player::save(vector<unsigned char>* data){
    Alive::save(data);
}

void Player::load(unsigned char* data, int* position){
    Alive::load(data, position);
}

double Player::useDelay(Item* item){
    Spell* spell = dynamic_cast<Spell*>(item);
    Weapon* weapon = dynamic_cast<Weapon*>(item);
    double d = 0;
    if(spell != nullptr){
        return spell->castDelay;
    }else if(weapon != nullptr){
        d = weapon->weight/10;
    }
    gainXp(iAGI, 1.0 / 3.0);
    return (1-(*levels.AGI / *maxLevels.AGI))+d;

}


