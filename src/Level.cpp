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

bool Level::getExplored(int x, int y) {
    return explored[x][y];
}

void Level::setExplored(int x, int y, bool a) {
    explored[x][y] = a;
}

bool Level::isInView(int x, int y, Entity* e) {
    return Math::distanceSquared(x, y, e->getPos()->x, e->getPos()->y) < (e->getViewDistance()*e->getViewDistance());
}

bool Level::inRange(int x, int y) {
    return x>=0 && y>=0 && x<WIDTH && y<HEIGHT;
}

bool Level::inRange(Geometry::Point2* a) {
    return inRange(a->x, a->y);
}

bool Level::setTile(int x, int y, int tile) {
    if(inRange(x, y)) {
        tileGrid[x][y] = tile;
        return true;
    }
    return false;
}

bool Level::setTile(int x, int y, Tile* tile) {
    return setTile(x, y, tile->getIndex());
}

Tile* Level::tileAt(int x, int y) {
    return getTile(indexAt(x, y));
}

int Level::indexAt(int x, int y) {
    if(inRange(x, y)) {
        return tileGrid[x][y];
    }
    return tileEdge->getIndex();
}

long Level::entityCount() {
    return entityList.size();
}

bool Level::update(int tick, Geometry::Point2* viewPos) {
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
            tileGrid[i][j] = tileAir->getIndex();
            explored[i][j] = true;
        }
    }

    std::vector<LevelGenerator::Room*>* rooms = LevelGenerator::createRooms(1000, WIDTH, HEIGHT);
    LevelGenerator::makeRoomsAndPaths(rooms, this);

    Utility::deleteContentsOfVector(rooms);
    delete rooms;


}

