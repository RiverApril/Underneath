//
//  Level.cpp
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Level.hpp"
#include "EntityAi.hpp"
#include "Math.hpp"
#include "Geometry.hpp"
#include "Global.hpp"
#include "LevelGenerator.hpp"
#include "ItemGenerator.hpp"
#include "EnemyGenerator.hpp"
#include "Utility.hpp"

Level::Level(World* w, string n, Point2 s, int d) {
    currentWorld = w;
    name = n;
    difficulty = d;
    size = s;
    tileGrid = vector<vector < TileData >> (size.x, vector<TileData>(size.y));

    for (size_t i = 0; i < size.x; i++) {
        for (size_t j = 0; j < size.y; j++) {
            tileGrid[i][j].index = (int8_t) Tiles::tileUnset->getIndex();
            tileGrid[i][j].explored = false;
            //tileGrid[i][j].entity = nullptr;
        }
    }

    tileEntityList = vector<TileEntity*>();

}

Level::~Level() {
    for (Entity* e : entityList) {
        if(e){
        	delete e;
        }
    }
    entityList.clear();

    for (Entity* e : deleteEntityList) {
        if(e){
        	delete e;
        }
    }
    deleteEntityList.clear();

    for (Entity* e : removeEntityList) {
        if(e){
        	delete e;
        }
    }
    removeEntityList.clear();

    for (TileEntity* e : tileEntityList) {
        if(e){
        	delete e;
        }
    }
    tileEntityList.clear();

    for (TileEntity* e : deleteTileEntityList) {
        if(e){
        	delete e;
        }
    }
    deleteTileEntityList.clear();
}

bool Level::getExplored(Point2 p) {
    if (inRange(p)) {
        return tileGrid[p.x][p.y].explored;
    } else {
        return false;
    }
}

void Level::setExplored(Point2 p, bool a) {
    if (inRange(p)) {
        tileGrid[p.x][p.y].explored = a;
    }
}

/*Entity* Level::getDisplayEntity(Point2 p){
    if(inRange(p)) {
        return tileGrid[p.x][p.y].entity;
    }else{
        return nullptr;
    }
}

void Level::setDisplayEntity(Point2 p, Entity* e){
    if(inRange(p)) {
        tileGrid[p.x][p.y].entity = e;
    }
}*/

bool Level::inRange(Point2 p) {
    return p.x >= 0 && p.y >= 0 && p.x < size.x && p.y < size.y;
}

Point2 Level::findRandomOfType(int index) {
    Point2 p;
    do {
        p.x = rand() % size.x;
        p.y = rand() % size.y;

    } while (indexAt(p) != index);
    return p;
}

Point2 Level::findRandomWithFlag(TileFlag flags) {
    Point2 p;
    do {
        p.x = rand() % size.x;
        p.y = rand() % size.y;

    } while (!(tileAt(p)->hasFlag(flags)));
    return p;
}

Point2 Level::findRandomWithoutFlag(TileFlag flags) {
    Point2 p;
    do {
        p.x = rand() % size.x;
        p.y = rand() % size.y;

    } while ((tileAt(p)->hasFlag(flags)));
    return p;
}

bool Level::setTile(Point2 p, int tile) {
    if (inRange(p)) {
        tileGrid[p.x][p.y].index = (int8_t) tile;
        return true;
    }
    return false;
}

bool Level::setTile(Point2 p, Tile* tile) {
    return setTile(p, tile->getIndex());
}

Tile* Level::tileAt(Point2 p) {
    return Tiles::getTile(indexAt(p));
}

int Level::indexAt(Point2 p) {
    if (inRange(p)) {
        return tileGrid[p.x][p.y].index;
    }
    return Tiles::tileEdge->getIndex();
}

bool Level::canSee(Point2 origin, Point2 test, double range, bool withWalls) {

    if (Math::distanceSquared(origin.x, origin.y, test.x, test.y) > range * range) {
        return false;
    }

    Vector2 step = ((test - origin) / ((double) range));

    Vector2 v;
    Point2 p;

    for (double i = 0; i < range; i += .2) {

        v = (step * i);

        if (withWalls) {
            p = v.roundToward0();
        } else {
            p = v.roundAwayFrom0();
        }

        if (tileAt(origin + (p))->isTall()) {
            return false;
        }
    }

    return true;
}

Entity* Level::getClosestVisableEntity(Point2 origin, double range, Entity* notMe) {
    Entity* closest = nullptr;
    double lastDistSquared = size.xPlusY();
    for (Entity* e : entityList) {
        if (e == notMe) {
            continue;
        }
        if (canSee(origin, e->pos, range, false)) {
            if (e != nullptr) {
                double temp = distanceSquared(origin, e->pos);
                if (temp < lastDistSquared) {
                    closest = e;
                    lastDistSquared = temp;
                }
            }
        }
    }
    return closest;
}

vector<Entity*> Level::getAllVisableEntitiesSortedByNearest(Point2 origin, double range, Entity* notMe) {
    return getAllVisableEntitiesSortedByNearest(origin, range,{notMe}, origin, Point2Zero);
}

vector<Entity*> Level::getAllVisableEntitiesSortedByNearest(Point2 origin, double range, vector<Entity*> notMes, Point2 nearestTo, Point2 inDirection) {

    vector<Entity*> list;

    for (Entity* e : entityList) {
        bool skip = false;
        if (inDirection.x < 0) {
            if (e->pos.x > nearestTo.x) {
                continue;
            }
        }
        if (inDirection.x > 0) {
            if (e->pos.x < nearestTo.x) {
                continue;
            }
        }
        if (inDirection.y < 0) {
            if (e->pos.y > nearestTo.y) {
                continue;
            }
        }
        if (inDirection.y > 0) {
            if (e->pos.y < nearestTo.y) {
                continue;
            }
        }
        for (Entity* notMe : notMes) {
            if (notMe) {
                if (e->uniqueId == notMe->uniqueId) {
                    skip = true;
                    break;
                }
            }
        }
        if (skip) {
            continue;
        }
        if (canSee(origin, e->pos, range, true)) {
            if (e != nullptr) {
                list.push_back(e);
            }
        }
    }

    size_t n = list.size();

    for (size_t x = 0; x < n; x++) {
        for (size_t y = 0; y < n - 1; y++) {
            if (distanceSquared(nearestTo, list[y]->pos) > distanceSquared(nearestTo, list[y + 1]->pos)) {
                Entity* temp = list[y + 1];
                list[y + 1] = list[y];
                list[y] = temp;
            }
        }
    }


    return list;

}

size_t Level::entityCount() {
    return entityList.size();
}

bool Level::update(double deltaTime, double time, Point2 viewPos) {
    bool u = false;
    for (int i = 0; i < entityList.size(); i++) {
        Entity* e = entityList[i];
        if (!e->removed) {
            if (e->update(deltaTime, time, this)) {
                u = true;
            }
        }
    }
    while (removeEntityList.size() > 0) {
        actuallyRemoveEntityUnsafe(removeEntityList[0], false);
        removeEntityList.erase(removeEntityList.begin());
    }
    while (deleteEntityList.size() > 0) {
        actuallyRemoveEntityUnsafe(deleteEntityList[0], true);
        deleteEntityList.erase(deleteEntityList.begin());
    }
    while (deleteTileEntityList.size() > 0) {
        actuallyRemoveTileEntityUnsafe(deleteTileEntityList[0]);
        deleteTileEntityList.erase(deleteTileEntityList.begin());
    }
    return u;
}

void Level::renderMenuGame(double displayTime) {
    if (menuGame != nullptr) {
        menuGame->render(displayTime);
    }
}

/*void Level::setAndUnsetDisplayEntities(){
    for (size_t i=0; i<entityList.size(); i++) {
        entityList.at(i)->setAndUnsetDisplayEntity(shared_from_this());
    }
}*/

Entity* Level::newEntity(Entity* newE, bool setUID) {
    if(setUID){
        if(nextUniqueId == 0){
            debugf("New Entity with UID = 0  Name: %s", newE->getName().c_str());
        }
        newE->uniqueId = nextUniqueId;
    	nextUniqueId++;
    }
    newE->setTimes(currentWorld->worldTime);
    entityList.push_back(newE);
    return newE;
}

void Level::removeEntity(Entity* e, bool deleteEntity) {
    e->removed = true;
    if (deleteEntity) {
        deleteEntityList.push_back(e);
    } else {
        removeEntityList.push_back(e);
    }
}

void Level::removeTileEntity(TileEntity* e) {
    deleteTileEntityList.push_back(e);
}

void Level::actuallyRemoveEntityUnsafe(Entity* e, bool deleteEntity) {
    for (int i = 0; i < entityList.size(); i++) {
        if (e->uniqueId == entityList[i]->uniqueId) {
            entityList.erase(entityList.begin() + i);
            debug("Removed Entity: " + e->getName());
            if (deleteEntity) {
                debug("Deleting Entity: " + e->getName());
                delete e;
            }
            return;
        }
    }
    debug("Failed to Remove Entity: " + e->getName());
}

void Level::actuallyRemoveTileEntityUnsafe(TileEntity* e) {
    for (size_t i = 0; i < tileEntityList.size(); i++) {
        if (e == tileEntityList[i]) {
            tileEntityList.erase(tileEntityList.begin()+(long) i);
            debugf("Deleting Tile Entity: %d", e->getTileEntityTypeId());
            delete e;
            return;
        }
    }
    debugf("Failed to Remove Tile Entity: %d", e->getTileEntityTypeId());
}

vector<Point2> Level::getPathTo(Point2 from, Point2 to, TileFlag requiredFlag, TileFlag bannedFlag, bool careAboutEntities) {
    vector<vector<int>> map = vector<vector<int>>(size.x, vector<int>(size.y));

    for (size_t i = 0; i < size.x; i++) {
        for (size_t j = 0; j < size.y; j++) {
            map[i][j] = -1;
        }
    }

    queue<Point2> priorityQueue;
    priorityQueue.push(from);

    while (!priorityQueue.empty()) {
        Point2 c = priorityQueue.front();
        priorityQueue.pop();
        if (c == to) {
            vector<Point2> path;
            Point2 l = to;
            int v = map[to.x][to.y];
            while (v > 0) {
                bool leave = false;
                for (int i = -1; i <= 1 && !leave; i++) {
                    for (int j = -1; j <= 1 && !leave; j++) {
                        Point2 p = Point2(i + l.x, j + l.y);
                        if ((abs(i) + abs(j)) == 1) {
                            if (inRange(p)) {
                                if (map[p.x][p.y] < v && map[p.x][p.y] != -1) {
                                    v = map[p.x][p.y];
                                    l = p;
                                    leave = true;
                                    path.push_back(l);
                                }
                            }
                        }
                    }
                }
                if (!leave) {
                    break;
                }
            }
            return path;
        } else {
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    Point2 p = Point2(i + c.x, j + c.y);
                    if ((abs(i) + abs(j)) == 1) {
                        if (inRange(p)) {
                            if (tileAt(p)->hasFlag(requiredFlag) && tileAt(p)->doesNotHaveFlag(bannedFlag)) {
                                bool ent = false;
                                if(careAboutEntities && p != from && p != to){
                                    for(Entity* e : entityList){
                                        if(e){
                                            if(e->isSolid()){
                                                if(e->pos == p){
                                                    ent = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                if(!ent){
                                    if (map[p.x][p.y] == -1) {
                                        map[p.x][p.y] = map[c.x][c.y] + 1;
                                        priorityQueue.push(p);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    }
    return vector<Point2>();
}

bool Level::canPathTo(Point2 from, Point2 to, TileFlag requiredFlag, TileFlag bannedFlag, bool careAboutEntities) {
    return !getPathTo(from, to, requiredFlag, bannedFlag, careAboutEntities).empty();

    /*vector<vector<char>> map = vector<vector<char>>(size.x, vector<char>(size.y));
    for(int i=0;i<size.x;i++){
        for(int j=0;j<size.y;j++){
            map[i][j] = 'U';
        }
    }
    queue<Point2> priorityQueue;
    priorityQueue.push(from);

    while(!priorityQueue.empty()){
        Point2 c = priorityQueue.front();
        priorityQueue.pop();
        if(c == to){
            return true;
        }else{
            for(int i=-1;i<=1;i++){
                for(int j=-1;j<=1;j++){
                    Point2 p = Point2(i+c.x, j+c.y);
                    if((abs(i)+abs(j)) == 1){
                        if(inRange(p)){
                                if(tileAt(p)->hasFlag(tileFlagPathable)){
                                if(map[p.x][p.y] == 'U'){
                                    map[p.x][p.y] = 'V';
                                    priorityQueue.push(p);
                                }
                                map[c.x][c.y] = 'E';
                            }
                        }
                    }
                }
            }
        }
    }
    return false;*/
}

void Level::genDebug(string s) {
    Ui::setColor(Ui::C_WHITE);
    move(genDebugPos, 0);
    clrtoeol();
    mvprintw(genDebugPos, 0, s.c_str());
    genDebugPos++;

    if (genDebugPos >= Ui::terminalSize.y) {
        genDebugPos = 1;
        move(genDebugPos, 0);
        clrtobot();
    }
    refresh();
}

void Level::explode(Point2 pos, double radius, double attackPower, bool destroyTiles){
    if(destroyTiles){
        for(int i=-radius; i<=radius; i++){
            for (int j=-radius; j<=radius; j++) {
                if( ((i*i) + (j*j)) <= radius*radius){
                    Point2 p = pos+Point2(i, j);
                    if(!tileAt(p)->hasFlag(tileFlagIndestructable)){
                        setTile(p, rand()%4==0?Tiles::tileFloor:Tiles::tileRubble);
                    }
                }
            }
        }
    }
    for (Entity* e : entityList) {
        if(e){
            if(!e->removed){
                EntityAlive* a = dynamic_cast<EntityAlive*>(e);
                if(a){
                    if(distanceSquared(a->pos, pos) <= radius*radius){
                        a->hurt(damExplosion, attackPower);
                    }
                }
            }
        }
    }
}

//Level::generate() is implemented in LevelGenerator.cpp

void Level::placeNewEntityAi(EntityAi* e, Point2 entrance) {
    Point2 p;
    do {
        p = Point2(findRandomWithoutFlag(tileFlagSolid));
    } while (!canPathTo(entrance, p, tileFlagPathable | tileFlagSecretPathable) || canSee(entrance, p, e->viewDistance, false));

    e->pos = p;
    newEntity(e);
}

void Level::save(vector<unsigned char>* data) {

    Point2::save(size, data);
    Utility::saveInt(data, difficulty);
    Utility::saveInt(data, nextUniqueId);

    Point2::save(stairDownPos, data);
    for (size_t i = 0; i < size.x; i++) {
        for (size_t j = 0; j < size.y; j++) {
            Utility::saveInt8Bit(data, tileGrid[i][j].index);
            Utility::saveBool(data, tileGrid[i][j].explored);
        }
    }
    Utility::saveInt(data, (int) entityList.size());
    debugf("Saving %d Entities...", (int) entityList.size());
    for (size_t i = 0; i < entityList.size(); i++) {
        debug("Saving Entity: " + entityList[i]->getName() + "(" + to_string(entityList[i]->getEntityTypeId()) + ")" + ", Pos: " + entityList[i]->pos.toString());
        entityList[i]->save(data);
    }
    debugf("%d Entities Saved.", (int) entityList.size());
    debugf("Saving %d Tile Entities...", (int) tileEntityList.size());
    Utility::saveInt(data, (int) tileEntityList.size());
    for (size_t i = 0; i < tileEntityList.size(); i++) {
        tileEntityList[i]->save(data);
    }
    debugf("%d Tile Entities Saved.", (int) tileEntityList.size());
}

void Level::load(unsigned char* data, int* position) {

    //these happen beforehand:
    //load size
    //load difficulty
    nextUniqueId = Utility::loadInt(data, position);

    stairDownPos = Point2::load(data, position);

    for (size_t i = 0; i < size.x; i++) {
        for (size_t j = 0; j < size.y; j++) {
            tileGrid[i][j].index = Utility::loadInt8Bit(data, position);
            tileGrid[i][j].explored = Utility::loadBool(data, position);
        }
    }
    debug("Loaded " + to_string(size.x) + " x " + to_string(size.y) + " Tiles");

    int entityCount = Utility::loadInt(data, position);
    debugf("%d Entities to Load...", entityCount);
    for (int i = 0; i < entityCount; i++) {
        entityList.push_back(Entity::loadNew(data, position));
    }
    debugf("Loaded %d Entities", (int) entityList.size());

    int tileEntityCount = Utility::loadInt(data, position);
    debugf("%d Tile Entities to Load...", tileEntityCount);
    for (int i = 0; i < tileEntityCount; i++) {
        tileEntityList.push_back(TileEntity::loadNew(data, position));
    }
    debugf("Loaded %d Tile Entities", (int) tileEntityList.size());
}


Point2 Level::generate(GenType genType, unsigned int seed, Point2 stairUpPos, string previousLevel) {
    switch(genType){
        case genTypeStartArea:{
            return generateStartArea(seed, stairUpPos, previousLevel);
        }
        case genTypeDungeon:{
            return generateDungeon(seed, stairUpPos, previousLevel);
        }

    }
    return Point2Neg1;
}

