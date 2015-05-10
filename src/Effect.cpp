//
//  Effect.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Effect.hpp"
#include "Utility.hpp"
#include "Ui.hpp"

Effect::Effect(unsigned char* data, int* position) {
    load(data, position);
}

Effect::Effect(EffectId eId, double timeLeft, double power) {
    this->eId = eId;
    this->timeLeft = timeLeft;
    this->power = power;
}

Effect Effect::setMeta(double meta) {
    this->meta = meta;
    return *this;
}

string Effect::toString() {
    return formatString("id: %d, timeLeft: %.2f, power: %.2f", eId, timeLeft, power);
}

void Effect::save(vector<unsigned char>* data) {
    Utility::saveInt(data, eId);
    Utility::saveDouble(data, timeLeft);
    Utility::saveDouble(data, power);
    Utility::saveDouble(data, meta);
}

void Effect::load(unsigned char* data, int* position) {
    eId = Utility::loadInt(data, position);
    timeLeft = Utility::loadDouble(data, position);
    power = Utility::loadDouble(data, position);
    meta = Utility::loadDouble(data, position);
}

bool operator==(const Effect a, const Effect b) {
    return a.eId == b.eId
            && a.timeLeft == b.timeLeft
            && a.power == b.power
            && a.meta == b.meta;
}

bool operator!=(const Effect a, const Effect b) {
    return !(a == b);
}
