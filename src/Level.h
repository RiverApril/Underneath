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
    shared_ptr<Entity> entity = nullptr;
};

class Level : public enable_shared_from_this<Level>{

public:

    Level(string n, Point2 s = Point2(100, 100));
    ~Level();

    bool getExplored(Point2 p);
    void setExplored(Point2 p, bool a);

    string getName(){
        return name;
    }

    shared_ptr<Entity> getDisplayEntity(Point2 p);

    void setDisplayEntity(Point2 p, shared_ptr<Entity> e);

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

    bool update(int tick, Point2* viewPos);

    void needsRedraw(Point2* p);

    shared_ptr<Entity> newEntity(shared_ptr<Entity> newE);
    void deleteEntity(shared_ptr<Entity> e);

    Point2 getSize(){
        return *size;
    }

    void save(string*);

    void load(char* data, int*position);

    vector<shared_ptr<Entity>> entityList;

private:

    Point2* size;

    vector<vector<TileData>> tileGrid;

    string name;
};



#endif /* defined(__Underneath__Level__) */
