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

string effectName(EffectId eid, double meta) {
    switch (eid) {
        case effDamage:
            return damageTypeName((DamageType) meta);

        case effHeal:
            return "Heal";

        case effBuffAttack:
            return "Buff "+damageTypeName((DamageType) meta);

        case effBuffDefense:
            return "Resist "+damageTypeName((DamageType) meta);

        case effLSD:
            return "Psychedelic";

        case effMemory:
            return "Where am I?";

        default:
            return "UNDEFINED";
    }
}

Ui::Color effectColor(EffectId eid, double meta) {
    switch (eid) {
        case effDamage:
            return damageTypeColor((DamageType) meta);

        case effHeal:
            return Ui::C_LIGHT_GREEN;

        case effBuffAttack:
            return Ui::C_LIGHT_BLUE;

        case effBuffDefense:
            return Ui::C_LIGHT_MAGENTA;
            
        default:
            return Ui::C_LIGHT_GREEN;
    }
}

Effect::Effect(unsigned char* data, int* position) {
    load(data, position);
}

Effect::Effect(EffectId eId, double timeLeft, double power, double meta) {
    this->eId = eId;
    this->timeLeft = timeLeft;
    this->power = power;
    this->meta = meta;
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
