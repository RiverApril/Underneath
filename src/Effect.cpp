//
//  Effect.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Effect.h"

bool operator== (const Effect a, const Effect b){
	return a.eId == b.eId &&
		a.lastTime == b.lastTime &&
		a.power == b.power &&
		a.timeEnd == b.timeEnd;
}

bool operator!= (const Effect a, const Effect b){
	return !(a == b);
}