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

bool Player::moveExact(Point2 p, shared_ptr<Level> level){
    if(tryToMove(p-*pos, level)){
        return true;
    }else{
        int tid = level->indexAt(p);
        if(tid == tileDoor->getIndex() || tid == tileSecretDoor->getIndex()){
            level->setTile(p, tileOpenDoor);
            return true;
        }
    }
    return false;
}

bool Player::moveRelative(Point2 p, shared_ptr<Level> level) {
    return moveExact(p+*pos, level);
}

bool Player::use(shared_ptr<Level> level){

    int tid = level->indexAt(*pos);
    if(tid == tileStairDown->getIndex()){
		
    }else if(tid == tileStairUp->getIndex()){

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


