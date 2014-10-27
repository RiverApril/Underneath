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

    std::vector<LevelGenerator::Room*>* rooms = LevelGenerator::createRooms(1000, WIDTH, HEIGHT);
    LevelGenerator::makeRoomsAndPaths(rooms, this);


}

namespace LevelGenerator{

    const int OPTION_ROOM_RADIUS_MAX = 10;
    const int OPTION_ROOM_RADIUS_MIN = 2;

    const int OPTION_ROOM_ENTRANCES_MAX = 8;
    const int OPTION_ROOM_ENTRANCES_MIN = 1;

    bool roomsOverlap(Room* a, Room* b, int border){
        return !(((a->center.x-a->radius.x-border) > (b->center.x+b->radius.x+border) ||
        (b->center.x-b->radius.x-border) > (a->center.x+a->radius.x+border)) ||
        ((a->center.y-a->radius.y-border) > (b->center.y+b->radius.y+border) ||
        (b->center.y-b->radius.y-border) > (a->center.y+a->radius.y+border)));
    }

    Room* createRoom(int levelWidth, int levelHeight, std::vector<Room*>* presentRooms){
        bool fit = false;
        Room* r = new Room();
        int att = 0;
        while(!fit && att<100){
            att++;
            r->radius.x = (rand()%(OPTION_ROOM_RADIUS_MAX-OPTION_ROOM_RADIUS_MIN))+OPTION_ROOM_RADIUS_MIN;
            r->radius.y = (rand()%(OPTION_ROOM_RADIUS_MAX-OPTION_ROOM_RADIUS_MIN))+OPTION_ROOM_RADIUS_MIN;
            r->center.x = rand()%(levelWidth-(r->radius.x*2))+r->radius.x;
            r->center.y = rand()%(levelHeight-(r->radius.y*2))+r->radius.y;
            fit = true;
            for(int i=0;i<presentRooms->size();i++){
                if(roomsOverlap(presentRooms->at(i), r, 2)){
                    fit = false;
                    break;
                }
            }
        }
        if(!fit){
            r->radius.set(0);
        }else{
            int ec = (rand()%(OPTION_ROOM_ENTRANCES_MAX-OPTION_ROOM_ENTRANCES_MIN))+OPTION_ROOM_ENTRANCES_MIN;
            for(int i=0;i<ec;i++){
                Entry* e = new Entry();
                e->direction = rand()%4;
                int rr = (e->direction==left||e->direction==right?r->radius.y:r->radius.x)-1;
                e->offset = ((rand()%(rr*2-1))-rr);
                bool tooClose = false;
                for(int j=0;j<r->entrances->size();j++){
                    Entry* ee = r->entrances->at(j);
                    if(ee->direction == e->direction){
                        if(abs(ee->offset-e->offset)<=2){
                            tooClose = true;
                            break;
                        }
                    }
                }
                if(!tooClose){
                    r->entrances->push_back(e);
                }
            }
        }
        return r;
    }

    std::vector<Room*>* createRooms(int qty, int levelWidth, int levelHeight){
        std::vector<Room*>* rooms = new std::vector<Room*>();
        for(int i=0;i<qty;i++){
            Room* r = createRoom(levelWidth, levelHeight, rooms);
            if(r->radius.x > 0 && r->radius.y > 0){
                rooms->push_back(r);
            }
        }
        return rooms;
    }

    void makeRoomsAndPaths(std::vector<Room*>* rooms, Level* level){
        for(int i=0;i<rooms->size();i++){
            Room* r = rooms->at(i);
            for(int j=-r->radius.x;j<=r->radius.x;j++){
                level->setTile(r->center.x+j, r->center.y+r->radius.y, tileWall);
                level->setTile(r->center.x+j, r->center.y-r->radius.y, tileWall);
            }
            for(int j=-r->radius.y;j<=r->radius.y;j++){
                level->setTile(r->center.x+r->radius.x, r->center.y+j, tileWall);
                level->setTile(r->center.x-r->radius.x, r->center.y+j, tileWall);
            }

            for(int j=0;j<r->entrances->size();j++){
                Entry* e = r->entrances->at(j);
                e->x = (e->direction==left?-r->radius.x:(e->direction==right?r->radius.x:e->offset))+r->center.x;
                e->y = (e->direction==up?-r->radius.y:(e->direction==down?r->radius.y:e->offset))+r->center.y;
                level->setTile(e->x, e->y, tileEdge);
            }
        }
        for(int i=0;i<rooms->size();i++){
            Room* r = rooms->at(i);

            for(int j=0;j<r->entrances->size();j++){
                Entry* e = r->entrances->at(j);

                for(int k=0;k<rooms->size();k++){
                    Room* ro = rooms->at(k);

                    if(r->center==ro->center){
                        continue;
                    }

                    for(int l=0;l<r->entrances->size();l++){
                        Entry* eo = r->entrances->at(l);

                        if((e->direction!=eo->direction)){
                            int ddif = 1;
                            if((e->direction==left && eo->direction==right)||
                            (e->direction==right && eo->direction==left)||
                            (e->direction==up && eo->direction==down)||
                            (e->direction==down && eo->direction==up)){
                                ddif = 2;
                            }
                            int xdif = abs(e->x-eo->x);
                            int ydif = abs(e->y-eo->y);
                            bool blocked = false;
                            if(ddif == 1){
                                if(e->direction == right){
                                    for(int m=1;m<xdif;m++){
                                        if(level->indexAt(e->x+m, e->y)!=tileAir->getIndex()){
                                            blocked = true;
                                            break;
                                        }
                                    }
                                    for(int m=1;m<ydif;m++){
                                        if(level->indexAt(e->x+xdif, e->y+m)!=tileAir->getIndex()){
                                            blocked = true;
                                            break;
                                        }
                                    }
                                    if(!blocked){
                                        for(int m=1;m<xdif;m++){
                                            level->setTile(e->x+m, e->y, tileEdge);
                                        }
                                        for(int m=1;m<ydif;m++){
                                            level->setTile(e->x+xdif, e->y+m, tileEdge);
                                        }
                                    }
                                }
                            }
                        }

                    }
                }
            }
        }


    }
}
