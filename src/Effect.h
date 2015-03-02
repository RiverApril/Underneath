//
//  Effect.h
//  Underneath
//
//  Created by Braeden Atlee on 3/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Effect__
#define __Underneath__Effect__

#include "Global.h"
#include "Utility.h"
#include "Ui.h"

#define EFFECT_DELAY 1

typedef int EffectId;

const EffectId effFire = 0;
const EffectId effBleed = 1;
const EffectId effRegen = 2;

struct Effect{

	Effect(unsigned char* data, int* position){
		load(data, position);
	}

	Effect(EffectId eId, double timeEnd, double power, double currentTime){
		this->eId = eId;
		this->timeEnd = timeEnd;
		this->power = power;
		this->lastTime = currentTime;
	}

	string toString(){
		return formatString("id: %d, timeEnd: %.2f, power: %.2f, lastTime: %.2f", eId, timeEnd, power, lastTime);
	}

	void save(vector<unsigned char>* data){
		FileUtility::saveInt(data, eId);
		FileUtility::saveDouble(data, timeEnd);
		FileUtility::saveDouble(data, power);
		FileUtility::saveDouble(data, lastTime);
	}

	void load(unsigned char* data, int* position){
		eId = FileUtility::loadInt(data, position);
		timeEnd = FileUtility::loadDouble(data, position);
		power = FileUtility::loadDouble(data, position);
		lastTime = FileUtility::loadDouble(data, position);
	}

	EffectId eId = effFire;
	double timeEnd;
	double lastTime = 0;
	double power = 1;
};

bool operator== (const Effect a, const Effect b);

bool operator!= (const Effect a, const Effect b);

#endif /* defined(__Underneath__Effect__) */
