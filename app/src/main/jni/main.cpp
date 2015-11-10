//
// Created by Braeden Atlee on 11/9/15.
//

#include "AndroidCursesInterface.hpp"

int input;
int tick = 0;

void initMain(){

    initscr();
    mvprintw(0, 0, "main()");
    refresh();
}

void updateMain(){
    input = getch();
    mvprintw(0, 1, "udpateMain() - input:%d  tick:%d", input, tick);
    refresh();
    tick++;
}

void cleanupMain(){
    mvprintw(0, 3, "cleanupMain()");
    refresh();
    endwin();
}

