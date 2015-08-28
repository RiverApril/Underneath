//
//  Animator.hpp
//  Underneath
//
//  Created by Braeden Atlee on 8/26/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Animator__
#define __Underneath__Animator__

#include "Ui.hpp"
#include "Level.hpp"

namespace Animator {
    void renderRangedAttack(Point2 from, Point2 to, Icon* icon, Level* level, int length);
    void renderExposion(Point2 center, double radius, Icon* icon, Level* level, int length);
}


#endif /* defined(__Underneath__Animator__) */
