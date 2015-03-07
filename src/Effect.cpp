//
//  Effect.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Effect.h"
#include "Utility.h"
#include "Ui.h"

Effect::Effect(unsigned char* data, int* position){
    load(data, position);
}

Effect::Effect(EffectId eId, double timeEnd, double power, double currentTime){
    this->eId = eId;
    this->timeEnd = timeEnd;
    this->power = power;
    this->lastTime = currentTime;
}

string Effect::toString(){
    return formatString("id: %d, timeEnd: %.2f, power: %.2f, lastTime: %.2f", eId, timeEnd, power, lastTime);
}

void Effect::save(vector<unsigned char>* data){
    FileUtility::saveInt(data, eId);
    FileUtility::saveDouble(data, timeEnd);
    FileUtility::saveDouble(data, power);
    FileUtility::saveDouble(data, lastTime);
    FileUtility::saveDouble(data, meta);
}

void Effect::load(unsigned char* data, int* position){
    eId = FileUtility::loadInt(data, position);
    timeEnd = FileUtility::loadDouble(data, position);
    power = FileUtility::loadDouble(data, position);
    lastTime = FileUtility::loadDouble(data, position);
    meta = FileUtility::loadDouble(data, position);
}

bool operator== (const Effect a, const Effect b){
	return a.eId == b.eId
    && a.lastTime == b.lastTime
    && a.power == b.power
    && a.timeEnd == b.timeEnd
    && a.meta == b.meta;
}

bool operator!= (const Effect a, const Effect b){
	return !(a == b);
}
