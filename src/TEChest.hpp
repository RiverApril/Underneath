//
//  TEChest.h
//  Underneath
//
//  Created by Braeden Atlee on 2/20/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__TEChest__
#define __Underneath__TEChest__

#include "TileEntity.hpp"
#include "Inventory.hpp"

struct TEChest : public TileEntity, public Inventory {
    static TEChest* cloneUnsafe(TEChest* oldE, TEChest* newE);

    TEChest() : TEChest(Point2Neg1) {

    }

    TEChest(Point2 pos) : TileEntity(pos) {

    }

    virtual ~TEChest() {

    }

    virtual int getTileEntityTypeId() {
        return TILE_ENTITY_TYPE_CHEST;
    }

    virtual string debugString(){
        return pos.toString()+", items("+to_string(inventory.size())+")";
    }
    
    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);
};

#endif /* defined(__Underneath__TEChest__) */
