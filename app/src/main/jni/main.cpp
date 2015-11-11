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
    mvprintw(1, 0, "udpateMain() - input:%d  tick:%d", input, tick);
    if(input != -1){
        move(3, 0);
        clrtoeol();
        mvprintw(3, 0, "Input was %d at tick:%d", input, tick);
    }
    refresh();
    tick++;
}

void cleanupMain(){
    mvprintw(2, 0, "cleanupMain()");
    refresh();
    endwin();
}

