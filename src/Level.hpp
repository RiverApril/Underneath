//
//  Level.h
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Level__
#define __Underneath__Level__

#include "Global.hpp"

#include "Tile.hpp"
#include "World.hpp"
#include "EntityAi.hpp"
#include "Geometry.hpp"
#include "MenuGame.hpp"
#include "TEStair.hpp"
#include "TEChest.hpp"

namespace Ui {
    class MenuGame;
}

class Entity;

struct TileData {
    uint8_t index = 0;
    bool explored = false;
    //Entity* entity = nullptr;
};

typedef int GenType;
const GenType genTypeStartArea = 0;
const GenType genTypeDungeon = 1;

class Level {
public:

    Level(World* w, string n, Point2 s, int d);
    ~Level();

    bool getExplored(Point2 p);
    void setExplored(Point2 p, bool a);

    string getName() {
        return name;
    }

    void renderMenuGame(double displayTime);

    //Entity* getDisplayEntity(Point2 p);

    //void setDisplayEntity(Point2 p, Entity* e);

    bool inRange(Point2 p);
    bool inRange(int x, int y);

    Point2 findRandomOfType(int index);
    Point2 findRandomWithFlag(TileFlag flag);
    Point2 findRandomWithoutFlag(TileFlag flag);

    Tile* tileAt(Point2 p);
    bool setTile(Point2 p, int tile);
    bool setTile(Point2 p, Tile* tile);
    int indexAt(Point2 p);

    Tile* tileAt(int x, int y);
    bool setTile(int x, int y, int tile);
    bool setTile(int x, int y, Tile* tile);
    int indexAt(int x, int y);
    
    Entity* firstEntityHere(Point2 p);

    Entity* getClosestVisableEntity(Point2 origin, double range, Entity* notMe = nullptr);
    vector<Entity*> getAllVisableEntities(Point2 origin, double range, Entity* notMe, bool sort);

    vector<Entity*> getAllVisableEntities(Point2 origin, double range, vector<Entity*> notMes, Point2 nearestTo, Point2 inDirection, bool sort);

    bool canSee(Point2 origin, Point2 test, double range);

    int entityCount();

    bool canPathTo(Point2 from, Point2 to, TileFlag requiredFlag, TileFlag bannedFlag = 0, bool careAboutEntities = false, bool mustBeExplored = false);

    vector<Point2> getPathTo(Point2 from, Point2 to, TileFlag requiredFlag, TileFlag bannedFlag = 0, bool careAboutEntities = false, bool mustBeExplored = false);

    Point2 generate(GenType genType, unsigned int seed, Point2 stairUpPos, string previousLevel);

    //The following are implemented in LevelGenerator.cpp
    Point2 generateStartArea(Point2 stairUpPos, string previousLevel);
    Point2 generateDungeon(Point2 stairUpPos, string previousLevel);
    

    void placeNewEntityAi(EntityAi* e, Point2 entrance);

    bool update(double deltaTime, double time, Point2 viewPos);

    void randomTileUpdate(Point2 p);

    //void setAndUnsetDisplayEntities();

    Entity* newEntity(Entity* newE, bool setUID = true);
    void removeEntity(Entity* e, bool deleteEntity);
    void actuallyRemoveEntityUnsafe(Entity* e, bool deleteEntity);

    void removeTileEntity(TileEntity* e);
    void actuallyRemoveTileEntityUnsafe(TileEntity* e);

    Point2 getSize() {
        return size;
    }

    int getDifficulty() {
        return difficulty;
    }

    void explode(Point2 pos, double radius, double attackPower, bool destroyTiles);

    int countTilesAround(Point2 p, Tile* t) {
        int c = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if ((!(i == 0 && j == 0)) && inRange(p + Point2(i, j))) {
                    if (tileAt(p + Point2(i, j))->getIndex() == t->getIndex()) {
                        c++;
                    }
                }
            }
        }
        return c;
    }

    void save(vector<unsigned char>*);

    void load(vector<unsigned char>* data, int*position);

    vector<Entity*> entityList;
    vector<Entity*> deleteEntityList;
    vector<Entity*> removeEntityList;

    World* currentWorld;

    vector<TileEntity*> tileEntityList;
    vector<TileEntity*> deleteTileEntityList;

    Point2 stairDownPos;

private:

    Point2 size;

    vector<vector<TileData> > tileGrid;

    string name;

    int difficulty = 1;
};



#endif /* defined(__Underneath__Level__) */
