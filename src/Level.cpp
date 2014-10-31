//
//  Level.cpp
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Level.h"
#include "Math.h"
#include "Geometry.h"
#include "Global.h"
#include "LevelGenerator.h"
#include "Utility.h"

Level::Level() {
    size = new Point2(500, 500);
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

Point2 Level::findRandomEmpty(){
    Point2 p;
    do{
        p.x = rand()%size->x;
        p.y = rand()%size->y;

    }while(tileAt(p)->isSolid() || indexAt(p)==tileUnset->getIndex());
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
        u = entityList.at(i)->update(tick, this) || u;
    }
    return u;
}

Entity* Level::newEntity(Entity* newE) {
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

void Level::generate(unsigned int seed) {
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
    
    
}

