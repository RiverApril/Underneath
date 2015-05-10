//
//  Random.h
//  Underneath
//
//  Created by Braeden Atlee on 2/18/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Random__
#define __Underneath__Random__

struct Point2;

namespace Random {

    double randDouble(double min, double max);

    int randomFromPoint(Point2 seed);


}

#endif /* defined(__Underneath__Random__) */
