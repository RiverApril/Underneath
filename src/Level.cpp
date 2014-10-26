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

Level::Level() {

}

Level::~Level() {
    std::vector<Entity*>::iterator it;
    for(it = entityList.begin(); it!=entityList.end();) {
        delete * it;
        it = entityList.erase(it);
    }
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

    std::vector<LevelGenerator::Room> rooms = LevelGenerator::createRooms(1000, WIDTH, HEIGHT);
    LevelGenerator::makeRoomsAndPaths(rooms, this);


}

namespace LevelGenerator{

    int OPTION_ROOM_RADIUS_MAX = 10;
    int OPTION_ROOM_RADIUS_MIN = 2;

    bool roomsOverlap(Room a, Room b, int border){
        return !(((a.center.x-a.radius.x-border) > (b.center.x+b.radius.x+border) ||
        (b.center.x-b.radius.x-border) > (a.center.x+a.radius.x+border)) ||
        ((a.center.y-a.radius.y-border) > (b.center.y+b.radius.y+border) ||
        (b.center.y-b.radius.y-border) > (a.center.y+a.radius.y+border)));
    }

    Room createRoom(int levelWidth, int levelHeight, std::vector<Room> presentRooms){
        bool fit = false;
        Room r;
        int att = 0;
        while(!fit && att<100){
            att++;
            r.radius.x = (rand()%(OPTION_ROOM_RADIUS_MAX-OPTION_ROOM_RADIUS_MIN))+OPTION_ROOM_RADIUS_MIN;
            r.radius.y = (rand()%(OPTION_ROOM_RADIUS_MAX-OPTION_ROOM_RADIUS_MIN))+OPTION_ROOM_RADIUS_MIN;
            r.center.x = rand()%(levelWidth-(r.radius.x*2))+r.radius.x;
            r.center.y = rand()%(levelHeight-(r.radius.y*2))+r.radius.y;
            fit = true;
            for(int i=0;i<presentRooms.size();i++){
                if(roomsOverlap(presentRooms[i], r, 2)){
                    fit = false;
                    break;
                }
            }
        }
        if(!fit){
            r.radius.set(0);
        }
        return r;
    }

    std::vector<Room> createRooms(int qty, int levelWidth, int levelHeight){
        std::vector<Room> rooms;
        for(int i=0;i<qty;i++){
            Room r = createRoom(levelWidth, levelHeight, rooms);
            if(r.radius.x > 0 && r.radius.y > 0){
                rooms.push_back(r);
            }
        }
        return rooms;
    }

    void makeRoomsAndPaths(std::vector<Room> rooms, Level* level){
        for(int i=0;i<rooms.size();i++){
            Room r = rooms[i];
            for(int j=-r.radius.x;j<=r.radius.x;j++){
                level->setTile(r.center.x+j, r.center.y+r.radius.y, tileWall);
                level->setTile(r.center.x+j, r.center.y-r.radius.y, tileWall);
            }
            for(int j=-r.radius.y;j<=r.radius.y;j++){
                level->setTile(r.center.x+r.radius.x, r.center.y+j, tileWall);
                level->setTile(r.center.x-r.radius.x, r.center.y+j, tileWall);
            }
        }

    }
}
