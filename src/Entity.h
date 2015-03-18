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
const int ENTITY_TYPE_ITEMENTITY = 4;

class Entity{

public:

    static Entity* cloneUnsafe(Entity* oldE, Entity* newE);
    
    static Entity* clone(Entity* oldE);

    Entity();

    Entity(char icon, Point2 startPos, Ui::Color colorCode = Ui::COLOR_DEFAULT_ENTITY);

    virtual ~Entity();

    virtual bool tryToMoveAbsalute(Point2 p, Level* world);

    virtual bool tryToMoveRelative(Point2 p, Level* world);
    
    virtual bool update(double deltaTime, double time, Level* level);

    //void setAndUnsetDisplayEntity(Level* level);

    virtual char getIcon(Point2 p, double time, Level* level);

    bool isSolid(){
        return solid;
    }

    virtual string getName(){
        return "NO_NAME";
    }

    virtual Ui::Color getFgColorCode();
    virtual Ui::Color getBgColorCode();

    virtual void save(vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual int getRenderDepth(){
        return 0;
    }

    virtual void setTimes(double time){

    }

    virtual void load(unsigned char* data, int* position);

    static Entity* loadNew(unsigned char* data, int* position);

    int uniqueId = 0;

    Point2 pos;

    bool removed = false;

protected:
    char defaultIcon = '?';
    Point2 lastPos;
    Ui::Color fgColorCode;
    Ui::Color bgColorCode;
    bool updateIcon = true;
    bool solid = false;

    template<class Super, class Sub>
    static Sub* makeNewAndClone(Super* oldT);

};






#endif /* defined(__Underneath__Entity__) */
