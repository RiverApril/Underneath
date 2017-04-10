//
//  EntityExplosive.hpp
//  Underneath
//
//  Created by Braeden Atlee on 4/11/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__EntityExplosive__
#define __Underneath__EntityExplosive__

#include "Entity.hpp"
#include "ItemExplosive.hpp"
#include "EntityMoving.hpp"

class EntityExplosive : public EntityMoving {
public:

    EntityExplosive();

    EntityExplosive(ItemExplosive* item, Point2 start, Point2 target, double z, int ignoreUID);

    EntityExplosive(ItemExplosive* item, Point2 pos);

    ~EntityExplosive();

    virtual void hit(Level* level, HitType h, Point2 p);

    virtual void activate(Level* level);

    virtual bool update(double deltaTime, double time, Level* level);

    virtual void save(vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual int getRenderDepth() {
        return -1;
    }

    virtual char getChar(unsigned long tick, Point2 pos, Level* lvl);

    virtual Ui::Color getFgColor(unsigned long tick, Point2 pos, Level* lvl);

    virtual string getName() {
        return expl ? expl->getName(false) : "Null Item";
    }

    virtual bool isHostile() {
        return expl ? expl->power > 0 : false;
    }

    virtual void load(vector<unsigned char>* data, int* position);
    
    
    
protected:
    ItemExplosive* expl = nullptr;
    int ignoreUID = -1;

    
};


#endif /* defined(__Underneath__EntityExplosive__) */
