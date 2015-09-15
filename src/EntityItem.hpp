//
//  EntityItem.h
//  Underneath
//
//  Created by Braeden Atlee on 11/15/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__EntityItem__
#define __Underneath__EntityItem__

#include "Entity.hpp"
#include "Item.hpp"

class EntityItem : public Entity {
public:

    static EntityItem* cloneUnsafe(EntityItem* oldE, EntityItem* newE);

    EntityItem();

    EntityItem(Item* item, Point2 p);

    ~EntityItem();

    virtual bool update(double deltaTime, double time, Level* level);

    virtual void save(vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual int getRenderDepth() {
        return -1;
    }

    virtual char getIcon(Point2 p, double time, Level* level);

    virtual string getName() {
        if (removed) {
            return "Item Removed";
        }
        return !item ? "Null Item" : ((item->qty!=1?formatString("%d ", item->qty):"")+item->getName(item->qty != 1).c_str());
    }

    virtual Item* getItem() {
        return item;
    }

    virtual void load(unsigned char* data, int* position);


protected:
    Item* item = nullptr;

};

#endif /* defined(__Underneath__EntityItem__) */
