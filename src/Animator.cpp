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

        if(!level->currentWorld->menuGame){
            return;
        }

        vector<Point2> line = Utility::plotLine(from, to);

        if(line.size() <= 2){
            return;
        }

        int j = 1-length;

        timeout(fastTimeout);

        for(int i=1; i < (int)line.size() || j < (int)line.size(); i++){

            /*if(level->tileAt(line[i])->hasFlag(tileFlagTall)){
                Utility::execute4Around(line[i].x, line[i].y, [&level, &line, &i](int x, int y){
                    Point2 p = Point2(x, y);
                    if(!level->tileAt(x, y)->hasFlag(tileFlagTall) && distanceSquared(line[i], p) <= 1){
                        line[i] = p;
                        return;
                    }
                });
            }*/


            if(i < (int)line.size()){
                if(line[i] > level->currentWorld->menuGame->viewPos && line[i] < level->currentWorld->menuGame->viewPos+level->currentWorld->menuGame->gameArea){
                    Point2 pos = line[i] - level->currentWorld->menuGame->viewPos;

                    Ui::setColor(icon->getFgColor(Ui::tick, pos, level), icon->getBgColor(Ui::tick, pos, level));
                    mvaddch(pos.y, pos.x, icon->getChar(Ui::tick, pos, level));
                }

                //level->setTile(line[i], Tiles::tileDebug2);
            }

            if(j >= 0){
                if(line[j] > level->currentWorld->menuGame->viewPos && line[j] < level->currentWorld->menuGame->viewPos+level->currentWorld->menuGame->gameArea){
                	Point2 pos = line[j] - level->currentWorld->menuGame->viewPos;
                	move(pos.y, pos.x);
                	level->currentWorld->menuGame->drawTileAt(line[j]);
                }
            }

            j++;

            for(int i=0;i<5;i++){
                getchSafe();
            }
            refresh();
        }

        timeout(defaultTimeout);


    }

    void renderExposion(Point2 center, double radius, Level* level, int length){

        double p = 8;

        RandomIcon icon = RandomIcon({'~', '*', '&'}, 0/*Irrelevant*/, C_BLACK);

        timeout(fastTimeout);

        for(int r=0;r<radius;r++){
            double ma = ((r+1)*p);
            double maDivTau = ma/TAU;

            Ui::setColor(r<(radius/3)?C_LIGHT_RED:C_LIGHT_YELLOW, icon.getBgColor(Ui::tick, Point2Zero, level));

            for(double a = 0; a<ma;a+=maDivTau){
                Point2 pos = Point2(sin(maDivTau*a)*r, cos(maDivTau*a)*r);
                pos += center-level->currentWorld->menuGame->viewPos;

                mvaddch(pos.y, pos.x, icon.getChar(Ui::tick, pos, level));
            }

            for(int i=0;i<5;i++){
                getchSafe();
            }
            refresh();
        }

        timeout(defaultTimeout);

    }

}
