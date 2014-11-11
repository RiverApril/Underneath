//
//  EMagical.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/7/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "EMagical.h"
#include "Utility.h"

void EMagical::copy(shared_ptr<EMagical> oldE, shared_ptr<EMagical> newE){
    newE->maxMp = oldE->maxMp;
    newE->mp = oldE->mp;
}

void EMagical::save(string* data){
    Utility::saveInt(data, maxMp);
    Utility::saveInt(data, mp);

}

void EMagical::load(char* data, int* position){
    maxMp = Utility::loadInt(data, position);
    mp = Utility::loadInt(data, position);
}