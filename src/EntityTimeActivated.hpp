//
//  EntityTimeActivated.h
//  Underneath
//
//  Created by Braeden Atlee on 4/11/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__EntityTimeActivated__
#define __Underneath__EntityTimeActivated__

#include "Entity.hpp"
#include "ItemTimeActivated.hpp"

class EntityTimeActivated : public Entity {
public:

    static EntityTimeActivated* cloneUnsafe(EntityTimeActivated* oldE, EntityTimeActivated* newE);

    EntityTimeActivated();

    EntityTimeActivated(ItemTimeActivated* item, Point2 p);

    EntityTimeActivated(string name, TimeActivatedType timeActivatedType, double time, double power, double radius, Point2 p);

    ~EntityTimeActivated();

    virtual void activate(Level* level);

    virtual bool update(double deltaTime, double time, Level* level);

    virtual void save(vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual int getRenderDepth() {
        return -1;
    }

    virtual char getChar(unsigned long tick, Point2 pos, Level* lvl);

    virtual string getName() {
        return name;
    }

    virtual bool isHostile() {
        return timeActivatedType == timeActivatedBomb || timeActivatedType == timeActivatedWallBomb;
    }

    virtual void load(unsigned char* data, int* position);
    
    
    
protected:
    string name = "";
    TimeActivatedType timeActivatedType = 0;
    double timeLeft = 1;
    double power = 1;
    double radius = 4;

    
};


#endif /* defined(__Underneath__EntityTimeActivated__) */
