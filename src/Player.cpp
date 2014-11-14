//
//  Player.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Player.h"
#include "Level.h"

Player::Player() : Player("", ' ', Point2Zero, Ui::C_WHITE){

}

Player::Player(string name, char icon, Point2 startPos, Ui::color colorCode) : Alive(name, icon, startPos, colorCode) {
}

Player::~Player() {

}

bool Player::update(int tick, shared_ptr<Level> level) {
    return Alive::update(tick, level);
}

bool Player::moveAbsalute(Point2 p, shared_ptr<Level> level){
    if(tryToMoveAbsalute(p, level)){
        return true;
    }else{
        return interact(level, p);
    }
    return false;
}

bool Player::moveRelative(Point2 p, shared_ptr<Level> level) {
    return moveAbsalute(p+*pos, level);
}

bool Player::interact(shared_ptr<Level> level, Point2 posToInteract){
    int tid = level->indexAt(posToInteract);
    int i;
    forVector(level->entityList, i){
        shared_ptr<Entity> e = level->entityList[i];
        if(e->uniqueId != uniqueId){
            if(level->entityList[i]->getPos() == posToInteract){
                if(e->isSolid()){
                    if(interactWithEntity(level, e, posToInteract)){
                        return true;
                    }
                }
            }
        }
    }
    return interactWithTile(level, tid, posToInteract);
}

bool Player::interactWithTile(shared_ptr<Level> level, int tid, Point2 posOfTile){

    if(tid == tileStairDown->getIndex()){
		
    }else if(tid == tileStairUp->getIndex()){

    }else if(tid == tileOpenDoor->getIndex()){
        if(posOfTile != *pos){
        	level->setTile(posOfTile, tileDoor);
        }
    }else if(tileList[tid]->hasFlag(tileFlagDoor)){
        level->setTile(posOfTile, tileOpenDoor);
        return true;
    }

    return false;
}

bool Player::interactWithEntity(shared_ptr<Level> level, shared_ptr<Entity> e, Point2 posOfEntity){
    shared_ptr<Alive> a = dynamic_pointer_cast<Alive>(e);
    if(a != nullptr){
        if(activeWeapon != nullptr){
            print("Dealt "+to_string(a->hurt(activeWeapon))+" damage.");
        }else{
            print("No weapon eqiped.");
        }
        return true;
    }
    return false;
}


shared_ptr<Player> Player::clone(shared_ptr<Player> oldE, shared_ptr<Player> newE){

    if(newE == nullptr){
        newE = shared_ptr<Player>(new Player());
    }

    Alive::clone(oldE, newE);

    EMagical::copy(oldE, newE);

    return newE;
}

int Player::getEntityTypeId(){
    return ENTITY_TYPE_PLAYER;
}

void Player::save(string* data){
    Alive::save(data);
    EMagical::save(data);
}

void Player::load(char* data, int* position){
    Alive::load(data, position);
    EMagical::load(data, position);
}


