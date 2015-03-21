//
//  LevelGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "LevelGenerator.h"
#include "EnemyGenerator.h"


Point2 Level::generate(unsigned int seed, Point2 stairUpPos, string previousLevel) {

    genDebugPos = 1;


    srand(seed);

    int attemt = 0;

    bool pathNotFound = false;

    Point2 stairDownPos;

    do{

        do{

            for (int i=0; i<size.x; i++) {
                for (int j=0; j<size.y; j++) {
                    tileGrid[i][j].index = (int8_t)Tiles::tileUnset->getIndex();
                    tileGrid[i][j].explored = false;
                    //tileGrid[i][j].entity = nullptr;
                    if(i==0 || j==0 || i==(size.x-1) || j==(size.y-1)){
                        tileGrid[i][j].index = (int8_t)Tiles::tileWall->getIndex();
                    }
                }
            }
            genDebug("generating...  attemt #"+to_string(attemt));
            attemt++;


            vector<LevelGenerator::Room*>* rooms = LevelGenerator::createRooms(stairUpPos, 1000, size);
            LevelGenerator::makeRoomsAndPaths(rooms, this);

            genDebug("generated");

            for (size_t i = 0; i<rooms->size(); i++){
                delete rooms->at(i);
            }
            delete rooms;


        }while(tileAt(stairUpPos)->getIndex() != Tiles::tileFloor->getIndex());


        pathNotFound = false;

        int dist = (size.xPlusY()) / 2;
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
                if(dist < ((size.xPlusY() / 2) / 10)){
                    pathNotFound = true;
                    break;
                }
            }
        }

    }while(pathNotFound);

    genDebug("found path");

    genDebug("Filling chests...");
    for (int i=0; i<size.x; i++) {
        for (int j=0; j<size.y; j++) {
            if(tileGrid[i][j].index == (int8_t)Tiles::tileChest->getIndex()){
                TEChest* te = new TEChest(Point2(i, j));
                te->addItems(ItemGenerator::createRandLoots(difficulty, difficulty * 100, 5, 1));
                tileEntityList.push_back(te);
            }else if(tileGrid[i][j].index == (int8_t)Tiles::tileCrate->getIndex()){
                if(rand()%6 == 0){
                	TEChest* te = new TEChest(Point2(i, j));
                    te->addItem(ItemGenerator::makeCoins((rand()%30)+1));
                    tileEntityList.push_back(te);
                }else if(rand()%20 == 0){
                    TEChest* te = new TEChest(Point2(i, j));
                    te->addItems(ItemGenerator::createRandLoots(difficulty, 0, 0, 1));
                    tileEntityList.push_back(te);
                }
            }
        }
    }

    setTile(stairUpPos, Tiles::tileStairUp);
    setTile(stairDownPos, Tiles::tileStairDown);
    if(previousLevel.size() > 0){
        tileEntityList.push_back(new TEStair(stairUpPos, true, previousLevel));
    }
    tileEntityList.push_back(new TEStair(stairDownPos, false, "Floor"+to_string(Utility::parseInt(name.substr(5))+1)));

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

    EnemyGenerator::setIntervals(difficulty);

    int count = (rand()%(200))+100;
    for(int i=0;i<count;i++){
        placeNewAiEntity(EnemyGenerator::makeRandomEntity(difficulty), stairUpPos);
    }
    
    genDebug("done");
    
    
    return stairUpPos;
    
    
}


namespace LevelGenerator{

    bool roomsOverlap(Room* a, Room* b, int border){
        return !(((a->center.x-a->radius.x-border) > (b->center.x+b->radius.x+border) ||
                  (b->center.x-b->radius.x-border) > (a->center.x+a->radius.x+border)) ||
                 ((a->center.y-a->radius.y-border) > (b->center.y+b->radius.y+border) ||
                  (b->center.y-b->radius.y-border) > (a->center.y+a->radius.y+border)));
    }

    Room* createRoom(Point2 roomSize, vector<Room*>* presentRooms, Point2 pos){
        bool fit = false;
        Room* r = new Room();
        int att = 0;
        while(!fit && att<100){
            att++;
            r->radius.x = (rand()%6)+3;
            r->radius.y = (rand()%6)+3;
            if(pos == Point2Neg1){
                r->center.x = (rand()%(roomSize.x-(r->radius.x*2)))+r->radius.x;
                r->center.y = (rand()%(roomSize.y-(r->radius.y*2)))+r->radius.y;
            }else{
                r->center.x = pos.x;
                r->center.y = pos.y;
            }
            fit = true;
			for (size_t i = 0; i<presentRooms->size(); i++){
                if(roomsOverlap(presentRooms->at(i), r, 2)){
                    fit = false;
                    break;
                }
            }
        }
        if(!fit){
            r->radius.set(0);
        }else{
            int ec = (rand()%4)+4;
            bool eL = false, eR = false, eU = false, eD = false;
            for(int i=0;i<ec || (!(eL && eR && eU && eD));i++){
                Entry* e = new Entry();
                e->direction = rand()%4;
                if(e->direction == left) eL = true;
                if(e->direction == right) eR = true;
                if(e->direction == up) eU = true;
                if(e->direction == down) eD = true;
                int rr = (e->direction==left||e->direction==right?r->radius.y:r->radius.x)-1;
                int offset = ((rand()%(rr*2-1))-rr);
                e->x = (e->direction==left?-r->radius.x:(e->direction==right?r->radius.x:offset))+r->center.x;
                e->y = (e->direction==up?-r->radius.y:(e->direction==down?r->radius.y:offset))+r->center.y;
                /*bool tooClose = false;
                for(int j=0;j<r->entrances->size();j++){
                    Entry* ee = r->entrances->at(j);
                    if(ee->direction == e->direction){
                        if(abs(ee->offset-e->offset)<=2){
                            tooClose = true;
                            break;
                        }
                    }
                }
                if(!tooClose){*/
                r->entrances->push_back(e);
                //}
            }
        }
        return r;
    }

    vector<Room*>* createRooms(Point2 startPos, int maxQty, Point2 roomSize){
        vector<Room*>* rooms = new vector<Room*>();
        Room* centerRoom = createRoom(roomSize, rooms, startPos);
        rooms->push_back(centerRoom);
        for(int i=1;i<maxQty;i++){
            Room* r = createRoom(roomSize, rooms);
            if(r->radius.x > 0 && r->radius.y > 0){
                rooms->push_back(r);
            }
        }
        return rooms;
    }

    int getPathAndMaybeDoor(){
        return (rand()%30==0?(rand()%10==0?Tiles::tileSecretDoor:Tiles::tileDoor):Tiles::tilePath)->getIndex();
    }

    void makeRoomsAndPaths(vector<Room*>* rooms, Level* level){
		for (size_t i = 0; i<rooms->size(); i++){
            Room* r = rooms->at(i);
            for(int j=-r->radius.x;j<=r->radius.x;j++){
                level->setTile(Point2(r->center.x+j, r->center.y+r->radius.y), Tiles::tileWall);
                level->setTile(Point2(r->center.x+j, r->center.y-r->radius.y), Tiles::tileWall);
            }
            for(int j=-r->radius.y;j<=r->radius.y;j++){
                level->setTile(Point2(r->center.x+r->radius.x, r->center.y+j), Tiles::tileWall);
                level->setTile(Point2(r->center.x-r->radius.x, r->center.y+j), Tiles::tileWall);
            }
            for(int j=-r->radius.x+1;j<=r->radius.x-1;j++){
                for(int k=-r->radius.y+1;k<=r->radius.y-1;k++){
                    level->setTile(Point2(r->center.x+j, r->center.y+k), Tiles::tileFloor);
                }
            }
        }
		for (size_t i = 0; i<rooms->size(); i++){
            Room* r = rooms->at(i);

			for (size_t j = 0; j<r->entrances->size(); j++){
                Entry* e = r->entrances->at(j);

				for (size_t k = 0; k<rooms->size(); k++){
                    Room* ro = rooms->at(k);

                    if(r->center==ro->center){
                        continue;
                    }

					for (size_t l = 0; l<ro->entrances->size(); l++){
                        Entry* eo = ro->entrances->at(l);

                        if((e->direction!=eo->direction)){
                            int ddif = 1;
                            if((e->direction==left && eo->direction==right)||
                               (e->direction==right && eo->direction==left)||
                               (e->direction==up && eo->direction==down)||
                               (e->direction==down && eo->direction==up)){
                                ddif = 2;
                            }
                            if(eo->x == e->x){
                                continue;
                            }
                            int xdif = abs(eo->x - e->x);
                            int ydif = abs(eo->y - e->y);
                            bool blocked = false;
                            if(ddif == 1){
                                bool xoLarge = eo->x>e->x;
                                bool yoLarge = eo->y>e->y;
                                /*if((e->direction == right && xoLarge &&
                                 ((eo->direction == up && yoLarge) ||
                                 (eo->direction == down && !yoLarge))) ||
                                 (e->direction == left && !xoLarge &&
                                 ((eo->direction == up && yoLarge) ||
                                 (eo->direction == down && !yoLarge)))){*/
                                if(((e->direction == right && xoLarge) || (e->direction == left && !xoLarge)) &&
                                   ((eo->direction == up && yoLarge) || (eo->direction == down && !yoLarge))){
                                    for(int m=1;m<xdif;m++){
                                        if(level->indexAt(Point2(e->x+(xoLarge?m:-m), e->y))!=Tiles::tileUnset->getIndex()){
                                            blocked = true;
                                            break;
                                        }
                                    }
                                    for(int m=0;m<ydif;m++){
                                        if(level->indexAt(Point2(e->x+(xoLarge?xdif:-xdif), e->y+(yoLarge?m:-m)))!=Tiles::tileUnset->getIndex()){
                                            blocked = true;
                                            break;
                                        }
                                    }
                                    if(!blocked){
                                        for(int m=1;m<xdif;m++){
                                            level->setTile(Point2(e->x+(xoLarge?m:-m), e->y), getPathAndMaybeDoor());

                                            if(rand()%10==0){
                                                int a = rand()%10;
                                                int aa = (rand()%3)-1;
                                                for(int n=1;n<a;n++){
                                                    if(level->indexAt(Point2(e->x+(xoLarge?m:-m), e->y+(n*aa)))==Tiles::tileUnset->getIndex()){
                                                        level->setTile(Point2(e->x+(xoLarge?m:-m), e->y+(n*aa)), getPathAndMaybeDoor());
                                                    }else{
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        for(int m=0;m<ydif;m++){
                                            level->setTile(Point2(e->x+(xoLarge?xdif:-xdif), e->y+(yoLarge?m:-m)), getPathAndMaybeDoor());

                                            if(rand()%10==0){
                                                int a = rand()%10;
                                                int aa = (rand()%3)-1;
                                                for(int n=1;n<a;n++){
                                                    if(level->indexAt(Point2(e->x+(xoLarge?xdif:-xdif)+(n*aa), e->y+(yoLarge?m:-m)))==Tiles::tileUnset->getIndex()){
														level->setTile(Point2(e->x+(xoLarge?xdif:-xdif)+(n*aa), e->y+(yoLarge?m:-m)), getPathAndMaybeDoor());
                                                    }else{
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        level->setTile(Point2(e->x, e->y), (rand()%20)==0?Tiles::tileSecretDoor:Tiles::tileDoor);
                                        level->setTile(Point2(eo->x, eo->y), (rand()%20)==0?Tiles::tileSecretDoor:Tiles::tileDoor);
                                    }
                                }
                            }
                        }

                    }
                }
            }
        }

        for(int i=0;i<level->getSize().x;i++){
            for(int j=0;j<level->getSize().y;j++){
                Tile* a = level->tileAt(Point2(i, j));
                if(a->hasFlag(tileFlagPathable)){
                    for(int k=-1;k<=1;k++){
                        for(int l=-1;l<=1;l++){
                            if(level->indexAt(Point2(i+k, j+l)) == Tiles::tileUnset->getIndex()){
                                level->setTile(Point2(i+k, j+l), Tiles::tileWall);
                            }
                        }
                    }
                }
            }
        }
        for(int i=0;i<level->getSize().x;i++){
            for(int j=0;j<level->getSize().y;j++){
                Tile* a = level->tileAt(Point2(i, j));
                if(a->getIndex() == Tiles::tilePath->getIndex()){
                    int walls = 0;
                    for(int k=-1;k<=1;k++){
                        for(int l=-1;l<=1;l++){
                            if(!(k == 0 && l == 0)){
                                if(level->indexAt(Point2(i+k, j+l)) == Tiles::tileWall->getIndex()){
                                    walls++;
                                }
                            }
                        }
                    }
                    if(walls == 7 && rand()%3==0){
                        level->setTile(Point2(i, j), Tiles::tileChest);
                    }
                }
            }
        }

        for(size_t i = 0; i<rooms->size(); i++){
            Room* r = rooms->at(i);

            Point2 inner = r->radius-1;

            if(rand()%3 == 0){

                for(int j = -inner.x; j <= inner.x; j++){
                    for(int k = -inner.y; k <= inner.y; k++){
                        int doors = level->countTilesAround(r->center + Point2(j, k), Tiles::tileDoor);
                        if(doors == 0 && rand()%10==0){
                            level->setTile(r->center + Point2(j, k), Tiles::tileCrate);
                        }
                    }
                }
            }

            if(rand()%3 == 0){

                Point2 inner = Point2(r->radius.x - ((rand()%r->radius.x) + 2), r->radius.y - ((rand()%r->radius.y) + 2));

                int w = (((rand()%3==0)) && (inner.x==0 || inner.y==1))?Tiles::tilePonyWall->getIndex():Tiles::tileWall->getIndex();

                if(inner.x >= 0 && inner.y >= 0){
                    for(int j = -inner.x; j <= inner.x; j++){
                        for(int k = -inner.y; k <= inner.y; k++){
                            level->setTile(r->center + Point2(j, k), w);
                        }
                    }
                }


            }
        }

        
        
    }
}

