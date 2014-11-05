//
//  Level.cpp
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Level.h"
#include "AiEntity.h"
#include "Math.h"
#include "Geometry.h"
#include "Global.h"
#include "LevelGenerator.h"
#include "Utility.h"

Level::Level(std::string n, Point2 s) {
    name = n;
    size = new Point2(s);
    tileGrid = new TileData*[size->x];
    
    for(int i=0;i<size->x;i++){
        tileGrid[i] = new TileData[size->y];
        for(int j=0;j<size->y;j++){
            tileGrid[i][j].index = tileUnset->getIndex();
            tileGrid[i][j].explored = false;
            tileGrid[i][j].entity = nullptr;
        }
    }
}

Level::~Level() {

    for(int i=0;i<size->x;i++){
        delete [] tileGrid[i];
    }
    delete [] tileGrid;
	
    entityList.clear();
}

bool Level::getExplored(Point2 p) {
    if(inRange(p)) {
    	return tileGrid[p.x][p.y].explored;
    }else{
        return false;
    }
}

void Level::setExplored(Point2 p, bool a) {
    if(inRange(p)) {
    	tileGrid[p.x][p.y].explored = a;
    }
}

Entity* Level::getDisplayEntity(Point2 p){
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
}

bool Level::inRange(Point2 p) {
    return p.x>=0 && p.y>=0 && p.x<size->x && p.y<size->y;
}

Point2 Level::findRandomOfType(int index){
    Point2 p;
    do{
        p.x = rand()%size->x;
        p.y = rand()%size->y;

    }while(indexAt(p)!=index);
    return p;
}

Point2 Level::findRandomWithFlag(TileFlag flags){
    Point2 p;
    do{
        p.x = rand()%size->x;
        p.y = rand()%size->y;

    }while(!(tileAt(p)->hasFlag(flags)));
    return p;
}

Point2 Level::findRandomWithoutFlag(TileFlag flags){
    Point2 p;
    do{
        p.x = rand()%size->x;
        p.y = rand()%size->y;

    }while((tileAt(p)->hasFlag(flags)));
    return p;
}

bool Level::setTile(Point2 p, int tile) {
    if(inRange(p)) {
        tileGrid[p.x][p.y].index = tile;
        return true;
    }
    return false;
}

bool Level::setTile(Point2 p, Tile* tile) {
    return setTile(p, tile->getIndex());
}

Tile* Level::tileAt(Point2 p) {
    return getTile(indexAt(p));
}

int Level::indexAt(Point2 p) {
    if(inRange(p)) {
        return tileGrid[p.x][p.y].index;
    }
    return tileEdge->getIndex();
}

bool Level::canSee(Point2 origin, Point2 test, int range){

    if(distanceSquared(origin.x, origin.y, test.x, test.y) > range*range){
        return false;
    }

    Vector2 step = ((test-origin)/((double)range));

    for(double i=0;i<range;i+=.2){
        if(tileAt(origin+((step*i).truncate()))->isTall()){
            return false;
        }
    }



    return true;
}

long Level::entityCount() {
    return entityList.size();
}

bool Level::update(int tick, Point2* viewPos) {
    bool u = false;
    for (size_t i=0; i<entityList.size(); i++) {
        if(entityList.at(i)->update(tick, this)){
            u = true;
        }
    }
    return u;
}

Entity* Level::newEntity(Entity* newE) {
    newE->uniqueId = nextUniqueId;
    nextUniqueId++;
    entityList.push_back(newE);
    return newE;
}

void Level::deleteEntity(Entity* e) {
    std::vector<Entity*>::iterator it;
    for(it = entityList.begin(); it!=entityList.end();) {
        if((*it) == e) {
            delete * it;
            it = entityList.erase(it);
        } else {
            it++;
        }
    }
}

Point2 Level::generate(unsigned int seed) {
    srand(seed);

    for (int i=0; i<size->x; i++) {
        for (int j=0; j<size->y; j++) {
            tileGrid[i][j].index = tileUnset->getIndex();
            tileGrid[i][j].explored = false;
            tileGrid[i][j].entity = nullptr;
            if(i==0 || j==0 || i==(size->x-1) || j==(size->y-1)){
                tileGrid[i][j].index = tileWall->getIndex();
            }
        }
    }

    std::vector<LevelGenerator::Room*>* rooms = LevelGenerator::createRooms(1000, *size);
    LevelGenerator::makeRoomsAndPaths(rooms, this);

    delete rooms;

    Point2 p;

    p = findRandomOfType(tileFloor->getIndex());
    setTile(p, tileStairUp);

    for(int i=0;i<10;i++){
        Point2 f = findRandomOfType(tileFloor->getIndex());
        if(distanceSquared(f, p) > 20*20){
        	setTile(f, tileStairDown->getIndex());
        }else{
            i--;
            debug("Too close");
        }
    }


    AiEntity* rat = new AiEntity("Rat", aiMoveRandom, 'r', Point2Zero, Ui::C_DARK_YELLOW);
    int ratCount = (rand()%50)+10;
    for(int i=0;i<ratCount;i++){
        AiEntity* r = AiEntity::clone(rat, nullptr);
        Point2 pp = Point2(findRandomWithoutFlag(tileFlagSolid));
        r->setPos(pp);
        newEntity(r);
    }

    return p;
    
    
}

void Level::save(std::string* data){
    size->save(data);
    for(int i=0;i<size->x;i++){
        for(int j=0;j<size->y;j++){
            Utility::saveInt8Bit(data, tileGrid[i][j].index);
            Utility::saveBool(data, tileGrid[i][j].explored);
        }
    }
    Utility::saveInt(data, (int)entityList.size());
    for(int i=0;i<entityList.size();i++){
        entityList[i]->save(data);
    }
}

void Level::load(char* data, int* position){
    for(int i=0;i<size->x;i++){
        for(int j=0;j<size->y;j++){
            tileGrid[i][j].index = Utility::loadInt8Bit(data, position);
            tileGrid[i][j].explored = Utility::loadBool(data, position);
        }
    }
    int entityCount = Utility::loadInt(data, position);
    for(int i=0;i<entityCount;i++){
        entityList.push_back(Entity::loadNew(data, position));
    }
}



