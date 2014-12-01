//
//  ItemEntity.h
//  Underneath
//
//  Created by Braeden Atlee on 11/15/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemEntity__
#define __Underneath__ItemEntity__

#include "Entity.h"
#include "Item.h"

class ItemEntity : public Entity{

public:

    static ItemEntity* clone(ItemEntity* oldE, ItemEntity* newE);

    ItemEntity();

    ItemEntity(Item* item, Point2 p);

    ~ItemEntity();

    virtual bool update(int tick, Level* level);

    virtual void save(vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual int getRenderDepth(){
        return -1;
    }

    virtual Item* getItem(){
        return item;
    }

    virtual void load(unsigned char* data, int* position);



protected:
    Item* item;
    
};

#endif /* defined(__Underneath__ItemEntity__) */
