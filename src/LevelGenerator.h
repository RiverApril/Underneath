//
//  LevelGenerator.h
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__LevelGenerator__
#define __Underneath__LevelGenerator__

#include "Global.h"
#include "Tile.h"
#include "Geometry.h"
#include "Level.h"
#include "Utility.h"

namespace LevelGenerator{

    const int left = 0;
    const int right = 1;
    const int up = 2;
    const int down = 3;

    struct Entry{
        int direction = 0;
        int x = 0;
        int y = 0;
    };

    struct Room{
        Room(){
            entrances = new vector<Entry*>();
        }
        ~Room(){
            for(int i=0;i<entrances->size();i++){
                delete entrances->at(i);
            }
            delete entrances;
        }
        Point2 center;
        Point2 radius;
        vector<Entry*>* entrances;
    };

    int getPathAndMaybeDoor();

    extern bool roomsOverlap(Room* a, Room* b, int border);

    extern Room* createRoom(Point2 roomSize, vector<Room*>* presentRooms);

    extern vector<Room*>* createRooms(int qty, Point2 roomSize);

    extern void makeRoomsAndPaths(vector<Room*>* rooms, Level* level);

}

#endif /* defined(__Underneath__LevelGenerator__) */