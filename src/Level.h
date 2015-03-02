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
#include "AiEntity.h"
#include "Geometry.h"
#include "MenuGame.h"
#include "TEStair.h"
#include "TEChest.h"

namespace Ui{
    class MenuGame;
}

class Entity;

struct TileData{
    int8_t index = 0;
    bool explored = false;
    //Entity* entity = nullptr;
};

class Level{

public:

    Level(World* w, string n, Point2 s, int d);
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

    Entity* getClosestVisableEntity(Point2 origin, double range, Entity* notMe = nullptr);
    vector<Entity*> getAllVisableEntitiesSortedByNearest(Point2 origin, double range, Entity* notMe);

    bool canSee(Point2 origin, Point2 test, double range, bool withWalls);

    size_t entityCount();

    bool canPathTo(Point2 from, Point2 to, TileFlag requiredFlag);

    vector<Point2> getPathTo(Point2 from, Point2 to, TileFlag requiredFlag);

    void genDebug(string s);

    Point2 generate(unsigned int seed, Point2 stairUpPos, string previousLevel);

    void placeNewAiEntity(AiEntity* e, Point2 entrance);

    bool update(double time, Point2 viewPos);

    //void setAndUnsetDisplayEntities();

    Entity* newEntity(Entity* newE);
    void removeEntity(Entity* e, bool deleteEntity);
    void actuallyRemoveEntityUnsafe(Entity* e, bool deleteEntity);

    Point2 getSize(){
        return *size;
    }

    int getDifficulty(){
        return difficulty;
    }

    void save(vector<unsigned char>*);

    void load(unsigned char* data, int*position);

    vector<Entity*> entityList;
    vector<Entity*> deleteEntityList;
    vector<Entity*> removeEntityList;

    World* currentWorld;

    vector<TileEntity*> tileEntityList;

    Ui::MenuGame* menuGame;

private:

    Point2* size;//TODO figure out if this needs to be a pointer

    vector<vector<TileData> > tileGrid;

    string name;

    int genDebugPos;

    int difficulty = 1;
};



#endif /* defined(__Underneath__Level__) */
