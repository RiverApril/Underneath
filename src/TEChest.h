//
//  TEChest.h
//  Underneath
//
//  Created by Braeden Atlee on 2/20/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__TEChest__
#define __Underneath__TEChest__

#include "TileEntity.h"
#include "Item.h"


struct TEChest : TileEntity{

    static TEChest* cloneUnsafe(TEChest* oldE, TEChest* newE);


    TEChest() : TEChest(Point2Neg1){

    }

    TEChest(Point2 pos) : TileEntity(pos){

    }

    virtual int getTileEntityTypeId(){
        return TILE_ENTITY_TYPE_CHEST;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    vector<Item*> inventory;
};

#endif /* defined(__Underneath__TEChest__) */
