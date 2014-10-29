//
//  Player.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Player.h"

Player::Player(std::string name, char icon, Point2 startPos, Ui::color colorCode) : Entity(icon, startPos, colorCode) {
    this->name = name;
}

Player::~Player() {

}

bool Player::update(int tick, Level* level) {
    return Entity::update(tick, level);
}

void Player::move(Point2 p, Level* level) {
    if(tryToMove(p, level)){

    }else{
        int tid = level->indexAt(*pos+p);
        if(tid == tileDoor->getIndex() || tid == tileSecretDoor->getIndex()){
            level->setTile(*pos+p, tileOpenDoor);
        }
    }
}

bool Player::use(Level* level){

    int tid = level->indexAt(*pos);
    if(tid == tileStairDown->getIndex()){
		
    }else if(tid == tileStairUp->getIndex()){

    }
    
    return false;
}