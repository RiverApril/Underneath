//
//  Entity.h
//  Underneath
//
//  Created by Braeden Atlee on 10/1/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Entity__
#define __Underneath__Entity__


#include "Ui.h"
#include "Geometry.h"

class Level;

const int ENTITY_TYPE_ENTITY = 0;
const int ENTITY_TYPE_ALIVE = 1;
const int ENTITY_TYPE_AIENTITY = 2;
const int ENTITY_TYPE_PLAYER = 3;

class Entity {

public:

    static Entity* clone(Entity* oldE, Entity* newE);

    Entity();

    Entity(std::string name, char icon, Point2 startPos, Ui::color colorCode = Ui::COLOR_DEFAULT_ENTITY);

    virtual ~Entity();

    virtual bool tryToMove(Point2 p, Level* world);
    virtual bool update(int tick, Level* level);

    virtual char getIcon(Point2 p, int tick, Level* level);

    Point2 getPos() {
        return *pos;
    }

    void setPos(Point2 p){
        pos->set(p);
    }

    int getColorCode();

    virtual void save(std::string* data);

    virtual int getEntityTypeId();

    virtual void load(char* data, int* position);

    static Entity* loadNew(char* data, int* position);

    std::string getName(){
        return name;
    }

    int uniqueId = 0;

protected:
    char defaultIcon = '?';
    std::string name;
    Point2* pos;
    Point2* lastPos;
    int colorCode;
    bool updateIcon = true;

};






#endif /* defined(__Underneath__Entity__) */
