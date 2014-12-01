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

Player::Player() : Player("", ' ', Point2Zero, Ui::C_WHITE){

}

Player::Player(string name, char icon, Point2 startPos, Ui::color colorCode) : Alive(name, icon, startPos, colorCode) {
}

Player::~Player() {

}

bool Player::update(int tick, Level* level) {
    if(dead){
        level->currentWorld->currentPlayer = nullptr;
    }
    return Alive::update(tick, level);
}

bool Player::moveAbsalute(Point2 p, Level* level){
    if(tryToMoveAbsalute(p, level)){
        return true;
    }else{
        return interact(level, p, true);
    }
    return false;
}

bool Player::moveRelative(Point2 p, Level* level) {
    return moveAbsalute(p+pos, level);
}

bool Player::interact(Level* level, Point2 posToInteract, bool needToBeSolid){
    int tid = level->indexAt(posToInteract);
    for(Entity* e : level->entityList){
        if(e->uniqueId != uniqueId){
            if(e->pos == posToInteract){
                if(!needToBeSolid || e->isSolid()){
                    if(interactWithEntity(level, e, posToInteract)){
                        return true;
                    }
                }
            }
        }
    }
    return interactWithTile(level, tid, posToInteract);
}

bool Player::interactWithTile(Level* level, int tid, Point2 posOfTile){

    if(tid == tileStairDown->getIndex()){
		
    }else if(tid == tileStairUp->getIndex()){

    }else if(tid == tileOpenDoor->getIndex()){
        if(posOfTile != pos){
        	level->setTile(posOfTile, tileDoor);
        }
    }else if(tileList[tid]->hasFlag(tileFlagDoor)){
        level->setTile(posOfTile, tileOpenDoor);
        return true;
    }

    return false;
}

bool Player::interactWithEntity(Level* level, Entity* e, Point2 posOfEntity){

    Alive* a = dynamic_cast<Alive*>(e);
    if(a != nullptr){
        if(activeWeapon != nullptr){
            print("Dealt "+to_string(a->hurt(activeWeapon))+" damage.");
        }else{
            print("No weapon eqiped.");
        }
        return true;
    }

    ItemEntity* ie = dynamic_cast<ItemEntity*>(e);
    if(ie != nullptr){
        inventory.push_back(ie->getItem());
        level->removeEntity(e, true);
        print("Picked up item.");
        return true;
    }

    return false;
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


