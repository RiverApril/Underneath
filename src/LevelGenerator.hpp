//
//  LevelGenerator.h
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__LevelGenerator__
#define __Underneath__LevelGenerator__

#include "Global.hpp"
#include "Tile.hpp"
#include "Geometry.hpp"
#include "Level.hpp"
#include "Utility.hpp"

namespace LevelGenerator {

    const int left = 0;
    const int right = 1;
    const int up = 2;
    const int down = 3;

    struct Room {

        Room() {}
        Point2 pos;
        Point2 size;
    };


    //int getPathAndMaybeDoor();

    extern bool roomsOverlap(Room* a, Room* b, int border);

    extern Room* createRoom(Point2 roomSize, vector<Room*>* presentRooms, Point2 pos = Point2Neg1);

    extern vector<Room*>* createRooms(Point2 startPos, int maxQty, Point2 roomSize);

    extern void makeRoomsAndPaths(vector<Room*>* rooms, Level* level);

    extern void fillRooms(Level* level, vector<Room*>* rooms);

    extern void generateMaze(Level* level, Room* room);

    extern void fillMaze(Level* level, Point2 size, vector<vector<char>>& grid);

}

#endif /* defined(__Underneath__LevelGenerator__) */
