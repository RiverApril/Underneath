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

        if(line.size() <= 2){
            return;
        }

        int j = 1-length;

        timeout(10);

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

            for(int i=0;i<5;i++){
                getch();
            }
            refresh();
        }

        timeout(defaultTimeout);


    }

    void renderExposion(Point2 center, double radius, Level* level, int length){

        double p = 8;

        RandomIcon icon = RandomIcon({'~', '*', '&'}, C_LIGHT_YELLOW, C_BLACK);

        timeout(10);

        for(int r=0;r<radius;r++){
            double ma = ((r+1)*p);
            double maDivTau = ma/Math::tau;

            Ui::setColor(r<(radius/3)?C_LIGHT_RED:C_LIGHT_YELLOW, icon.Icon::getBgColor(Ui::tick, Point2Zero, level));

            for(double a = 0; a<ma;a+=maDivTau){
                Point2 pos = Point2(sin(maDivTau*a)*r, cos(maDivTau*a)*r);
                pos += center-level->menuGame->viewPos;

                mvaddch(pos.y, pos.x, icon.getChar(Ui::tick, pos, level));
            }

            for(int i=0;i<5;i++){
                getch();
            }
            refresh();
        }

        timeout(defaultTimeout);

    }

}
