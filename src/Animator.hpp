//
//  Animator.hpp
//  Underneath
//
//  Created by Emily Atlee on 8/26/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__Animator__
#define __Underneath__Animator__

#include "Ui.hpp"
#include "Level.hpp"

namespace Animator {
    void renderFlash(Point2 p, Level* level, vector<char> chars, int speed, Ui::Color fg, Ui::Color bg);
    void renderRangedAttack(Point2 from, Point2 to, Icon* icon, Level* level, int length);
    void renderExposion(Point2 center, double radius, Level* level, int length, DamageType damType = damExplosion);
}


#endif /* defined(__Underneath__Animator__) */
