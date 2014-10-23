//
//  Ui.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Ui.h"
#include "UiMenuGame.h"
#include "Global.h"
#include "Entity.h"
#include "AiEntity.h"
#include "Player.h"

namespace Ui {

    const color COLOR_DEFAULT_ENTITY = C_LIGHT_RED;

    int tick = 0;

    double ms = 0;

    int fps = 0;
    long l = 0;
    long lastL = 0;
    long startTime = 0;
    int frames = 0;

    Menu* currentMenu;
    
    void changeMenu(Menu* newMenu){
		delete currentMenu;
        currentMenu = newMenu;
        newMenu->_openUi();
    }

	void initNCurses(){

		setlocale(LC_ALL, "");

        initscr();
        start_color();
        use_default_colors();
        resizeterm(TERMINAL_HEIGHT, TERMINAL_WIDTH);
        keypad(stdscr, TRUE);
        noecho();
        curs_set(0);
        timeout(-1);
        
        initColorPairs();

        refresh();
        
    }

    void initColorPairs(){

        for(int i = 0;i<256;i++){
            init_pair(i, i, 0);
        }
        
    }

    void exitProgram(){
        running = false;
        delete currentMenu;
        endwin();
    }

    void setColor(color c, int attr){
        attrset(COLOR_PAIR(c) | attr);
    }


    ///////////////////////////////////////////////////
    // MENU


    Menu::Menu(int inputTimeout){
		this->inputTimeout = inputTimeout;
    }

    void Menu::_openUi(){
        timeout(inputTimeout);
        move(0, 0);
        clrtobot();
		refresh();
        openUi();
        update();
		refresh();
    }

    void Menu::_handleInput(int in){
        switch (in) {
            case KEY_RESIZE:
                TERMINAL_WIDTH = getmaxx(stdscr);
                TERMINAL_HEIGHT = getmaxy(stdscr);
                move(0, 0);
                clrtobot();
                break;
        }
        handleInput(in);
    }

    void Menu::_update(){

        update();

        tick++;

        l = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        ms = (l - lastL)/1000.0;
        lastL = l;

        frames++;
        if(l - startTime > 1000000){
            startTime = l;
            fps = frames;
            frames = 0;
        }
        
    }


    ///////////////////////////////////////////////////
    // MAIN MENU


	void MenuMain::openUi(){

    }

    void MenuMain::handleInput(int in){
		const int maxUiSelection = 1;

        move(selection+2, 0);
        clrtoeol();

		switch (in) {
            case KEY_ENTER:
            case 13:
            case '\n':
                switch (selection) {
                    case 0:
                        changeMenu(new MenuGame());
                        break;

                    case 1:
                        running = false;
                        break;
                        
                    default:
                        break;
                }
                break;

            case KEY_UP:
                selection--;
                if(selection<0){
					selection = maxUiSelection;
                }
                break;

            case KEY_DOWN:
                selection++;
                if(selection>maxUiSelection){
					selection = 0;
                }
                break;
                
            default:
                break;
        }

    }

	void MenuMain::update(){
        setColor(C_WHITE);
		mvaddstr(1, 1, "Main Menu");
		mvaddstr(2, 3, "Play");
		mvaddstr(3, 3, "Exit");
        mvaddstr(selection+2, 1, "-");
        move(5, 0);
        for(int i=0;i<COLORS*2;i++){
			setColor(i);
			printw("%X ", i);
        }
        refresh();
    }
    

}