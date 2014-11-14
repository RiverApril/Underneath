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

Level::Level(shared_ptr<World> w, string n, Point2 s) {
    currentWorld = w;
    name = n;
    size = new Point2(s);
    tileGrid = vector<vector<TileData>>(size->x, std::vector<TileData>(size->y));
    
    for(int i=0;i<size->x;i++){
        for(int j=0;j<size->y;j++){
            tileGrid[i][j].index = tileUnset->getIndex();
            tileGrid[i][j].explored = false;
            tileGrid[i][j].entity = nullptr;
        }
    }
}

Level::~Level() {
    delete size;
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

shared_ptr<Entity> Level::getDisplayEntity(Point2 p){
    if(inRange(p)) {
    	return tileGrid[p.x][p.y].entity;
    }else{
        return nullptr;
    }
}

void Level::setDisplayEntity(Point2 p, shared_ptr<Entity> e){
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

    if(Math::distanceSquared(origin.x, origin.y, test.x, test.y) > range*range){
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
        if(entityList.at(i)->update(tick, shared_from_this())){
            u = true;
        }
    }
    return u;
}

shared_ptr<Entity> Level::newEntity(shared_ptr<Entity> newE) {
    newE->uniqueId = nextUniqueId;
    nextUniqueId++;
    entityList.push_back(newE);
    return newE;
}

void Level::deleteEntity(shared_ptr<Entity> e) {
    for(int i=0;i<entityList.size();i++){
        if(e->uniqueId == entityList[i]->uniqueId){
            entityList.erase(entityList.begin()+i);
            break;
        }
    }
}

bool Level::canPathTo(Point2 from, Point2 to){
    vector<vector<char>> map = vector<vector<char>>(size->x, vector<char>(size->y));
    for(int i=0;i<size->x;i++){
        for(int j=0;j<size->y;j++){
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
            for(int i=c.x-1;i<=c.x+1;i++){
                for(int j=c.y-1;j<=c.y+1;j++){
                    Point2 p = Point2(i, j);
                    if(inRange(p)){
                        if(!tileAt(p)->isSolid() || tileAt(p)->hasFlag(tileFlagDoor)){
                            if(c != p){
                                if(map[i][j] == 'U'){
                                    map[i][j] = 'V';
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
    return false;

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

    shared_ptr<vector<shared_ptr<LevelGenerator::Room>>> rooms = LevelGenerator::createRooms(1000, *size);
    LevelGenerator::makeRoomsAndPaths(rooms, shared_from_this());

    Point2 p;

    p = findRandomOfType(tileFloor->getIndex());
    setTile(p, tileStairUp);

    int fail = 0;

    for(int i=0;i<10;i++){
        Point2 f = findRandomOfType(tileFloor->getIndex());
        if(((distanceSquared(f, p) > 20*20) || fail>20) && canPathTo(p, f)){
        	setTile(f, tileStairDown->getIndex());
        }else{
            i--;
            fail++;
        }
    }


    shared_ptr<AiEntity> rat = shared_ptr<AiEntity>(new AiEntity("Rat", aiMoveRandom, 'r', Point2Zero, Ui::C_DARK_YELLOW, 5));
    int ratCount = (rand()%50)+10;
    for(int i=0;i<ratCount;i++){
        shared_ptr<AiEntity> r = AiEntity::clone(rat, nullptr);
        Point2 pp = Point2(findRandomWithoutFlag(tileFlagSolid));
        r->setPos(pp);
        newEntity(r);
    }

    shared_ptr<AiEntity> goblin = shared_ptr<AiEntity>(new AiEntity("Goblin", aiFollowPlayerDumb | aiAttackPlayer, 'g', Point2Zero, Ui::C_DARK_GREEN, 10));
    goblin->setActiveWeapon(shared_ptr<Weapon>(new Weapon(1, "Rusted Spear")));
    int goblinCount = (rand()%40)+10;
    for(int i=0;i<goblinCount;i++){
        shared_ptr<AiEntity> g = AiEntity::clone(goblin, nullptr);
        Point2 pp = Point2(findRandomWithoutFlag(tileFlagSolid));
        g->setPos(pp);
        newEntity(g);
    }


    return p;
    
    
}

void Level::save(string* data){
    
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



