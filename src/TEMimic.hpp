//
//  TEMimic.hpp
//  Underneath
//
//  Created by Braeden Atlee on 3/28/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__TEMimic__
#define __Underneath__TEMimic__

#include "TileEntity.hpp"

struct TEMimic : public TileEntity {
    static TEMimic* cloneUnsafe(TEMimic* oldE, TEMimic* newE);

    TEMimic() : TEMimic(Point2Neg1) {

    }

    TEMimic(Point2 pos) : TileEntity(pos) {

    }

    virtual ~TEMimic() {

    }

    virtual int getTileEntityTypeId() {
        return TILE_ENTITY_TYPE_MIMIC;
    }

    virtual string debugString(){
        return pos.toString();
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);
};

#endif /* defined(__Underneath__TEMimic__) */
