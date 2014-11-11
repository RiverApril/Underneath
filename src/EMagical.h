//
//  EMagical.h
//  Underneath
//
//  Created by Braeden Atlee on 11/7/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__EMagical__
#define __Underneath__EMagical__

#include "Global.h"

class EMagical {

public:

	static void copy(shared_ptr<EMagical> oldE, shared_ptr<EMagical> newE);

    int getMp(){
        return mp;
    }

    int getMaxMp(){
        return maxMp;
    }

    void save(string* data);

    void load(char* data, int* position);
    
    int maxMp = 10;
    int mp = maxMp;
    
};

#endif /* defined(__Underneath__EMagical__) */
