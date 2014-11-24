//
//  ItemEntity.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/15/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "ItemEntity.h"

#include "Level.h"


ItemEntity::ItemEntity() : ItemEntity(nullptr, Point2Zero){

}

ItemEntity::ItemEntity(Item* item, Point2 p) : Entity(' ', p, Ui::C_WHITE){
    this->item = item;
    switch (item->getItemTypeId()) {
        case ITEM_TYPE_ITEM:
            defaultIcon = '*';
            break;
        case ITEM_TYPE_WEAPON:
            defaultIcon = '/';
            break;

        default:
            defaultIcon = '*';
            break;
    }
}

ItemEntity::~ItemEntity(){

}


bool ItemEntity::update(int tick, shared_ptr<Level> level) {



    return Entity::update(tick, level);
}


ItemEntity* ItemEntity::clone(ItemEntity* oldE, ItemEntity* newE){

    if(newE == nullptr){
        newE = new ItemEntity();
    }

    Entity::clone(oldE, newE);

    newE->item = oldE->item;

    return newE;

}

int ItemEntity::getEntityTypeId(){
    return ENTITY_TYPE_ITEMENTITY;
}

void ItemEntity::save(string* data){
    item->save(data);
    Entity::save(data);
}

void ItemEntity::load(char* data, int* position){
    item = Item::loadNew(data, position);
    Entity::load(data, position);
}


