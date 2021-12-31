//
//  Entity.hpp
//  Underneath
//
//  Created by Emily Atlee on 10/1/14.
//  Copyright (c) 2014 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__Entity__
#define __Underneath__Entity__


#include "Ui.hpp"
#include "Geometry.hpp"
#include "Icon.hpp"
#include "Tile.hpp"

class Level;

const int ENTITY_TYPE_ENTITY = 0;
const int ENTITY_TYPE_ALIVE = 1;
const int ENTITY_TYPE_AIENTITY = 2;
const int ENTITY_TYPE_PLAYER = 3;
const int ENTITY_TYPE_ITEMENTITY = 4;
const int ENTITY_TYPE_EXPLOSIVE = 5;
const int ENTITY_TYPE_SHOP = 6;
const int ENTITY_TYPE_MOVING = 7;
const int ENTITY_TYPE_MULTI = 8;
const int ENTITY_TYPE_MULTI_SUB = 9;

class Entity : Icon {
public:

    static Entity* clone(Entity* oldE);

    Entity();

    Entity(char icon, Point2 startPos, Ui::Color colorCode = Ui::COLOR_DEFAULT_ENTITY);

    virtual ~Entity();
    
    virtual bool canMoveAbsolute(Point2 p, Level* world, bool force = false);
    virtual bool tryToMoveAbsolute(Point2 p, Level* world, bool force = false);

    virtual bool tryToMoveRelative(Point2 p, Level* world);

    virtual bool update(double deltaTime, double time, Level* level);

    //void setAndUnsetDisplayEntity(Level* level);

    virtual string getName() {
        return "NO_NAME";
    }

    virtual char getChar(unsigned long tick, Point2 pos, Level* lvl){
        return defaultIcon;
    }

    virtual Ui::Color getFgColor(unsigned long tick, Point2 pos, Level* lvl){
        return fgColor;
    }

    virtual Ui::Color getBgColor(unsigned long tick, Point2 pos, Level* lvl);

    virtual void save(vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual int getRenderDepth() {
        return (getSolidity() & tileFlagSolidInAir)?1:0;
    }

    virtual void setTimes(double time) {

    }

    virtual TileFlag getSolidity(){
        return tileFlagNone;
    }

    virtual bool isHostile() {
        return false;
    }

    virtual void load(vector<unsigned char>* data, int* position);

    static Entity* loadNew(vector<unsigned char>* data, int* position);

    int uniqueId = 0;

    Point2 pos;

    bool removed = false;

    char defaultIcon = '?';
    Ui::Color fgColor;

    Point2 lastPos;

};






#endif /* defined(__Underneath__Entity__) */
