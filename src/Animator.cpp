//
//  Animator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 8/26/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Animator.hpp"

namespace Animator {

    void renderRangedAttack(Point2 from, Point2 to, Ui::Color fg, Ui::Color bg, char character, Level* level, int length){

        vector<Point2> line = Utility::plotLine(from, to);

        int j = -length;

        for(int i=0;i<line.size()&&j<line.size();i++){

            if(i < line.size()){
            	Point2 pos = level->menuGame->viewPos-line[i];

            	Ui::setColor(fg, bg);
            	mvaddch(pos.x, pos.y, character);
            }

            if(j >= 0){
                Point2 pos = level->menuGame->viewPos-line[j];
                move(pos.x, pos.y);
                level->menuGame->drawTileAt(line[j]);
            }

            usleep(100);
            refresh();

            j++;
        }


    }

}
