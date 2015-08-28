//
//  Animator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 8/26/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Animator.hpp"
#include "Utility.hpp"

namespace Animator {

    void renderRangedAttack(Point2 from, Point2 to, Icon* icon, Level* level, int length){

        if(!level->menuGame){
            return;
        }

        vector<Point2> line = Utility::plotLine(from, to);

        int j = 1-length;

        for(int i=1; i < line.size() || j < line.size(); i++){


            if(i < line.size()){
            	Point2 pos = line[i] - level->menuGame->viewPos;

                Ui::setColor(icon->getFgColor(Ui::tick, pos, level), icon->getBgColor(Ui::tick, pos, level));
            	mvaddch(pos.y, pos.x, icon->getChar(Ui::tick, pos, level));

                //level->setTile(line[i], Tiles::tileDebug2);
            }

            if(j >= 0){
                Point2 pos = line[j] - level->menuGame->viewPos;
                move(pos.y, pos.x);
                level->menuGame->drawTileAt(line[j]);
            }

            j++;

            usleep(1000*50);
            refresh();
        }


    }

    void renderExposion(Point2 center, double radius, Icon* icon, Level* level, int length){

        double p = 3;

        for(int r=0;r<radius;r++){
            double ma = (r*p);
            double maDivTau = ma/Math::tau;
            for(double a = 0; a<ma;a+=maDivTau){
                Point2 pos = Point2(sin(maDivTau*a)*r, cos(maDivTau*a)*r);
                consolef("pos: %d, %d", pos.x, pos.y);
                pos += center-level->menuGame->viewPos;

                Ui::setColor(icon->getFgColor(Ui::tick, pos, level), icon->getBgColor(Ui::tick, pos, level));
                mvaddch(pos.y, pos.x, icon->getChar(Ui::tick, pos, level));
            }

            usleep(1000*50);
            refresh();
        }

    }

}
