//
//  TEAlter.hpp
//  Underneath
//
//  Created by Braeden Atlee on 3/11/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef TEAlter_hpp
#define TEAlter_hpp

#include "TileEntity.hpp"

struct TEAlter : public TileEntity {
    static TEAlter* cloneUnsafe(TEAlter* oldE, TEAlter* newE);
    
    TEAlter() : TEAlter(Point2Neg1) {
        
    }
    
    TEAlter(Point2 pos) : TileEntity(pos) {
        
    }
    
    virtual ~TEAlter() {
        
    }
    
    virtual int getTileEntityTypeId() {
        return TILE_ENTITY_TYPE_ALTER;
    }
    
    virtual string debugString(){
        return pos.toString();
    }
    
    virtual void save(vector<unsigned char>* data);
    
    virtual void load(vector<unsigned char>* data, int* position);
};

#endif /* TEAlter_hpp */