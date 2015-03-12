//
//  Random.h
//  Underneath
//
//  Created by Braeden Atlee on 2/18/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Random__
#define __Underneath__Random__

namespace Random{

    double randDouble(double min, double max);

    template<typename T>
    T choose(size_t count, ...){

        size_t chosen = rand()%count;
        size_t index = 0;

        va_list args;
        va_start(args, count);

        if(index == chosen){
            return va_arg(args, T);
        }
        index++;
        
        va_end(args);

        return 0;
    }


}

#endif /* defined(__Underneath__Random__) */
