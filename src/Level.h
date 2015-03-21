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

    vector<Entity*> getAllVisableEntitiesSortedByNearest(Point2 origin, double range, vector<Entity*> notMes, Point2 nearestTo, Point2 inDirection);

    bool canSee(Point2 origin, Point2 test, double range, bool withWalls);

    size_t entityCount();

    bool canPathTo(Point2 from, Point2 to, TileFlag requiredFlag);

    vector<Point2> getPathTo(Point2 from, Point2 to, TileFlag requiredFlag);

    void genDebug(string s);

    Point2 generate(unsigned int seed, Point2 stairUpPos, string previousLevel);

    void placeNewAiEntity(AiEntity* e, Point2 entrance);

    bool update(double deltaTime, double time, Point2 viewPos);

    //void setAndUnsetDisplayEntities();

    Entity* newEntity(Entity* newE);
    void removeEntity(Entity* e, bool deleteEntity);
    void actuallyRemoveEntityUnsafe(Entity* e, bool deleteEntity);

    void removeTileEntity(TileEntity* e);
    void actuallyRemoveTileEntityUnsafe(TileEntity* e);

    Point2 getSize(){
        return size;
    }

    int getDifficulty(){
        return difficulty;
    }

    int countTilesAround(Point2 p, Tile* t){
        int c = 0;
        for(int i=-1;i<=1;i++){
            for(int j=-1;j<=1;j++){
                if( (!(i==0 && j==0)) && inRange(p + Point2(i, j))){
                    if(tileAt(p + Point2(i, j))->getIndex() == t->getIndex()){
                        c++;
                    }
                }
            }
        }
        return c;
    }

    void save(vector<unsigned char>*);

    void load(unsigned char* data, int*position);

    vector<Entity*> entityList;
    vector<Entity*> deleteEntityList;
    vector<Entity*> removeEntityList;

    World* currentWorld;

    vector<TileEntity*> tileEntityList;
    vector<TileEntity*> deleteTileEntityList;

    Ui::MenuGame* menuGame;

private:

    Point2 size;

    vector<vector<TileData> > tileGrid;

    string name;

    int genDebugPos;

    int difficulty = 1;
};



#endif /* defined(__Underneath__Level__) */
