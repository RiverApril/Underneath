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

Player::Player(std::string name, char icon, Point2 startPos, Ui::color colorCode) : Alive(name, icon, startPos, colorCode) {
}

Player::~Player() {

}

bool Player::update(int tick, Level* level) {
    return Alive::update(tick, level);
}

bool Player::moveExact(Point2 p, Level* level){
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

bool Player::moveRelative(Point2 p, Level* level) {
    return moveExact(p+*pos, level);
}

bool Player::use(Level* level){

    int tid = level->indexAt(*pos);
    if(tid == tileStairDown->getIndex()){
		
    }else if(tid == tileStairUp->getIndex()){

    }
    
    return false;
}


Player* Player::clone(Player* oldE, Player* newE){

    if(newE == nullptr){
        newE = new Player();
    }

    Alive::clone(oldE, newE);

    //Player Specific

    return newE;
}

void Player::save(std::string* data){
    Alive::save(data);
    //debug("Save Player hp: "+std::to_string(hp));
}

int Player::getEntityTypeId(){
    return ENTITY_TYPE_PLAYER;
}

void Player::load(char* data, int* position){
    Alive::load(data, position);
}


