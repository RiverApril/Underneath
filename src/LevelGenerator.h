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

    extern const int OPTION_ROOM_RADIUS_MAX;
    extern const int OPTION_ROOM_RADIUS_MIN;
    extern const int OPTION_ROOM_ENTRANCES_MAX;
    extern const int OPTION_ROOM_ENTRANCES_MIN;

    const int left = 0;
    const int right = 1;
    const int up = 2;
    const int down = 3;

    struct Entry{
        int direction = 0;
        int offset = 0;
        int x = 0;
        int y = 0;
    };

    struct Room{
        Room(){
            entrances = new std::vector<Entry*>();
        }
        ~Room(){
            Utility::deleteContentsOfVector(entrances);
            delete entrances;
        }
        Point2 center;
        Point2 radius;
        std::vector<Entry*>* entrances;
    };

    extern bool roomsOverlap(Room* a, Room* b);

    extern Room* createRoom(Point2 roomSize, std::vector<Room*>* presentRooms);

    extern std::vector<Room*>* createRooms(int qty, Point2 roomSize);

    extern void makeRoomsAndPaths(std::vector<Room*>* rooms, Level* level);
    
}

#endif /* defined(__Underneath__LevelGenerator__) */
