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

}

Level::~Level() {
    /*std::vector<Entity*>::iterator it;
    for(it = entityList.begin(); it!=entityList.end();) {
        delete * it;
        it = entityList.erase(it);
    }*/
    Utility::deleteContentsOfVector(&entityList);
}

bool Level::getExplored(Point2 p) {
    return tileGrid[p.x][p.y].explored;
}

void Level::setExplored(Point2 p, bool a) {
    tileGrid[p.x][p.y].explored = a;
}

bool Level::inRange(Point2 p) {
    return p.x>=0 && p.y>=0 && p.x<WIDTH && p.y<HEIGHT;
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
        u = entityList[i]->update(tick, this) || u;
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

    for (int i=0; i<WIDTH; i++) {
        for (int j=0; j<HEIGHT; j++) {
            displayEntityGrid[i][j] = nullptr;
            tileGrid[i][j].index = tileUnset->getIndex();
            tileGrid[i][j].explored = false;
        }
    }

    std::vector<LevelGenerator::Room*>* rooms = LevelGenerator::createRooms(1000, Point2(WIDTH, HEIGHT));
    LevelGenerator::makeRoomsAndPaths(rooms, this);

    Utility::deleteContentsOfVector(rooms);
    delete rooms;


}

