//
//  LevelGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "LevelGenerator.h"


namespace LevelGenerator{

    const int OPTION_ROOM_RADIUS_MAX = 10;
    const int OPTION_ROOM_RADIUS_MIN = 2;

    const int OPTION_ROOM_ENTRANCES_MAX = 8;
    const int OPTION_ROOM_ENTRANCES_MIN = 2;

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
            r->center.x = (rand()%(levelWidth-(r->radius.x*2)))+r->radius.x;
            r->center.y = (rand()%(levelHeight-(r->radius.y*2)))+r->radius.y;
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
                //level->setTile(e->x, e->y, tileDebug1);
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
                                        if(level->indexAt(e->x+(xoLarge?m:-m), e->y)==tileWall->getIndex()){
                                            blocked = true;
                                            break;
                                        }
                                    }
                                    for(int m=0;m<ydif;m++){
                                        if(level->indexAt(e->x+(xoLarge?xdif:-xdif), e->y+(yoLarge?m:-m))==tileWall->getIndex()){
                                            blocked = true;
                                            break;
                                        }
                                    }
                                    if(!blocked){
                                        for(int m=1;m<xdif;m++){
                                            level->setTile(e->x+(xoLarge?m:-m), e->y, tileDebug2);
                                        }
                                        for(int m=0;m<ydif;m++){
                                            level->setTile(e->x+(xoLarge?xdif:-xdif), e->y+(yoLarge?m:-m), tileDebug2);
                                        }
                                        level->setTile(e->x, e->y, tileDebug3);
                                        level->setTile(eo->x, eo->y, tileDebug4);
                                        level->setTile(e->x+(xoLarge?xdif:-xdif), e->y+(yoLarge?ydif:-ydif), tileDebug3);
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