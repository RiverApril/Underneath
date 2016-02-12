//
//  MenuDebug.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/18/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "MenuDebug.hpp"
#include "ItemGenerator.hpp"
#include "Controls.hpp"
#include "MenuMessage.hpp"

namespace Ui {

    MenuDebug::MenuDebug() : Menu() {

    }


    void MenuDebug::handleInput(int in){
        if(in == KEY_ESCAPE){
            closeThisMenu();
            return;
        }
    }

    void MenuDebug::update() {
        move(0, 0);
        clrtobot();
        setColor(C_WHITE);

        /*
         To my favorite human...
         To the best girlfriend...
         To the one and only...
         My love...
         Happy Birthday Davalynn!!!
         Davalynn, you are my best friend, someone I can talk to you about anything.
         But you are more than my best friend, much more.
         You are the love of my life.
         I miss you everyday.
         When we are apart, I feel torn in half, like part of me stayed stuck to you.
         When I’m with you I feel safe. I feel at home.
         I love to hug you, to hold you in my arms.
         I hope your birthday is wonderful :)
         I love you Dava.
         */

        printCenter(terminalSize.y/2, "To my favorite human...");
        while(getchSafe() == ERR);
        move(0, 0);
        clrtobot();
        printCenter(terminalSize.y/2, "To the best girlfriend...");
        while(getchSafe() == ERR);
        move(0, 0);
        clrtobot();
        printCenter(terminalSize.y/2, "To the one and only...");
        while(getchSafe() == ERR);
        move(0, 0);
        clrtobot();
        printCenter(terminalSize.y/2, "My love...");
        while(getchSafe() == ERR);
        move(0, 0);
        clrtobot();
        vector<pair<Point2, int>> fireworks;
        for(int i=0;i<10;i++){
        	fireworks.push_back(make_pair(Point2(rand()%terminalSize.x, terminalSize.y+(rand()%20)), rand()%10));
        }
        int m = 0;
        while(true){
            move(0, 0);
            clrtobot();
            for(int i=0;i<fireworks.size();i++){
                if(fireworks[i].first.y > terminalSize.y/2){
                    fireworks[i].first.y--;
                    mvaddch(fireworks[i].first.y, fireworks[i].first.x, '.');
                }else{
                    if(fireworks[i].second > 0){
                        fireworks[i].second--;
                        fireworks[i].first.y--;
                        mvaddch(fireworks[i].first.y, fireworks[i].first.x, ',');
                    }else if(fireworks[i].second <= -10){
                        if(m == 0){
                        	fireworks[i].first = Point2(rand()%terminalSize.x, terminalSize.y+(rand()%20));
                        	fireworks[i].second = rand()%10;
                        }
                    }else{
                        fireworks[i].second--;
                        switch (fireworks[i].second) {
                            case 0:
                                mvaddch(fireworks[i].first.y, fireworks[i].first.x, ' ');
                                break;
                            case -1:
                                mvaddch(fireworks[i].first.y, fireworks[i].first.x, '*');
                                break;
                            case -2:
                                mvaddstr(fireworks[i].first.y-1, fireworks[i].first.x-1, " * ");
                                mvaddstr(fireworks[i].first.y  , fireworks[i].first.x-1, "*.*");
                                mvaddstr(fireworks[i].first.y+1, fireworks[i].first.x-1, " * ");
                                break;
                            case -3:
                                mvaddstr(fireworks[i].first.y-2, fireworks[i].first.x-2, "  .  ");
                                mvaddstr(fireworks[i].first.y-1, fireworks[i].first.x-2, " .*. ");
                                mvaddstr(fireworks[i].first.y  , fireworks[i].first.x-2, ".*.*.");
                                mvaddstr(fireworks[i].first.y+1, fireworks[i].first.x-2, " .*. ");
                                mvaddstr(fireworks[i].first.y+2, fireworks[i].first.x-2, "  .  ");
                                break;
                            case -4:
                                mvaddstr(fireworks[i].first.y-2, fireworks[i].first.x-2, " *.* ");
                                mvaddstr(fireworks[i].first.y-1, fireworks[i].first.x-2, "*\\./*");
                                mvaddstr(fireworks[i].first.y  , fireworks[i].first.x-2, "..*..");
                                mvaddstr(fireworks[i].first.y+1, fireworks[i].first.x-2, "*/.\\*");
                                mvaddstr(fireworks[i].first.y+2, fireworks[i].first.x-2, " *.* ");
                                break;
                            case -5:
                                mvaddstr(fireworks[i].first.y-2, fireworks[i].first.x-2, " . . ");
                                mvaddstr(fireworks[i].first.y-1, fireworks[i].first.x-2, ".   .");
                                mvaddstr(fireworks[i].first.y  , fireworks[i].first.x-2, "  .  ");
                                mvaddstr(fireworks[i].first.y+1, fireworks[i].first.x-2, ".   .");
                                mvaddstr(fireworks[i].first.y+2, fireworks[i].first.x-2, " . . ");
                                break;
                            case -6:

                                break;
                            case -7:

                                break;
                            case -8:

                                break;
                            case -9:
                                
                                break;
                        }
                    }
                }
            }
            switch (m) {
                case 0:
                    printCenter(terminalSize.y/2, "Happy Birthday Davalynn!!! :D");
                    break;
                case 1:
                    printCenter(terminalSize.y/2, "Davalynn, you are my best friend, someone I can talk to you about anything.");
                    break;
                case 2:
                    printCenter(terminalSize.y/2, "But you are more than my best friend, much more.");
                    break;
                case 3:
                    printCenter(terminalSize.y/2, "You are the love of my life.");
                    break;
                case 4:
                    printCenter(terminalSize.y/2, "I miss you everyday.");
                    break;
                case 5:
                    printCenter(terminalSize.y/2, "When we are apart, I feel torn in half, like part of me stayed stuck to you.");
                    break;
                case 6:
                    printCenter(terminalSize.y/2, "When I’m with you I feel safe. I feel at home.");
                    break;
                case 7:
                    printCenter(terminalSize.y/2, "I love to hug you, to hold you in my arms.");
                    break;
                case 8:
                    printCenter(terminalSize.y/2, "I hope your birthday is wonderful :)");
                    break;
                case 9:
                    printCenter(terminalSize.y/2, "I love you Dava.");
                    break;
            }

            if(getchSafe() != ERR){
                m++;
            }

            if(m == 10){
                break;
            }
        }
        move(0, 0);
        clrtobot();
        printCenter(terminalSize.y/2, "I love you so much.");
        while(getchSafe() == ERR);
        move(0, 0);
        clrtobot();
        printCenter(terminalSize.y/2, "<3");
        while(getchSafe() == ERR);
        move(0, 0);
        clrtobot();
        printCenter(terminalSize.y/2, "The end x)");
        while(getchSafe() == ERR);
        closeThisMenu();

    }

}
