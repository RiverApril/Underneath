//
//  EntityItem.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/15/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "EntityItem.hpp"
#include "Level.hpp"

EntityItem::EntityItem() : EntityItem(nullptr, Point2Zero) {

}

EntityItem::EntityItem(Item* item, Point2 p) : Entity(' ', p, Ui::C_WHITE) {
    this->item = item;
    defaultIcon = '*';
}

EntityItem::~EntityItem() {

}

char EntityItem::getIcon(Point2 p, double time, Level* level) {
    if (removed) {
        return defaultIcon;
    }
    if (!item) {
        return defaultIcon;
    }
    switch (item->getItemTypeId()) {
        case ITEM_TYPE_ITEM:
            return '*';
        case ITEM_TYPE_WEAPON:
            return '(';
        case ITEM_TYPE_RANGED:
            return ')';
        case ITEM_TYPE_COMBAT_SPELL:
            return '~';
        case ITEM_TYPE_UTILITY_SPELL:
            return '~';

        default:
            return '*';
    }
}

bool EntityItem::update(double deltaTime, double time, Level* level) {
    return Entity::update(deltaTime, time, level);
}

EntityItem* EntityItem::cloneUnsafe(EntityItem* oldE, EntityItem* newE) {

    Entity::cloneUnsafe(oldE, newE);

    newE->item = Item::clone(oldE->item);

    return newE;

}

int EntityItem::getEntityTypeId() {
    return ENTITY_TYPE_ITEMENTITY;
}

void EntityItem::save(vector<unsigned char>* data) {
    Entity::save(data);
    item->save(data);
}

void EntityItem::load(unsigned char* data, int* position) {
    Entity::load(data, position);
    item = Item::loadNew(data, position);
}