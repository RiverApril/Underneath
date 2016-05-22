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

    void renderFlash(Point2 p, Level* level, vector<char> chars, int speed, Ui::Color fg, Ui::Color bg){
        Point2 pos = p - level->currentWorld->menuGame->viewPos;

        int maxTick = speed * (int)chars.size();
        timeout(fastTimeout);
        for(int tick = 0; tick < maxTick; tick++){
            Ui::setColor(fg, bg);
            mvaddch(pos.y, pos.x, chars[tick/speed]);
            refresh();
            getchSafe();
        }
        timeout(defaultTimeout);
    }

    void renderRangedAttack(Point2 from, Point2 to, Icon* icon, Level* level, int length){

        if(!level->currentWorld->menuGame){
            return;
        }

        level->currentWorld->menuGame->render(level->currentWorld->worldTime);

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

                    Ui::setColor(icon->getFgColor(Ui::tick, line[i], level), level->tileAt(line[i])->getIcon(level->canSee(level->currentWorld->currentPlayer->pos, line[i], level->currentWorld->currentPlayer->viewDistance))->getBgColor(Ui::tick, line[i], level));
                    mvaddch(pos.y, pos.x, icon->getChar(Ui::tick, line[i], level));
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

            if(getchSafe() != ERR){
                break;
            }

            refresh();
        }

        timeout(defaultTimeout);


    }

    void renderExposion(Point2 center, double radius, Level* level, int length, DamageType damType){
        
        Ui::Color color1;
        Ui::Color color2;
        
        switch (damType) {
                
            case damPoison:
                color1 = C_LIGHT_GREEN;
                color2 = C_DARK_GREEN;
                break;
                
            case damShock:
                color1 = C_LIGHT_MAGENTA;
                color2 = C_DARK_MAGENTA;
                break;
                
            case damIce:
                color1 = C_LIGHT_BLUE;
                color2 = C_LIGHT_CYAN;
                break;
                
            case damFire:
            case damExplosion:
            default:
                color1 = C_LIGHT_RED;
                color2 = C_LIGHT_YELLOW;
                break;
        }

        double p = 8;

        vector<char> chars = {'~', '*', '&'};

        timeout(fastTimeout);

        for(int r=0;r<radius;r++){
            double ma = ((r+1)*p);
            double maDivTau = ma/TAU;

            for(double a = 0; a<ma;a+=maDivTau){
                Point2 circleP = Point2(sin(maDivTau*a)*r, cos(maDivTau*a)*r);
                Point2 worldP = circleP + center;
                Point2 screenP = worldP - level->currentWorld->menuGame->viewPos;
                
                Ui::setColor(r<(radius/3)?color1:color2, level->tileAt(worldP)->getIcon(level->canSee(level->currentWorld->currentPlayer->pos, worldP, level->currentWorld->currentPlayer->viewDistance))->getBgColor(Ui::tick, worldP, level));

                mvaddch(screenP.y, screenP.x, chars[rand()%chars.size()]);
            }

            if(getchSafe() != ERR){
                break;
            }
            refresh();
        }

        timeout(defaultTimeout);

    }

}
