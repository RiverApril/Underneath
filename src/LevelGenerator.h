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
            entrances = shared_ptr<vector<shared_ptr<Entry>>>(new vector<shared_ptr<Entry>>());
        }
        ~Room(){
            
        }
        Point2 center;
        Point2 radius;
        shared_ptr<vector<shared_ptr<Entry>>> entrances;
    };

    extern bool roomsOverlap(shared_ptr<Room> a, shared_ptr<Room> b);

    extern shared_ptr<Room> createRoom(Point2 roomSize, shared_ptr<vector<shared_ptr<Room>>> presentRooms);

    extern shared_ptr<vector<shared_ptr<Room>>> createRooms(int qty, Point2 roomSize);

    extern void makeRoomsAndPaths(shared_ptr<vector<shared_ptr<Room>>> rooms, shared_ptr<Level> level);

}

#endif /* defined(__Underneath__LevelGenerator__) */
