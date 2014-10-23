//
//  Player.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Player.h"

Player::Player(std::string name, std::string icon, const Geometry::Point2* startPos, Ui::color colorCode) : Entity(icon, startPos, colorCode){
    this->name = name;
}

Player::~Player(){

}

bool Player::update(int tick, Level* level){
    return Entity::update(tick, level);
}

void Player::move(int x, int y, Level* level){
	tryToMove(x, y, level);

}