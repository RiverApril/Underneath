//
//  Potion.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/2/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Potion.h"
#include "Utility.h"

void Potion::save(vector<unsigned char>* data){
	Item::save(data);

	FileUtility::saveInt(data, (int)effects.size());
	for (Effect e : effects){
		e.save(data);
	}
}

void Potion::load(unsigned char* data, int* position){
	Item::load(data, position);

	int size = FileUtility::loadInt(data, position);
	effects.clear();
	repeat(size, i){
		effects.push_back(Effect(data, position));
	}
}

Potion* Potion::cloneUnsafe(Potion* oldE, Potion* newE){

	Item::cloneUnsafe(oldE, newE);

	newE->effects = oldE->effects;

	return newE;

}
