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
#include "MenuGame.h"

namespace Ui{
    class MenuGame;
}

class Entity;

struct TileData{
    int8_t index = 0;
    bool explored = false;
    //Entity* entity = nullptr;
};

struct Stair{

    Stair(Point2 p, bool up, string levelName){
        this->p = p;
        this->up = up;
        this->levelName = levelName;
    }

    Point2 p = Point2(0, 0);
    bool up = false;
    string levelName = "";
};

class Level{

public:

    Level(World* w, string n, Point2 s);
    ~Level();

    bool getExplored(Point2 p);
    void setExplored(Point2 p, bool a);

    string getName(){
        return name;
    }

    void renderMenuGame(double displayTime);

    //Entity* getDisplayEntity(Point2 p);

    //void setDisplayEntity(Point2 p, Entity* e);

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

    bool canPathTo(Point2 from, Point2 to, TileFlag requiredFlag);

    vector<Point2> getPathTo(Point2 from, Point2 to, TileFlag requiredFlag);

    Point2 generate(unsigned int seed, Point2 stairUpPos, string previousLevel);

    template <typename T> void addEntitiesRandomly(Point2 start, T* e, int count);

    bool update(double time, Point2 viewPos);

    //void setAndUnsetDisplayEntities();

    Entity* newEntity(Entity* newE);
    void removeEntity(Entity* e, bool deleteEntity);

    Point2 getSize(){
        return *size;
    }

    void save(vector<unsigned char>*);

    void load(unsigned char* data, int*position);

    vector<Entity*> entityList;

    World* currentWorld;

    vector<Stair> stairList;

    Ui::MenuGame* menuGame;

private:

    Point2* size;

    vector<vector<TileData> > tileGrid;

    string name;
};



#endif /* defined(__Underneath__Level__) */
