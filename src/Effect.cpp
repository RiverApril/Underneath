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
#include "Abilities.hpp"

string effectName(EffectId eid, double meta) {
    switch (eid) {
        case effDamage:
            return damageTypeName((DamageType) meta);

        case effHeal:
            return "Heal";

        case effMultAttack:
            return "Buff "+damageTypeName((DamageType) meta);

        case effMultRecivedDamage:
            return "Resist "+damageTypeName((DamageType) meta);

        case effLSD:
            return "Psychedelic";

        case effMemory:
            return "Where am I?";

        case effPurity:
            return "Purity";

        case effBuffAbility:
            return "Buff "+abilityNames[(size_t)meta];

        case effStun:
            return "Stun";

        default:
            return "UNDEFINED";
    }
}

Ui::Color effectColor(EffectId eid, double meta) {
    switch (eid) {
        case effDamage:
            return damageTypeColor((DamageType) meta);

        case effHeal:
            return C_LIGHT_GREEN;

        case effMultAttack:
            return C_LIGHT_BLUE;

        case effMultRecivedDamage:
            return C_LIGHT_BLUE;

        case effLSD:
            return C_LIGHT_MAGENTA;

        case effMemory:
            return C_LIGHT_GRAY;

        case effPurity:
            return C_LIGHT_BLUE;

        case effBuffAbility:
            return C_LIGHT_YELLOW;

        case effStun:
            return C_LIGHT_YELLOW;

        default:
            return C_LIGHT_GREEN;
    }
}

string effectPowerString(EffectId eid, double power){
    switch (eid) {
        case effDamage:
            return formatString("%.2f", power);

        case effHeal:
            return formatString("%.2f", power);

        case effMultAttack:
            return formatString("%.2f", power);

        case effMultRecivedDamage:
            return formatString("%.2f", power);

        case effLSD:
            return "";

        case effMemory:
            return "";

        case effPurity:
            return "";

        case effBuffAbility:
            return formatString("%.2f", power);

        case effStun:
            return "";
            
        default:
            return formatString("%.2f", power);
    }
}

bool isBadEffect(Effect e){
    switch (e.eId) {
        case effDamage:
            return e.power > 0;

        case effHeal:
            return e.power < 0;

        case effMultAttack:
            return e.power < 0;

        case effMultRecivedDamage:
            return e.power < 0;

        case effLSD:
            return true;

        case effMemory:
            return true;

        case effPurity:
            return false;

        case effBuffAbility:
            return e.power < 0;

        case effStun:
            return true;

        default:
            return false;
    }
}

Effect::Effect(){
    
}

Effect::Effect(vector<unsigned char>* data, int* position) {
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

void Effect::load(vector<unsigned char>* data, int* position) {
    eId = (EffectId)Utility::loadInt(data, position);
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
