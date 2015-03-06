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
#include "EnemyGenerator.h"
#include "Utility.h"

Level::Level(World* w, string n, Point2 s, int d) {
    currentWorld = w;
    name = n;
    difficulty = d;
    size = new Point2(s);
    tileGrid = vector<vector<TileData>>(size->x, vector<TileData>(size->y));
    
    for(size_t i=0;i<size->x;i++){
        for(size_t j=0;j<size->y;j++){
            tileGrid[i][j].index = (int8_t)Tiles::tileUnset->getIndex();
            tileGrid[i][j].explored = false;
            //tileGrid[i][j].entity = nullptr;
        }
    }

    tileEntityList = vector<TileEntity*>();

}

Level::~Level() {
    delete size;

    entityList.erase(entityList.begin(), entityList.end());
    deleteEntityList.erase(deleteEntityList.begin(), deleteEntityList.end());
    removeEntityList.erase(removeEntityList.begin(), removeEntityList.end());
    tileEntityList.erase(tileEntityList.begin(), tileEntityList.end());
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
        tileGrid[p.x][p.y].index = (int8_t)tile;
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
    if(inRange(p)) {
        return tileGrid[p.x][p.y].index;
    }
    return Tiles::tileEdge->getIndex();
}

bool Level::canSee(Point2 origin, Point2 test, double range, bool withWalls){

    if(Math::distanceSquared(origin.x, origin.y, test.x, test.y) > range*range){
        return false;
    }

    Vector2 step = ((test-origin)/((double)range));

    Vector2 v;
    Point2 p;

    for(double i=0;i<range;i+=.2){

        v = (step*i);

        if(withWalls){
            p = v.roundToward0();
        }else{
            p = v.roundAwayFrom0();
        }

        if(tileAt(origin+(p))->isTall()){
            return false;
        }
    }

    return true;
}

Entity* Level::getClosestVisableEntity(Point2 origin, double range, Entity* notMe){
    Entity* closest = nullptr;
    double lastDistSquared = size->xPlusY();
    for(Entity* e : entityList){
        if(e == notMe){
            continue;
        }
        if(canSee(origin, e->pos, range, true)){
            if(e != nullptr){
                double temp = distanceSquared(origin, e->pos);
                if(temp < lastDistSquared){
                    closest = e;
                    lastDistSquared = temp;
                }
            }
        }
    }
    return closest;
}

vector<Entity*> Level::getAllVisableEntitiesSortedByNearest(Point2 origin, double range, Entity* notMe){
    vector<Entity*> list;

    for(Entity* e : entityList){
        if(e == notMe){
            continue;
        }
        if(canSee(origin, e->pos, range, true)){
            if(e != nullptr){
                list.push_back(e);
            }
        }
    }

    size_t n = list.size();

    for(size_t x=0; x<n; x++){
        for(size_t y=0; y<n-1; y++){
            if(distanceSquared(origin, list[y]->pos) > distanceSquared(origin, list[y+1]->pos)){
                Entity* temp = list[y+1];
                list[y+1] = list[y];
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
    for (Entity* e : entityList) {
        if(e->update(deltaTime, time, this)){
            u = true;
        }
    }
    while(removeEntityList.size() > 0){
        actuallyRemoveEntityUnsafe(removeEntityList[0], false);
        removeEntityList.erase(removeEntityList.begin());
    }
    while(deleteEntityList.size() > 0){
        actuallyRemoveEntityUnsafe(deleteEntityList[0], true);
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
void Level::actuallyRemoveEntityUnsafe(Entity* e, bool deleteEntity){
	for (size_t i = 0; i<entityList.size(); i++){
        if(e->uniqueId == entityList[i]->uniqueId){
            entityList.erase(entityList.begin()+(long)i);
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

    for(size_t i=0;i<size->x;i++){
        for(size_t j=0;j<size->y;j++){
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

void Level::genDebug(string s){
	Ui::setColor(Ui::C_WHITE);
    move(genDebugPos, 0);
    clrtoeol();
    mvprintw(genDebugPos, 0, s.c_str());
    genDebugPos ++;

    if(genDebugPos >= Ui::terminalSize.y){
        genDebugPos = 1;
        move(genDebugPos, 0);
        clrtobot();
    }
    refresh();
}

Point2 Level::generate(unsigned int seed, Point2 stairUpPos, string previousLevel) {

    genDebugPos = 1;


    srand(seed);

    int attemt = 0;

    bool pathNotFound = false;

    Point2 stairDownPos;

    do{

        do{

            for (int i=0; i<size->x; i++) {
                for (int j=0; j<size->y; j++) {
                    tileGrid[i][j].index = (int8_t)Tiles::tileUnset->getIndex();
                    tileGrid[i][j].explored = false;
                    //tileGrid[i][j].entity = nullptr;
                    if(i==0 || j==0 || i==(size->x-1) || j==(size->y-1)){
                        tileGrid[i][j].index = (int8_t)Tiles::tileWall->getIndex();
                    }
                }
            }
            genDebug("generating...  attemt #"+to_string(attemt));
            attemt++;


            vector<LevelGenerator::Room*>* rooms = LevelGenerator::createRooms(1000, *size);
            LevelGenerator::makeRoomsAndPaths(rooms, this);

            genDebug("generated");

            for (size_t i = 0; i<rooms->size(); i++){
                delete rooms->at(i);
            }
            delete rooms;


        }while(tileAt(stairUpPos)->getIndex() != Tiles::tileFloor->getIndex());


        pathNotFound = false;

        int dist = (size->x+size->y) / 2;
        attemt = 0;
        while(true){
            genDebug("looking for exit location  attemt #"+to_string(attemt));
            attemt++;
            stairDownPos = findRandomOfType(Tiles::tileFloor->getIndex());
            //stairUpPos = findRandomOfType(tileFloor->getIndex());
            if((distanceSquared(stairUpPos, stairDownPos) > (dist*dist)) && canPathTo(stairUpPos, stairDownPos, tileFlagPathable)){
                break;
            }else{
                dist--;
                genDebug("distance between entance and exit: "+to_string(dist));
                if(dist < 1){
                    pathNotFound = true;
                    break;
                }
            }
        }

    }while(pathNotFound);

    genDebug("found path");

    genDebug("Filling chests...");
    for (int i=0; i<size->x; i++) {
        for (int j=0; j<size->y; j++) {
            if(tileGrid[i][j].index == (int8_t)Tiles::tileChest->getIndex()){
                TEChest* te = new TEChest(Point2(i, j));
                te->addItems(ItemGenerator::createLootsRand(difficulty));
                tileEntityList.push_back(te);
            }
        }
    }

    setTile(stairUpPos, Tiles::tileStairUp);
    setTile(stairDownPos, Tiles::tileStairDown);
    if(previousLevel.size() > 0){
    	tileEntityList.push_back(new TEStair(stairUpPos, true, previousLevel));
    }
    tileEntityList.push_back(new TEStair(stairDownPos, false, "Floor"+to_string(ParsingUtility::parseInt(name.substr(5))+1)));

    for(TileEntity* e : tileEntityList){
        debugf("TileEntity id: %d", e->getTileEntityTypeId());
    }
    

    vector<Point2> path = getPathTo(stairUpPos, stairDownPos, tileFlagPathable);
    for(Point2 pe : path){
        if(!tileAt(pe)->isSolid()){
        	setTile(pe, Tiles::tileDebug1);
        }
    }

    //return stairUpPos;

    genDebug("adding entities...");

    /*

    {
        AiEntity* e = new AiEntity("Rat", aiMoveRandom | aiFleeFromPlayer, 'r', Point2Zero, Ui::C_DARK_YELLOW, 5);
        e->viewDistance = 6;
        e->setMoveDelay(Math::randomRange(.5, 1.5));

        int count = (rand()%20)+10;
        for(int i=0;i<count;i++){

            placeNewAiEntity(dynamic_cast<AiEntity*>(Entity::clone(e)), stairUpPos);

        }

    }


    {
        AiEntity* e = new AiEntity ("Goblin", aiAttackPlayer, 'g', Point2Zero, Ui::C_DARK_GREEN, 10);
        e->setActiveWeapon(ItemGenerator::createWeapon(difficulty, damMelee));
        e->setMoveDelay(Math::randomRange(.5, 1.5));

        int count = (rand()%80)+20;
        for(int i=0;i<count;i++){
            placeNewAiEntity(dynamic_cast<AiEntity*>(Entity::clone(e)), stairUpPos);
        }
    }

    {
        AiEntity* e = new AiEntity ("Troll", aiAttackPlayer, 't', Point2Zero, Ui::C_DARK_RED, 15);
        e->setActiveWeapon(ItemGenerator::createWeapon(difficulty, damMelee));
        e->setMoveDelay(Math::randomRange(.5, 1.5));

        int count = (rand()%20)+20;
        for(int i=0;i<count;i++){
            placeNewAiEntity(dynamic_cast<AiEntity*>(Entity::clone(e)), stairUpPos);
        }
    }

    {

        AiEntity* e = new AiEntity ("Goblin Archer", aiAttackPlayer, 'a', Point2Zero, Ui::C_DARK_GREEN, 8);
        e->setActiveWeapon(ItemGenerator::createWeapon(difficulty, damRanged));
        e->setMoveDelay(Math::randomRange(.5, 1.5));

        int count = (rand()%40)+20;
        for(int i=0;i<count;i++){

            placeNewAiEntity(dynamic_cast<AiEntity*>(Entity::clone(e)), stairUpPos);
        }
    }
     
     */
    //
    int count = (rand()%200)+100;
    for(int i=0;i<count;i++){
        placeNewAiEntity(EnemyGenerator::makeRandomEntity(), stairUpPos);
    }

    genDebug("done");


    return stairUpPos;
    
    
}

void Level::placeNewAiEntity(AiEntity* e, Point2 entrance){
    Point2 p;
    do{
    	p = Point2(findRandomWithoutFlag(tileFlagSolid));
    }while(!canPathTo(entrance, p, tileFlagPathable | tileFlagSecretPathable) && canSee(entrance, p, e->viewDistance, false));

    e->pos = p;
    newEntity(e);
}

void Level::save(vector<unsigned char>* data){

    Point2::save(*size, data);
    FileUtility::saveInt(data, difficulty);
    for(size_t i=0;i<size->x;i++){
        for(size_t j=0;j<size->y;j++){
            FileUtility::saveInt8Bit(data, tileGrid[i][j].index);
            FileUtility::saveBool(data, tileGrid[i][j].explored);
        }
    }
    FileUtility::saveInt(data, (int)entityList.size());
    debugf("Saving %d entities...", (int)entityList.size());
	for (size_t i = 0; i<entityList.size(); i++){
        //debug("Saving Entity: "+entityList[i]->getName()+"("+to_string(entityList[i]->getEntityTypeId())+")"+", Pos: "+entityList[i]->pos.toString());
        entityList[i]->save(data);
    }
    debugf("%d Entities saved.", (int)entityList.size());
    debugf("Saving %d tile entities...", (int)tileEntityList.size());
    FileUtility::saveInt(data, (int)tileEntityList.size());
    for (size_t i = 0; i<tileEntityList.size(); i++) {
        tileEntityList[i]->save(data);
    }
    debugf("%d Tile entities saved.", (int)tileEntityList.size());
}

void Level::load(unsigned char* data, int* position){

    for(size_t i=0;i<size->x;i++){
        for(size_t j=0;j<size->y;j++){
            tileGrid[i][j].index = FileUtility::loadInt8Bit(data, position);
            tileGrid[i][j].explored = FileUtility::loadBool(data, position);
        }
    }
    debug("Loaded "+to_string(size->x)+" x "+to_string(size->y)+" Tiles");

    int entityCount = FileUtility::loadInt(data, position);
    debugf("%d Entities to Load...", entityCount);
    for(int i=0; i<entityCount; i++){
        entityList.push_back(Entity::loadNew(data, position));
    }
    debugf("Loaded %d entities", (int)entityList.size());

    int tileEntityCount = FileUtility::loadInt(data, position);
    debugf("%d Tile entities to Load...", tileEntityCount);
    for (int i=0; i<tileEntityCount; i++) {
        tileEntityList.push_back(TileEntity::loadNew(data, position));
    }
    debugf("Loaded %d tile entities", (int)tileEntityList.size());
}



