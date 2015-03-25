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

    static ItemEntity* cloneUnsafe(ItemEntity* oldE, ItemEntity* newE);

    ItemEntity();

    ItemEntity(Item* item, Point2 p);

    ~ItemEntity();

    virtual bool update(double deltaTime, double time, Level* level);

    virtual void save(vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual int getRenderDepth(){
        return -1;
    }

    virtual string getName(){
        if(removed){
            return "Item Removed";
        }
        return item==nullptr?"Null Item":(item->qty==1?item->name:(formatString("%d %s", item->qty, plural(item->name).c_str())));
    }

    virtual Item* getItem(){
        return item;
    }

    virtual void load(unsigned char* data, int* position);


protected:
    Item* item = nullptr;

};

#endif /* defined(__Underneath__ItemEntity__) */
