//
//  TEStair.h
//  Underneath
//
//  Created by Braeden Atlee on 2/20/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__TEStair__
#define __Underneath__TEStair__

#include "TileEntity.h"

struct TEStair : public TileEntity {
    static TEStair* cloneUnsafe(TEStair* oldE, TEStair* newE);

    TEStair() : TEStair(Point2Neg1, true, "") {

    }

    TEStair(Point2 pos, bool up, string levelName) : TileEntity(pos) {
        this->up = up;
        this->levelName = levelName;
    }

    virtual ~TEStair() {

    }

    virtual int getTileEntityTypeId() {
        return TILE_ENTITY_TYPE_STAIR;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    bool up = false;
    string levelName = "";
};

#endif /* defined(__Underneath__TEStair__) */
