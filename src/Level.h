//
//  Level.h
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Level__
#define __Underneath__Level__

#include "Global.h"

#include "Tile.h"
#include "World.h"
#include "Entity.h"
#include "Geometry.h"

class Entity;

struct TileData{
    int8_t index = 0;
    bool explored = false;
    Entity* entity = nullptr;
};

class Level {

public:

    Level(std::string n, Point2 s = Point2(100, 100));
    ~Level();

    bool getExplored(Point2 p);
    void setExplored(Point2 p, bool a);

    std::string getName(){
        return name;
    }

    Entity* getDisplayEntity(Point2 p);

    void setDisplayEntity(Point2 p, Entity* e);

    bool inRange(Point2 p);

    Point2 findRandomOfType(int index);
    Point2 findRandomWithFlag(TileFlag flag);
    Point2 findRandomWithoutFlag(TileFlag flag);

    Tile* tileAt(Point2 p);
    bool setTile(Point2 p, int tile);
    bool setTile(Point2 p, Tile* tile);
    int indexAt(Point2 p);

    bool canSee(Point2 origin, Point2 test, int range);

    long entityCount();

    Point2 generate(unsigned int seed);

    bool update(int time, Point2* viewPos);

    void needsRedraw(Point2* p);

    Entity* newEntity(Entity* newE);
    void deleteEntity(Entity* e);

    Point2 getSize(){
        return *size;
    }

    void save(std::string*);

    void load(char* data, int*position);

    std::vector<Entity*> entityList;

private:

    Point2* size;

    TileData** tileGrid;

    std::string name;
};



#endif /* defined(__Underneath__Level__) */
