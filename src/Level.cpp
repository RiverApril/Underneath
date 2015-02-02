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
#include "ItemGenerator.h"
#include "Utility.h"

Level::Level(World* w, string n, Point2 s) {
    currentWorld = w;
    name = n;
    size = new Point2(s);
    tileGrid = vector<vector<TileData>>(size->x, vector<TileData>(size->y));
    
    for(int i=0;i<size->x;i++){
        for(int j=0;j<size->y;j++){
            tileGrid[i][j].index = tileUnset->getIndex();
            tileGrid[i][j].explored = false;
            //tileGrid[i][j].entity = nullptr;
        }
    }

    stairList = vector<Stair>();

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

bool Level::canSee(Point2 origin, Point2 test, double range){

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

bool Level::update(double time, Point2 viewPos) {
    bool u = false;
    for (Entity* e : entityList) {
        if(e->update(time, this)){
            u = true;
        }
    }
    while(removeEntityList.size() > 0){
        actuallyRemoveEntity(removeEntityList[0], false);
        removeEntityList.erase(removeEntityList.begin());
    }
    while(deleteEntityList.size() > 0){
        actuallyRemoveEntity(deleteEntityList[0], true);
        deleteEntityList.erase(deleteEntityList.begin());
    }
    return u;
}

void Level::renderMenuGame(double displayTime){
    if(menuGame!=nullptr){
    	menuGame->render(displayTime);
    }
}

/*void Level::setAndUnsetDisplayEntities(){
    for (size_t i=0; i<entityList.size(); i++) {
        entityList.at(i)->setAndUnsetDisplayEntity(shared_from_this());
    }
}*/

Entity* Level::newEntity(Entity* newE) {
    newE->uniqueId = nextUniqueId;
    nextUniqueId++;
    entityList.push_back(newE);
    return newE;
}

void Level::removeEntity(Entity* e, bool deleteEntity) {
    e->removed = true;
    if(deleteEntity){
        deleteEntityList.push_back(e);
    }else{
        removeEntityList.push_back(e);
    }
}
void Level::actuallyRemoveEntity(Entity* e, bool deleteEntity){
    for(int i=0;i<entityList.size();i++){
        if(e->uniqueId == entityList[i]->uniqueId){
            entityList.erase(entityList.begin()+i);
            debug("Removed Entity: "+e->getName());
            if(deleteEntity){
                string n = e->getName();
                delete e;
                debug("Deleted Entity: "+n);
            }
            return;
        }
    }
    debug("Failed to Remove Entity: "+e->getName());
}

vector<Point2> Level::getPathTo(Point2 from, Point2 to, TileFlag requiredFlag){
	vector<vector<int>> map = vector<vector<int>>(size->x, vector<int>(size->y));

    for(int i=0;i<size->x;i++){
        for(int j=0;j<size->y;j++){
            map[i][j] = -1;
        }
    }

    queue<Point2> priorityQueue;
    priorityQueue.push(from);

    while(!priorityQueue.empty()){
        Point2 c = priorityQueue.front();
        priorityQueue.pop();
        if(c == to){
            vector<Point2> path;
            Point2 l = to;
            int v = map[to.x][to.y];
            while(v > 0){
                bool leave = false;
                for(int i=-1;i<=1 && !leave;i++){
                    for(int j=-1;j<=1 && !leave;j++){
                        Point2 p = Point2(i+l.x, j+l.y);
                        if((abs(i)+abs(j)) == 1){
                        	if(inRange(p)){
                                if(map[p.x][p.y] < v && map[p.x][p.y] != -1){
                                    v = map[p.x][p.y];
                                    l = p;
                                    leave = true;
                                    path.push_back(l);
                                }
                            }
                        }
                    }
                }
                if(!leave){
                    break;
                }
            }
            return path;
        }else{
            for(int i=-1;i<=1;i++){
                for(int j=-1;j<=1;j++){
                    Point2 p = Point2(i+c.x, j+c.y);
                    if((abs(i)+abs(j)) == 1){
                    	if(inRange(p)){
                        	if(tileAt(p)->hasFlag(requiredFlag)){
                                if(map[p.x][p.y] == -1){
                                    map[p.x][p.y] = map[c.x][c.y]+1;
                                    priorityQueue.push(p);
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

bool Level::canPathTo(Point2 from, Point2 to, TileFlag requiredFlag){
    return !getPathTo(from, to, requiredFlag).empty();

    /*vector<vector<char>> map = vector<vector<char>>(size->x, vector<char>(size->y));
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

Point2 Level::generate(unsigned int seed, Point2 stairUpPos, string previousLevel) {
    srand(seed);

    do{

        for (int i=0; i<size->x; i++) {
            for (int j=0; j<size->y; j++) {
                tileGrid[i][j].index = tileUnset->getIndex();
                tileGrid[i][j].explored = false;
                //tileGrid[i][j].entity = nullptr;
                if(i==0 || j==0 || i==(size->x-1) || j==(size->y-1)){
                    tileGrid[i][j].index = tileWall->getIndex();
                }
            }
        }

        vector<LevelGenerator::Room*>* rooms = LevelGenerator::createRooms(1000, *size);
        LevelGenerator::makeRoomsAndPaths(rooms, this);

        for(int i=0;i<rooms->size();i++){
            delete rooms->at(i);
        }
        delete rooms;

    }while(tileAt(stairUpPos)->hasFlag(tileFlagSolid));

    //Point2 stairUpPos = entrance;
    Point2 stairDownPos;
    int dist = (size->x+size->y) / 2;
    while(true){
        stairDownPos = findRandomOfType(tileFloor->getIndex());
        //stairUpPos = findRandomOfType(tileFloor->getIndex());
        if((distanceSquared(stairUpPos, stairDownPos) > (dist*dist)) && canPathTo(stairUpPos, stairDownPos, tileFlagPathable)){
            break;
        }else{
            dist--;
            mvprintw(1, 0, ("dist: "+to_string(dist)+"   ").c_str());
            refresh();
            if(dist < 1){
                return Point2Neg1;
            }
        }
    }

    setTile(stairUpPos, tileStairUp);
    setTile(stairDownPos, tileStairDown);
    stairList.push_back(Stair(stairUpPos, true, previousLevel));
    stairList.push_back(Stair(stairDownPos, false, "Floor"+to_string(ParsingUtility::parseInt(name.substr(5))+1)));

    vector<Point2> path = getPathTo(stairUpPos, stairDownPos, tileFlagPathable);
    for(Point2 pe : path){
        if(!tileAt(pe)->isSolid()){
        	setTile(pe, tileDebug1);
        }
    }


    AiEntity* rat = new AiEntity("Rat", aiMoveRandom | aiFleeFromPlayerDumb, 'r', Point2Zero, Ui::C_DARK_YELLOW, 5);
    rat->viewDistance = 4;

    addEntitiesRandomly(stairUpPos, rat, (rand()%10)+20);


    AiEntity* goblin = new AiEntity ("Goblin", aiFollowPlayerDumb | aiAttackPlayer, 'g', Point2Zero, Ui::C_DARK_GREEN, 10);
    goblin->setActiveWeapon(ItemGenerator::createWeapon("", ItemGenerator::combat1Shitty, damMelee, false));
    goblin->setMoveDelay(Math::randomRange(.5, 1.2));

    addEntitiesRandomly(stairUpPos, goblin, (rand()%80)+20);


    AiEntity* troll = new AiEntity ("Troll", aiFollowPlayerSmart | aiAttackPlayer, 't', Point2Zero, Ui::C_DARK_RED, 15);
    troll->setActiveWeapon(ItemGenerator::createWeapon("", ItemGenerator::combat2Training, damMelee, false));

    addEntitiesRandomly(stairUpPos, troll, (rand()%100)+20);


    return stairUpPos;
    
    
}

template <typename T> void Level::addEntitiesRandomly(Point2 start, T* e, int count){

    for(int i=0;i<count;i++){
        T* r = dynamic_cast<T*>(Entity::clone(e));
        Point2 p = Point2(findRandomWithoutFlag(tileFlagSolid));
        if(canPathTo(start, p, tileFlagPathable | tileFlagSecretPathable)){
            r->pos = p;
        	newEntity(r);
        }else{
            i--;
        }
    }
}

void Level::save(vector<unsigned char>* data){
    
    Point2::save(*size, data);
    for(int i=0;i<size->x;i++){
        for(int j=0;j<size->y;j++){
            FileUtility::saveInt8Bit(data, tileGrid[i][j].index);
            FileUtility::saveBool(data, tileGrid[i][j].explored);
        }
    }
    FileUtility::saveInt(data, (int)entityList.size());
    for(int i=0;i<entityList.size();i++){
        debug("Saving Entity: "+entityList[i]->getName()+"("+to_string(entityList[i]->getEntityTypeId())+")"+", Pos: "+entityList[i]->pos.toString());
        entityList[i]->save(data);
    }
}

void Level::load(unsigned char* data, int* position){

    for(int i=0;i<size->x;i++){
        for(int j=0;j<size->y;j++){
            tileGrid[i][j].index = FileUtility::loadInt8Bit(data, position);
            tileGrid[i][j].explored = FileUtility::loadBool(data, position);
        }
    }
    debug("Loaded "+to_string(size->x)+" x "+to_string(size->y)+" Tiles");

    debugf("Entity Count bytes: %X %X %X %X", data[*position], data[*position+1], data[*position+2], data[*position+3]);
    debug("Entity Count 4th byte: "+to_string(data[*position+3]));
    int entityCount = FileUtility::loadInt(data, position);
    debug(to_string(entityCount)+" Entities to Load...");
    for(int i=0;i<entityCount;i++){
        entityList.push_back(Entity::loadNew(data, position));
    }
    debug("Loaded "+to_string(entityCount)+" Entities");
}


