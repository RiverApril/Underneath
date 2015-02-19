//
//  Random.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/18/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Global.h"
#include "Random.h"

namespace Random {

    double randDouble(double min, double max){
        return min + ((rand() / (double)RAND_MAX) * (max-min));
    }

}
