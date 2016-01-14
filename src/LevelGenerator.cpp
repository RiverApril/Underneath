//
//  LevelGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "LevelGenerator.hpp"
#include "EnemyGenerator.hpp"
#include "TEMimic.hpp"
#include "EntityShop.hpp"


Point2 Level::generateStartArea(Point2 stairUpPos, string previousLevel){

    Point2 center = size / 2;

    int ring = center.xPlusY() * .2;
    int deepRing = center.xPlusY() * .5;

    debug("Gen: Start");

    if(distanceSquared(stairUpPos, center) < deepRing*deepRing){
        return Point2Neg1;
    }

    int8_t ind = (int8_t) Tiles::tileUnset->getIndex();

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            tileGrid[i][j].index = ind;
            tileGrid[i][j].explored = false;
        }
    }

    debug("Gen: Filled all to unset");

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            Point2 here = Point2(i, j);

            setTile(here, Tiles::tileGrass);
            int distance = sqrt(distanceSquared(here, center));

            if((rand() % (distance+1)) > ring){
                setTile(here, Tiles::tileTree);
            }

            /*if(distance > outerRing && (rand() % 10) != 0){
                setTile(here, Tiles::tileTree);
            }else if(distance > innerRing && (rand() % 10) == 0){
                setTile(here, Tiles::tileTree);
            }*/
        }
    }

    debug("Gen: Filled in Trees and Grass");


    //RUIN
    int centerRoomRadius = (rand() % 3) + 4;

    int centerRoomInnerRadius = centerRoomRadius - 2;


    Utility::executeGrid(center-centerRoomRadius, center+centerRoomRadius, [this](int x, int y){
        setTile(Point2(x, y), Tiles::tileFloor);
    });

    Utility::executeBorder(center-centerRoomInnerRadius, center+centerRoomInnerRadius, [this](int x, int y){
        if(rand()%6 != 0 && (x+y)%2 == 1){
            setTile(Point2(x, y), Tiles::tilePillar);
        }
    });

    Utility::executeBorder(center-centerRoomRadius, center+centerRoomRadius, [this](int x, int y){
        setTile(Point2(x, y), Tiles::tileWall);
    });


    Point2 door = center;

    Point2 d = (stairUpPos-center);

    if(abs(d.x) < abs(d.y)){
        door.x += d.x>0?centerRoomRadius:-centerRoomRadius;
        //door.y += (rand()%centerRoomRadius*2)-centerRoomRadius;
    }else{
        //door.x += (rand()%centerRoomRadius*2)-centerRoomRadius;
        door.y += d.y>0?centerRoomRadius:-centerRoomRadius;
    }



    setTile(door, Tiles::tileDoor);

    debug("Gen: Created Ruin");


    /*int r = (rand() % 3) + 2;

    Utility::executeGrid(stairUpPos-r*4, stairUpPos+r*4, [this, stairUpPos, r](int x, int y){
        if(distanceSquared(stairUpPos, Point2(x, y)) < r*r*8){
            setTile(Point2(x, y), Tiles::tileGrass);
        }
    });*/


    /*HOUSE
    Point2 houseCenter = stairUpPos;
    int houseRadius = (rand() % 3) + 2;

    Utility::executeGrid(houseCenter-houseRadius*4, houseCenter+houseRadius*4, [this, houseCenter, houseRadius](int x, int y){
        if(distanceSquared(houseCenter, Point2(x, y)) < houseRadius*houseRadius*8){
        	setTile(Point2(x, y), Tiles::tileGrass);
        }
    });

    Utility::executeGrid(houseCenter-houseRadius, houseCenter+houseRadius, [this](int x, int y){
        setTile(Point2(x, y), Tiles::tileFloor);
    });

    Utility::executeBorder(houseCenter-houseRadius, houseCenter+houseRadius, [this](int x, int y){
        setTile(Point2(x, y), Tiles::tileWall);
    });

    door = houseCenter;

    if((rand()&2)==0){
        door.x += (rand()&2)==0?houseRadius:-houseRadius;
        door.y += (rand()%houseRadius*2)-houseRadius;
    }else{
        door.x += (rand()%houseRadius*2)-houseRadius;
        door.y += (rand()&2)==0?houseRadius:-houseRadius;
    }

    setTile(door, Tiles::tileDoor);*/






    stairDownPos = center;

    setTile(stairDownPos, Tiles::tileStairDown);
    tileEntityList.push_back(new TEStair(stairDownPos, false, "Floor 1"));

    debug("Gen: Stair down placed");


    int count = (rand() % 20)+5;

    for(int i=0;i<count;i++){
        Entity* e = makeEntity(EnemyGenerator::bunny, difficulty);
        e->pos = findRandomOfType(Tiles::tileGrass->getIndex());
        newEntity(e);
    }

    debug("Gen: Added Bunnies");

    vector<Point2> path = getPathTo(stairUpPos, stairDownPos, tileFlagPathable);
    if(path.size() == 0){
        debug("Gen: No path found");
        return Point2Neg1;
    }
    for(Point2 p : path){
        Utility::executeGrid(p-1, p+1, [this](int x, int y){
            if(tileAt(Point2(x, y))->getIndex() == Tiles::tileTree->getIndex()){
            	setTile(Point2(x, y), Tiles::tileGrass);
            }
        });
        if(rand()%3 != 0){
            if(tileAt(p)->getIndex() == Tiles::tileGrass->getIndex()){
                setTile(p, Tiles::tilePath);
            }
        }
    }

    debug("Gen: Added the path");

    debug("Gen: Done");

    return stairUpPos;


}

Point2 Level::generateDungeon(Point2 stairUpPos, string previousLevel){

    int attemt = 0;

    bool pathNotFound = false;

    stairDownPos = Point2Zero;

    do {

        do {

            for (int i = 0; i < size.x; i++) {
                for (int j = 0; j < size.y; j++) {
                    tileGrid[i][j].index = (int8_t) Tiles::tileUnset->getIndex();
                    tileGrid[i][j].explored = false;
                }
            }
            debugf("generating...  attemt #%d", attemt);
            attemt++;


            vector<LevelGenerator::Room*>* rooms = LevelGenerator::createRooms(stairUpPos, 1000, size);
            this->entityList.clear();
            LevelGenerator::makeRoomsAndPaths(rooms, this);

            debugf("generated");

            for (size_t i = 0; i < rooms->size(); i++) {
                delete rooms->at(i);
            }
            rooms->clear();
            delete rooms;


        } while (tileAt(stairUpPos)->getIndex() != Tiles::tileFloor->getIndex());


        pathNotFound = false;

        int dist = (size.xPlusY()) / 2;
        attemt = 0;
        while (true) {
            debugf("looking for exit location   attemt #%d", attemt);
            attemt++;
            stairDownPos = findRandomOfType(Tiles::tileFloor->getIndex());
            if ((distanceSquared(stairUpPos, stairDownPos) > (dist * dist)) && canPathTo(stairUpPos, stairDownPos, tileFlagPathable)) {
                break;
            } else {
                dist--;
                debugf("distance between entance and exit: %d", dist);
                if (dist < ((size.xPlusY() / 2) / 10)) {
                    pathNotFound = true;
                    break;
                }
            }
        }

    } while (pathNotFound);

    debugf("found path");

    debugf("Adding Tile Entities...");
    int chestCount = 0;
    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {

            if (tileGrid[i][j].index == (int8_t) Tiles::tileDoor->getIndex() || tileGrid[i][j].index == (int8_t) Tiles::tileSecretDoor->getIndex() || tileGrid[i][j].index == (int8_t) Tiles::tileChest->getIndex()) {
                if (rand() % 50 == 0) {
                    TEMimic* te = new TEMimic(Point2(i, j));
                    tileEntityList.push_back(te);
                    continue;
                }
            }

            if (tileGrid[i][j].index == (int8_t) Tiles::tileChest->getIndex()) {
                TEChest* te = new TEChest(Point2(i, j));
                //te->addItems(ItemGenerator::createRandLoots(difficulty, difficulty * 100, 5, 5, 5, 10));
                te->lootProfileIndex = ItemGenerator::lootProfileChest;
                tileEntityList.push_back(te);
                chestCount++;
                continue;
            }

            /*if (tileGrid[i][j].index == (int8_t) Tiles::tileCrate->getIndex()) {
                if (rand() % 6 == 0) {
                    TEChest* te = new TEChest(Point2(i, j));
                    te->addItem(ItemGenerator::makeCoins((rand() % 30) + 1));
                    tileEntityList.push_back(te);
                    continue;
                } else if (rand() % 10 == 0) {
                    TEChest* te = new TEChest(Point2(i, j));
                    //te->addItems(ItemGenerator::createRandLoots(difficulty, 0, 0, 0, 1, 100));
                    te->lootProfileIndex = ItemGenerator::lootProfileCrate;
                    tileEntityList.push_back(te);
                    continue;
                }
            }*/
        }
    }

    debugf("Chest Count: %d", chestCount);

    setTile(stairUpPos, Tiles::tileStairUp);
    setTile(stairDownPos, Tiles::tileStairDown);
    if (previousLevel.size() > 0) {
        tileEntityList.push_back(new TEStair(stairUpPos, true, previousLevel));
    }
    tileEntityList.push_back(new TEStair(stairDownPos, false, "Floor" + to_string(Utility::parseInt(name.substr(5)) + 1)));


    debugf("Counting Solid Tiles...");


    int nonsolidAccessableTileCount = 0;
    int unaccessableTileCount = 0;

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            if(!tileAt(Point2(i, j))->hasAnyOfFlags(tileFlagSolidBoth)){
                nonsolidAccessableTileCount++;
            }else{
                unaccessableTileCount++;
            }
        }
    }


    debugf("Adding Entities...");

    EnemyGenerator::setIntervals(difficulty);

    map<string, int> entitiesGenerated;

    int maxCountDiv2 = max(((nonsolidAccessableTileCount)/30)/2, 1);

    int count = (rand() % (maxCountDiv2))+(maxCountDiv2);
    for (int i = 0; i < count; i++) {
        EntityAi* e = EnemyGenerator::makeRandomEntity(difficulty);
        if(entitiesGenerated.count(e->getName())){
            entitiesGenerated[e->getName()]++;
        }else{
            entitiesGenerated[e->getName()] = 1;
        }
        placeNewEntityAi(e, stairUpPos);
    }

    for(pair<string, int> p : entitiesGenerated){
        debugf("%s x%d", p.first.c_str(), p.second);
    }

    debugf("done");


    return stairUpPos;


}


namespace LevelGenerator {


    bool roomsOverlap(Room* a, Room* b, int border) {
        return !(((a->pos.x - border) > (b->pos.x + b->size.x + border) ||
                (b->pos.x - border) > (a->pos.x + a->size.x + border)) ||
                ((a->pos.y - border) > (b->pos.y + b->size.y + border) ||
                (b->pos.y - border) > (a->pos.y + a->size.y + border)));
    }

    Room* createRoom(Point2 levelSize, vector<Room*>* presentRooms, Point2 pos) {
        bool fit = false;
        Room* r = new Room();
        int att = 0;
        while (!fit && att < 100) {
            att++;
            r->size.x = (((rand() % 2 == 0) ? (rand() % 6) : (rand() % 12)) + 3)*2;
            r->size.y = (((rand() % 2 == 0) ? (rand() % 6) : (rand() % 12)) + 3)*2;
            if (pos == Point2Neg1) {
                r->pos.x = ((rand() % (levelSize.x - (r->size.x)))/2)*2;
                r->pos.y = ((rand() % (levelSize.y - (r->size.y)))/2)*2;
            } else {
                r->pos.x = pos.x;
                r->pos.y = pos.y;
            }
            fit = true;
            for (size_t i = 0; i < presentRooms->size(); i++) {
                if (roomsOverlap(presentRooms->at(i), r, 1)) {
                    fit = false;
                    break;
                }
            }
        }
        if (!fit) {
            r->size.set(0);
        }
        return r;
    }

    vector<Room*>* createRooms(Point2 startPos, int maxQty, Point2 levelSize) {
        vector<Room*>* rooms = new vector<Room*>();
        //Room* centerRoom = createRoom(roomSize, rooms, startPos);
        //rooms->push_back(centerRoom);
        for (int i = 1; i < maxQty; i++) {
            Room* r = createRoom(levelSize, rooms);
            if (r->size.x > 0 && r->size.y > 0) {
                rooms->push_back(r);
            }else{
                delete r;
            }
        }
        return rooms;
    }

    /*int getPathAndMaybeDoor() {
        return (rand() % 30 == 0 ? (rand() % 10 == 0 ? Tiles::tileSecretDoor : Tiles::tileDoor) : Tiles::tileFloor)->getIndex();
    }*/

    void makeRoomsAndPaths(vector<Room*>* rooms, Level* level) {


        Point2 size = level->getSize();

        vector<vector<char>> grid = vector<vector<char>>(size.x, vector<char>(size.y));

        for (int i = 0; i < size.x; i++) {
            for (int j = 0; j < size.y; j++) {
                grid[i][j] = 'u';
                if(i == 0 || j == 0 || i == level->getSize().x-1 || j == level->getSize().y-1){
                    grid[i][j] = 'x';
                }
            }
        }


        for (Room* r : *rooms) {
            for (int i = r->pos.x; i <= r->pos.x+r->size.x; i++) {
                level->setTile(Point2(i, r->pos.y), Tiles::tileWall);
                level->setTile(Point2(i, r->pos.y+r->size.y), Tiles::tileWall);
                grid[i][r->pos.y] = 'x';
                grid[i][r->pos.y+r->size.y] = 'x';
            }
            for (int i = r->pos.y; i <= r->pos.y+r->size.y; i++) {
                level->setTile(Point2(r->pos.x, i), Tiles::tileWall);
                level->setTile(Point2(r->pos.x+r->size.x, i), Tiles::tileWall);
                grid[r->pos.x][i] = 'x';
                grid[r->pos.x+r->size.x][i] = 'x';
            }
            for (int i = r->pos.x + 1; i <= r->pos.x+r->size.x-1; i++) {
                for (int j = r->pos.y + 1; j <= r->pos.y+r->size.y-1; j++) {
                    level->setTile(Point2(i, j), Tiles::tileFloor);
                    grid[i][j] = 'x';
                }
            }
        }

        fillMaze(level, size, grid);

        for (Room* r : *rooms) {
            do{
                Point2 p;
                Point2 p2;
                bool r2 = rand() % 2 == 0;

                if(rand() % 2 == 0){
                    p.x = r->pos.x + (r2?r->size.x:0);
                    p2.x = r->pos.x + (r2?r->size.x+1:-1);

                    //p.y = r->pos.y + (rand() % (r->size.y-1))+1;
                    p.y = r->pos.y + (r->size.y/2);
                    p2.y = p.y;
                }else{
                    //p.x = r->pos.x + (rand() % (r->size.x-1))+1;
                    p.x = r->pos.x + (r->size.x/2);
                    p2.x = p.x;

                    p.y = r->pos.y + (r2?r->size.y:0);
                    p2.y = r->pos.y + (r2?r->size.y+1:-1);
                }

                grid[p.x][p.y] = 'v';
                level->setTile(p, (rand()%100==0)?Tiles::tileSecretDoor:Tiles::tileDoor);

                if(level->inRange(p2)){
                    grid[p2.x][p2.y] = 'v';
                }

            }while(rand() % 5 == 0);
        }

        for (int i = 0; i < size.x; i++) {
            for (int j = 0; j < size.y; j++) {
                if(level->indexAt(Point2(i, j)) == Tiles::tileUnset->getIndex()){
                    if(grid[i][j] == 'v'){
                        level->setTile(Point2(i, j), Tiles::tileFloor);
                    }else if(grid[i][j] == 'u'){
                        level->setTile(Point2(i, j), Tiles::tileWall);
                    }
                }
            }
        }

        bool changed = true;

        do{
            changed = false;
            for (int i = 0; i < level->getSize().x; i++) {
                for (int j = 0; j < level->getSize().y; j++) {
                    if (level->indexAt(Point2(i, j)) == Tiles::tileFloor->getIndex()) {
                        int paths = 0;
                        if (level->tileAt(Point2(i + 1, j))->hasAllOfFlags(tileFlagPathable)) {
                            paths++;
                        }
                        if (level->tileAt(Point2(i - 1, j))->hasAllOfFlags(tileFlagPathable)) {
                            paths++;
                        }
                        if (level->tileAt(Point2(i, j + 1))->hasAllOfFlags(tileFlagPathable)) {
                            paths++;
                        }
                        if (level->tileAt(Point2(i, j - 1))->hasAllOfFlags(tileFlagPathable)) {
                            paths++;
                        }
                        if (paths == 1) {
                            level->setTile(Point2(i, j), Tiles::tileWall);
                            changed = true;
                        }
                    }
                }
            }
        }while(changed);


        for (int i = 0; i < level->getSize().x; i++) {
            for (int j = 0; j < level->getSize().y; j++) {
                if (level->indexAt(Point2(i, j)) == Tiles::tileUnset->getIndex()) {
                    level->setTile(Point2(i, j), Tiles::tileWall);
                }
                if(i == 0 || j == 0 || i == level->getSize().x-1 || j == level->getSize().y-1){
                    level->setTile(Point2(i, j), Tiles::tileEdge);
                }
            }
        }


        fillRooms(level, rooms);


    }

    void fillRooms(Level* level, vector<Room*>* rooms){
        for (Room* r : *rooms) {

            bool addedWalls = false;

            if(r->size.x > 6 && r->size.y > 6 && rand()%3 > 0){
                int l = (rand()%2)+2;
                Utility::executeBorder(r->pos+l, r->pos+r->size-l, [level](int x, int y){
                    if((x+y)%2 == 0){
                        if(rand()%3 > 0){
                            level->setTile(Point2(x, y), Tiles::tilePillar);
                        }
                    }
                });
            }


            //Maze
            if (!addedWalls) {
                if (rand() % 5 == 0 && r->size.xPlusY() > 24) {
                    generateMaze(level, r);
                    level->setTile(r->pos+(r->size/2), Tiles::tileChest);
                    addedWalls = true;
                }
            }

            //Ring shaped room
            if (!addedWalls) {
                if (rand() % 3 == 0) {

                    Point2 min = (Point2(rand(), rand()) % ((r->size/2)-2))+2;
                    Point2 max = r->size-min+1;

                    int w = (rand() % 3 == 0) ? Tiles::tilePonyWall->getIndex() : Tiles::tileWall->getIndex();

                    for (int i = min.x; i < max.x; i++) {
                        for (int j = min.y; j < max.y; j++) {
                            level->setTile(r->pos + Point2(i, j), w);
                        }
                    }
                    addedWalls = true;


                }
            }

            //Chest Room or Shop
            if(!addedWalls){
                int doors = 0;
                Point2 lastDoorLocation = Point2Neg1;
                Utility::executeBorder(r->pos, r->pos+r->size, [&r, &level, &doors, &lastDoorLocation](int x, int y){
                    if(level->tileAt(x, y)->hasAllOfFlags(tileFlagDoor)){
                        doors++;
                        lastDoorLocation = Point2(x, y);
                    }
                });

                if(doors == 1){
                    //Chest
                    if (rand() % 2 == 0) {
                        level->setTile(r->pos + (r->size/2), Tiles::tileChest);
                        level->setTile(lastDoorLocation, rand()%2==0?Tiles::tileSecretDoor:Tiles::tileLockedDoor);
                    }else{
                        int c = 0;
                        Utility::executeBorder(r->pos + (r->size/2)-1, r->pos + (r->size/2)+1, [&level, &c](int x, int y){
                            if(level->tileAt(x, y)->hasAllOfFlags(tileFlagPathable)){
                                c++;
                            }
                        });
                        if(c == 8){
                            EntityShop* e = new EntityShop("Merchant", aiNone, 'M', r->pos + (r->size/2), C_LIGHT_MAGENTA, 100);
                            e->addItems(ItemGenerator::makeLoot(ItemGenerator::lootProfileShop, level->getDifficulty(), (rand()%9000)+1000, 10, 20, 2));
                            e->addItem(ItemGenerator::makeCoins(1000));
                            level->newEntity(e);
                        }
                    }
                }
                addedWalls = true;
            }


            //Crates
            if (rand() % 2 == 0) {

                for (int pass = 0; pass < 3; pass++) {
                    for (int i = r->pos.x; i < r->pos.x+r->size.x; i++) {
                        for (int j = r->pos.y; j < r->pos.y+r->size.y; j++) {
                            Point2 p = Point2(i, j);
                            if (level->indexAt(p) == Tiles::tileFloor->getIndex()) {
                                int doors = level->countTilesAround(p, Tiles::tileDoor);
                                int walls = level->countTilesAround(p, Tiles::tileWall) + level->countTilesAround(r->pos + Point2(i, j), Tiles::tilePonyWall);
                                int crates = level->countTilesAround(p, Tiles::tileCrate);
                                switch (pass) {
                                    case 0:
                                    {
                                        if (doors == 0 && walls > 0 && rand() % 10 == 0) {
                                            level->setTile(p, Tiles::tileCrate);
                                        }
                                        break;
                                    }
                                    case 1:
                                    {
                                        if (doors == 0 && crates == 1 && walls > 0 && rand() % 2 == 0) {
                                            level->setTile(p, Tiles::tileTemp);
                                        }
                                        break;
                                    }
                                    case 2:
                                    {
                                        if (doors == 0 && crates >= 2 && rand() % 2 == 0) {
                                            level->setTile(p, Tiles::tileTemp);
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    for (int i = r->pos.x; i < r->pos.x+r->size.x; i++) {
                        for (int j = r->pos.y; j < r->pos.y+r->size.y; j++) {
                            if (level->indexAt(i, j) == Tiles::tileTemp->getIndex()) {
                                level->setTile(i, j, Tiles::tileCrate);
                            }
                        }
                    }
                }
            }
        }


    }

    void generateMaze(Level* level, Room* room) {

        Point2 size = room->size;

        vector<vector<char>> grid = vector<vector<char>>(size.x, vector<char>(size.y));

        for (int i = 0; i < (int)grid.size(); i++) {
            for (int j = 0; j < (int)grid[0].size(); j++) {
                grid[i][j] = 'u';
            }
        }

        fillMaze(level, size, grid);

        for (int i = 0; i < size.x; i++) {
            for (int j = 0; j < size.y; j++) {
                Point2 p = Point2(i, j) + room->pos;
                level->setTile(p, grid[i][j] == 'v' ? Tiles::tileFloor->getIndex() : Tiles::tileWall->getIndex());
                if (level->tileAt(p - Point2(-1, 0))->hasAllOfFlags(tileFlagDoor) ||
                        level->tileAt(p - Point2(1, 0))->hasAllOfFlags(tileFlagDoor) ||
                        level->tileAt(p - Point2(0, -1))->hasAllOfFlags(tileFlagDoor) ||
                        level->tileAt(p - Point2(0, 1))->hasAllOfFlags(tileFlagDoor)) {
                    level->setTile(p, Tiles::tileFloor);
                }
            }
        }


    }

    void fillMaze(Level* level, Point2 size, vector<vector<char>>& grid){
        //u == unexplored
        //v == path
        //x == not usable


        Point2 activePoint = size/2;

        grid[activePoint.x][activePoint.y] = 'v';

        vector<Point2> backPath;

        while (true) {
            vector<Point2> possibilities;

            Point2 temp;
            temp = (activePoint + Point2(-2, 0));
            if (temp.inRange(0, 0, size.x - 1, size.y - 1) && grid[temp.x][temp.y] == 'u') {
                possibilities.push_back(temp);
            }
            temp = (activePoint + Point2(2, 0));
            if (temp.inRange(0, 0, size.x - 1, size.y - 1) && grid[temp.x][temp.y] == 'u') {
                possibilities.push_back(temp);
            }
            temp = (activePoint + Point2(0, -2));
            if (temp.inRange(0, 0, size.x - 1, size.y - 1) && grid[temp.x][temp.y] == 'u') {
                possibilities.push_back(temp);
            }
            temp = (activePoint + Point2(0, 2));
            if (temp.inRange(0, 0, size.x - 1, size.y - 1) && grid[temp.x][temp.y] == 'u') {
                possibilities.push_back(temp);
            }

            Point2 chosen;

            if (possibilities.size() == 0) {
                if (backPath.size() == 0) {
                    break;
                }
                activePoint = backPath.back();
                backPath.pop_back();
                continue;
            } else {
                chosen = possibilities[rand() % possibilities.size()] - activePoint;
                backPath.push_back(activePoint);
            }

            Point2 chosenWall = (chosen / 2) + activePoint;
            Point2 chosenNext = chosen + activePoint;

            if (grid[chosenNext.x][chosenNext.y] == 'u') {
                grid[chosenWall.x][chosenWall.y] = 'v';
                grid[chosenNext.x][chosenNext.y] = 'v';
            }

            activePoint = chosenNext;
        }
    }


}
