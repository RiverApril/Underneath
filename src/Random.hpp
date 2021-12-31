//
//  Random.hpp
//  Underneath
//
//  Created by Emily Atlee on 2/18/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__Random__
#define __Underneath__Random__

#define PREDEFINED_RANDOM_GRID_SIZE 64

struct Point2;

namespace Random {

    extern unsigned char predefinedRandomGrid[PREDEFINED_RANDOM_GRID_SIZE][PREDEFINED_RANDOM_GRID_SIZE];

    double randDouble(double min, double max);

    int randomFromPoint(Point2 seed);

    void setup();

    template <typename T>
    const T choose(int count, ...) {
        vector<T> options;
        va_list args;
        va_start(args, count);
        for(int i=0;i<count;i++){
            T a = va_arg(args, T);
            options.push_back(a);
        }
        va_end(args);
        int index = rand() % count;
        return options[index];
    }


}

#endif /* defined(__Underneath__Random__) */
