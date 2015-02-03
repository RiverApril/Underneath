//
//  LevelGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "LevelGenerator.h"


namespace LevelGenerator{

    bool roomsOverlap(Room* a, Room* b, int border){
        return !(((a->center.x-a->radius.x-border) > (b->center.x+b->radius.x+border) ||
                  (b->center.x-b->radius.x-border) > (a->center.x+a->radius.x+border)) ||
                 ((a->center.y-a->radius.y-border) > (b->center.y+b->radius.y+border) ||
                  (b->center.y-b->radius.y-border) > (a->center.y+a->radius.y+border)));
    }

    Room* createRoom(Point2 roomSize, vector<Room*>* presentRooms){
        bool fit = false;
        Room* r = new Room();
        int att = 0;
        while(!fit && att<100){
            att++;
            r->radius.x = (rand()%6)+3;
            r->radius.y = (rand()%6)+3;
            r->center.x = (rand()%(roomSize.x-(r->radius.x*2)))+r->radius.x;
            r->center.y = (rand()%(roomSize.y-(r->radius.y*2)))+r->radius.y;
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

    vector<Room*>* createRooms(int qty, Point2 roomSize){
        vector<Room*>* rooms = new vector<Room*>();
        for(int i=0;i<qty;i++){
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
        for(int i=0;i<rooms->size();i++){
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
        for(int i=0;i<rooms->size();i++){
            Room* r = rooms->at(i);

            for(int j=0;j<r->entrances->size();j++){
                Entry* e = r->entrances->at(j);

                for(int k=0;k<rooms->size();k++){
                    Room* ro = rooms->at(k);

                    if(r->center==ro->center){
                        continue;
                    }

                    for(int l=0;l<ro->entrances->size();l++){
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
        
        
    }
}

