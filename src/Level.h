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



class Level {

public:

    const static int WIDTH = 100;
    const static int HEIGHT = 100;

    Level();
    ~Level();

    bool isInView(int x, int y, Entity* e);

    bool getExplored(int x, int y);
    void setExplored(int x, int y, bool a);

    bool inRange(int x, int y);
    bool inRange(Geometry::Point2* a);
    Tile* tileAt(int x, int y);
    bool setTile(int x, int y, int tile);
    bool setTile(int x, int y, Tile* tile);
    int indexAt(int x, int y);

    bool canSeeSky(int x, int y);

    long entityCount();

    void generate(unsigned int seed);

    bool update(int time, Geometry::Point2* viewPos);

    void needsRedraw(Geometry::Point2* p);

    Entity* newEntity(Entity* newE);
    void deleteEntity(Entity* e);

    Entity* displayEntityGrid[WIDTH][HEIGHT];

private:

    int tileGrid[WIDTH][HEIGHT];
    bool explored[WIDTH][HEIGHT];

    std::vector<Entity*> entityList;
};



#endif /* defined(__Underneath__Level__) */
