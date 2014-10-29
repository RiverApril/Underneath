//
//  Level.h
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Level__
#define __Underneath__Level__

class Entity;

#include "Global.h"

#include "Tile.h"
#include "Entity.h"
#include "Geometry.h"

struct TileData{
	int8_t index = 0;
    bool explored = false;
};

class Level {

public:

    const static int WIDTH = 100;
    const static int HEIGHT = 100;

    Level();
    ~Level();

    bool getExplored(Point2 p);
    void setExplored(Point2 p, bool a);

    bool inRange(Point2 p);
    Tile* tileAt(Point2 p);
    bool setTile(Point2 p, int tile);
    bool setTile(Point2 p, Tile* tile);
    int indexAt(Point2 p);

    bool canSee(Point2 origin, Point2 test, int range);

    long entityCount();

    void generate(unsigned int seed);

    bool update(int time, Point2* viewPos);

    void needsRedraw(Point2* p);

    Entity* newEntity(Entity* newE);
    void deleteEntity(Entity* e);

    Entity* displayEntityGrid[WIDTH][HEIGHT];

private:

    TileData tileGrid[WIDTH][HEIGHT];

    std::vector<Entity*> entityList;
};



#endif /* defined(__Underneath__Level__) */
