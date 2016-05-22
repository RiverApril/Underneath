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
#include "Settings.hpp"
#include "Random.hpp"

Level::Level(World* w, string n, Point2 s, int d) {
    currentWorld = w;
    name = n;
    difficulty = d;
    size = s;
    tileGrid = vector<vector < TileData >> (size.x, vector<TileData>(size.y));

    pathMap = vector<vector<int>>(size.x, vector<int>(size.y));

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
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

bool Level::getExplored(int x, int y) {
    if (inRange(x, y)) {
        return tileGrid[x][y].explored;
    } else {
        return false;
    }
}

void Level::setExplored(int x, int y, bool a) {
    if (inRange(x, y)) {
        tileGrid[x][y].explored = a;
    }
}

bool Level::getExplored(Point2 p) {
    return getExplored(p.x, p.y);
}

void Level::setExplored(Point2 p, bool a) {
    setExplored(p.x, p.y, a);
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

bool Level::inRange(int x, int y) {
    return x >= 0 && y >= 0 && x < size.x && y < size.y;
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

    } while (tileAt(p)->doesNotHaveAllOfFlags(flags));
    return p;
}

Point2 Level::findRandomWithoutFlag(TileFlag flags) {
    Point2 p;
    do {
        p.x = rand() % size.x;
        p.y = rand() % size.y;

    } while ((tileAt(p)->hasAnyOfFlags(flags)));
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

bool Level::solidAt(Point2 p, TileFlag anyOf, bool checkEntities){
    if(Tiles::getTile(indexAt(p))->hasAnyOfFlags(anyOf)){
        return true;
    }else{
        for(Entity* e : entityList){
            if(e->pos == p){
                if(e->getSolidity() & anyOf){
                    return true;
                }
            }
        }
    }
    return false;
}

bool Level::setTile(int x, int y, int tile) {
    if (inRange(x, y)) {
        tileGrid[x][y].index = (int8_t) tile;
        return true;
    }
    return false;
}

bool Level::setTile(int x, int y, Tile* tile) {
    return setTile(x, y, tile->getIndex());
}

Tile* Level::tileAt(int x, int y) {
    return Tiles::getTile(indexAt(x, y));
}

int Level::indexAt(int x, int y) {
    if (inRange(x, y)) {
        return tileGrid[x][y].index;
    }
    return Tiles::tileEdge->getIndex();
}

Entity* Level::getEnitity(int UID){
    for(Entity* e : entityList){
        if(e->uniqueId == UID){
            return e;
        }
    }
    return nullptr;
}

Entity* Level::firstEntityHere(Point2 p){
    for(Entity* e : entityList){
        if(e->pos == p){
            return e;
        }
    }
    return nullptr;
}

vector<Entity*> Level::allEntitiesHere(Point2 p){
    vector<Entity*> v = {};
    for(Entity* e : entityList){
        if(e->pos == p){
            v.push_back(e);
        }
    }
    return v;
}

bool Level::canSee(Point2 origin, Point2 test, double range) {

    if(origin == test){
        return true;
    }

    if (distanceSquared(origin, test) > range * range) {
        return false;
    }

    bool hitAWall1 = false;
    bool hitAWall2 = false;

    Vector2 v;

    {
        Vector2 step = ((test - origin) / ((double) range));

        for (double i = 0; i <= range; i += .1) {

            v = (step * i);

            Point2 p = v.roundAwayFrom0();

            if (p != Point2Zero && (origin+p) != test && tileAt(origin + p)->hasAllOfFlags(tileFlagSolidBoth)) {
                hitAWall1 = true;
                break;
            }
        }
    }

    Point2 t = test;
    test = origin;
    origin = t;

    if(hitAWall1){
        Vector2 step = ((test - origin) / ((double) range));

        for (double i = 0; i <= range; i += .1) {

            v = (step * i);

            Point2 p = v.roundAwayFrom0();

            if (p != Point2Zero && (origin+p) != test && tileAt(origin + p)->hasAllOfFlags(tileFlagSolidBoth)) {
                hitAWall2 = true;
                break;
            }
        }
    }

    return (!hitAWall1 || !hitAWall2);

}

Entity* Level::getClosestVisableEntity(Point2 origin, double range, Entity* notMe) {
    Entity* closest = nullptr;
    double lastDistSquared = size.xPlusY();
    for (Entity* e : entityList) {
        if (e == notMe) {
            continue;
        }
        if (canSee(origin, e->pos, range)) {
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

vector<Entity*> Level::getAllVisableEntities(Point2 origin, double range, Entity* notMe, bool sort) {
    return getAllVisableEntities(origin, range,{notMe}, origin, Point2Zero, sort);
}

vector<Entity*> Level::getAllVisableEntities(Point2 origin, double range, vector<Entity*> notMes, Point2 nearestTo, Point2 inDirection, bool sort) {

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
        if (canSee(origin, e->pos, range)) {
            if (e != nullptr) {
                list.push_back(e);
            }
        }
    }

    int n = (int)list.size();

    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n - 1; y++) {
            if (distanceSquared(nearestTo, list[y]->pos) > distanceSquared(nearestTo, list[y + 1]->pos)) {
                Entity* temp = list[y + 1];
                list[y + 1] = list[y];
                list[y] = temp;
            }
        }
    }


    return list;

}

int Level::entityCount() {
    return (int)entityList.size();
}

bool Level::update(double deltaTime, double time, Point2 viewPos) {
    bool u = false;
    for (int i = 0; i < (int)entityList.size(); i++) {
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


    for(Point2 p = Point2Zero; p.x < size.x; p.x++){
        for(p.y = 0; p.y < size.y; p.y++){
            regularTileUpdate(p);
            if(rand()%50==0){
                randomTileUpdate(p);
            }
        }
    }


    return u;
}

void Level::randomTileUpdate(Point2 p){
    int index = indexAt(p);
    if(index == Tiles::tileCorpse->getIndex()){
		setTile(p, Tiles::tileBones);
    }else if(index == Tiles::tileBloodFloor->getIndex()){
        setTile(p, Tiles::tileFloor);
    }
}

void Level::regularTileUpdate(Point2 p){
    int index = indexAt(p);
    if(index == Tiles::tileFire->getIndex() && rand()%10==0){
        if(rand()%3 != 0){
            Utility::execute4Around(p.x, p.y, [this](int x, int y){
                if(this->tileAt(x, y)->hasAllOfFlags(tileFlagFlammable)){
                    setTile(x, y, Tiles::tileFire);
                }
            });
        }
        setTile(p, Tiles::tileAsh);
    }
}

void Level::renderMenuGame(double displayTime) {
    if (currentWorld->menuGame != nullptr) {
        currentWorld->menuGame->render(displayTime);
    }
}

Entity* Level::newEntity(Entity* newE, bool setUID) {
    if(setUID){
        //if(nextUniqueId == 0){
            //debugf("New Entity with UID = 0  Name: %s", newE->getName().c_str());
        //}
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
    for (int i = 0; i < (int)entityList.size(); i++) {
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
    for (int i = 0; i < (int)tileEntityList.size(); i++) {
        if (e == tileEntityList[i]) {
            tileEntityList.erase(tileEntityList.begin()+(long) i);
            debugf("Deleting Tile Entity: %d", e->getTileEntityTypeId());
            delete e;
            return;
        }
    }
    debugf("Failed to Remove Tile Entity: %d", e->getTileEntityTypeId());
}

vector<Point2> Level::getPathTo(Point2 to, Point2 from, TileFlag requiredFlag, TileFlag bannedFlag, bool careAboutEntities, bool mustBeExplored, TileFlag requiredEitherFlag) {

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            pathMap[i][j] = -1;
        }
    }

    if(!inRange(to) || !inRange(from)){
        return vector<Point2>();
    }

    queue<Point2> priorityQueue;
    priorityQueue.push(to);

    Point2 p;

    while (!priorityQueue.empty()) {
        Point2 c = priorityQueue.front();
        priorityQueue.pop();
        if (c == from) {
            vector<Point2> path;
            Point2 l = from;
            int v = pathMap[from.x][from.y];
            while (v > 0) {
                bool leave = false;
                bool rx = rand()%2==0;
                bool ry = rand()%2==0;
                for (int i = -1; i <= 1 && !leave; i++) {
                    for (int j = -1; j <= 1 && !leave; j++) {
                        p.x = l.x + (rx?i:-i);
                        p.y = l.y + (ry?j:-j);
                        if ((abs(i) + abs(j)) == 1) {
                            if (inRange(p)) {
                                if (pathMap[p.x][p.y] < v && pathMap[p.x][p.y] != -1) {
                                    v = pathMap[p.x][p.y];
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
            bool rx = rand()%2==0;
            bool ry = rand()%2==0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    Point2 p = Point2(c.x + (rx?i:-i), c.y + (ry?j:-j));
                    if ((abs(i) + abs(j)) == 1) {
                        if (inRange(p)) {
                            if (tileAt(p)->hasAllOfFlags(requiredFlag) && tileAt(p)->doesNotHaveAnyOfFlags(bannedFlag) && tileAt(p)->hasAnyOfFlags(requiredEitherFlag) && (!mustBeExplored || getExplored(p))) {
                                bool ent = false;
                                if((!mustBeExplored || getExplored(p)) && careAboutEntities && p != to && p != from){
                                    for(Entity* e : entityList){
                                        if(e){
                                            if(!(bool)(e->getSolidity() & requiredFlag) || (bool)(e->getSolidity() & bannedFlag)){
                                                if(e->pos == p){
                                                    ent = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                if(!ent){
                                    if (pathMap[p.x][p.y] == -1) {
                                        pathMap[p.x][p.y] = pathMap[c.x][c.y] + 1;
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

bool Level::canPathTo(Point2 from, Point2 to, TileFlag requiredFlag, TileFlag bannedFlag, bool careAboutEntities, bool mustBeExplored, TileFlag requiredAnyFlag) {
    return !(getPathTo(from, to, requiredFlag, bannedFlag, careAboutEntities, mustBeExplored, requiredAnyFlag).empty());

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

void Level::explode(Point2 pos, double radius, double attackPower, bool destroyTiles, DamageType damType, bool fire){
    double radiusSquared = radius*radius;
    for(int i=-radius; i<=radius; i++){
        for (int j=-radius; j<=radius; j++) {
            if( ((i*i) + (j*j)) <= radiusSquared){
                Point2 p = pos+Point2(i, j);
                if(tileAt(p)->doesNotHaveAnyOfFlags(tileFlagIndestructable) && rand()%20!=0){

                    if(destroyTiles){
                        if(tileAt(p)->hasAllOfFlags(tileFlagFlammable) || !fire){
                            setTile(p, Tiles::tileFire);
                        }else{
                            setTile(p, Tiles::tileRubble);
                        }
                    }else if(fire){
                        if(tileAt(p)->hasAllOfFlags(tileFlagFlammable | tileFlagReplaceable)){
                            setTile(p, Tiles::tileFire);
                        }
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
                    double disS = distanceSquared(a->pos, pos);
                    if(canSee(pos, a->pos, radius)){
                        a->hurt(this, damType, attackPower*(((radiusSquared)-disS))/(radiusSquared));
                        a->addEffect(Effect(effDamage, Random::randDouble(5, 10), 1, damType == damExplosion ? damFire : damType));
                    }
                }
            }
        }
    }
}


void Level::placeNewEntityAi(EntityAi* e, Point2 entrance) {
    Point2 p;
    int attmpts = 0;
    do {
        p = Point2(findRandomWithoutFlag(e->solidity));
        attmpts++;
        if(canPathTo(entrance, p, tileFlagPathable) && !canSee(entrance, p, e->viewDistance*2)){
            e->pos = p;
            newEntity(e);
            return;
        }
    } while (attmpts < 1000);
}

void Level::save(vector<unsigned char>* data) {

    size.save(data);
    Utility::saveInt(data, difficulty);
    Utility::saveInt(data, nextUniqueId);

    stairDownPos.save(data);
    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            Utility::saveUInt8Bit(data, tileGrid[i][j].index);
        }
    }

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            Utility::saveBool(data, tileGrid[i][j].explored);
        }
    }

    int count = 0;
    for (int i = 0; i < (int)entityList.size(); i++) {
	    if(entityList[i]->getEntityTypeId() != ENTITY_TYPE_PLAYER){
            count++;
        }
    }
    Utility::saveInt(data, count);
    debugf("Saving %d Entities...", count);
    for (Entity* e : entityList) {
	    if(e->getEntityTypeId() == ENTITY_TYPE_PLAYER){
	    	debug("Skipping Saving Player");
	    	continue;
	    }
        debug("Saving Entity: " + e->getName() + "(" + to_string(e->getEntityTypeId()) + ")" + ", Pos: " + e->pos.toString());
        e->save(data);
    }
    debugf("%d Entities Saved.", (int) entityList.size());
    debugf("Saving %d Tile Entities...", (int) tileEntityList.size());
    Utility::saveInt(data, (int) tileEntityList.size());
    for (TileEntity* t : tileEntityList) {
        t->save(data);
    }
    debugf("%d Tile Entities Saved.", (int) tileEntityList.size());
}

void Level::load(vector<unsigned char>* data, int* position) {

    //these happen beforehand:
    //load size
    //load difficulty
    nextUniqueId = Utility::loadInt(data, position);

    stairDownPos = Point2(data, position);

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            tileGrid[i][j].index = Utility::loadInt8Bit(data, position);
        }
    }

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            tileGrid[i][j].explored = Utility::loadBool(data, position);
        }
    }
    debug("Loaded " + to_string(size.x) + " x " + to_string(size.y) + " Tiles");

    int entityCount = Utility::loadInt(data, position);
    debugf("%d Entities to Load...", entityCount);
    for (int i = 0; i < entityCount; i++) {
        Entity* e = Entity::loadNew(data, position);
        e->setTimes(currentWorld->worldTime);
        entityList.push_back(e);
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
    entityList.clear();
    tileEntityList.clear();

    srand(seed);

    switch(genType){
        case genTypeStartArea:{
            return generateStartArea(stairUpPos, previousLevel);
        }
        case genTypeDungeon:{
            return generateDungeon(stairUpPos, previousLevel);
        }

    }
    return Point2Neg1;
}
