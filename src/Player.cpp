//
//  Player.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Player.h"

Player::Player(std::string name, std::string icon, char iconAlt, const Point2 startPos, Ui::color colorCode) : Entity(icon, iconAlt, startPos, colorCode) {
    this->name = name;
}

Player::~Player() {

}

bool Player::update(int tick, Level* level) {
    return Entity::update(tick, level);
}

void Player::move(Point2 p, Level* level) {
    tryToMove(p, level);

}
