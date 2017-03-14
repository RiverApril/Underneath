//
//  TileEntity.hpp
//  Underneath
//
//  Created by Braeden Atlee on 2/20/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__TileEntity__
#define __Underneath__TileEntity__

#include "Geometry.hpp"

const int TILE_ENTITY_TYPE_NONE = 0;
const int TILE_ENTITY_TYPE_STAIR = 1;
const int TILE_ENTITY_TYPE_CHEST = 2;
const int TILE_ENTITY_TYPE_MIMIC = 3;
const int TILE_ENTITY_TYPE_ALTER = 4;

struct TileEntity {
    static TileEntity* clone(TileEntity* oldI);

    TileEntity() : TileEntity(Point2Neg1) {

    }

    TileEntity(Point2 pos) {
        this->pos = pos;
    }

    virtual ~TileEntity() {

    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual int getTileEntityTypeId() {
        return TILE_ENTITY_TYPE_NONE;
    }

    static TileEntity* loadNew(vector<unsigned char>* data, int* position);

    Point2 pos = Point2(0, 0);

    virtual string debugString(){
        return pos.toString();
    }


protected:

    template<class Super, class Sub>
    static Sub* makeNewAndClone(Super* oldT);

    static TileEntity* cloneUnsafe(TileEntity* oldE, TileEntity* newE);
};

#endif /* defined(__Underneath__TileEntity__) */
