//
//  MenuPreGame.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/3/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuPreGame.h"
#include "MenuMain.h"
#include "MenuGame.h"

namespace Ui {

    void MenuPreGame::openUi(Menu* oldMenu) {

    }

    void MenuPreGame::closeUi(Menu* newMenu) {
        delete this;
    }

    void MenuPreGame::handleInput(int in) {

        const int selName = 0;
        const int selPlay = 1;
        const int selDel = 2;
        const int selBack = 3;

        const int maxUiSelection = selBack;

        move(selection+2, 0);
        clrtoeol();

        switch (in) {
            case KEY_ENTER:
            case 13:
            case '\n':
            case ' ':
                switch (selection) {
                    case selPlay:
                    case selName:
                        if(name.length() > 0){
                        	changeMenu(new MenuGame(name));
                        }
                        break;

                    case selDel:
                        WorldLoader::deleteWorld(name);
                        break;

                    case selBack:
                        changeMenu(new MenuMain());
                        break;

                    default:
                        break;
                }
                break;

            case KEY_UP:
                selection--;
                if(selection<0) {
                    selection = maxUiSelection;
                }
                break;

            case KEY_DOWN:
                selection++;
                if(selection>maxUiSelection) {
                    selection = 0;
                }
                break;

            case 27:
                changeMenu(new MenuMain());
                break;

            case KEY_BACKSPACE:
            case 8: //Backspace
            case 127: //Delete
                if(selection == selName){
                    if(name.length() > 0){
                        name = name.substr(0, name.length()-1);
                    }
                }
                break;

            default:
                if(selection == selName){
                    if((in>=65 && in<=90) || (in>=97 && in<=122)){
                        name += in;
                    }
                }
                break;
        }

    }

    void MenuPreGame::update() {
        setColor(C_WHITE);

        mvprintw(1, 1, "Enter name to play");
        mvprintw(2, 3, "Enter Name: %s", name.c_str());
        mvprintw(3, 3, "Start");
        mvprintw(4, 3, "Delete");
        mvprintw(5, 3, "Back");

        mvprintw(selection+2, 1, "-");


        std::string s = "";

        if(name.length() == 0){
            setColor(C_LIGHT_RED);
			s = "You need to enter a name.";
        }else{
            if(WorldLoader::exists(name)){
                setColor(C_LIGHT_GREEN);
                s = "World exists, it will be loaded.";
            }else{
                setColor(C_LIGHT_YELLOW);
                s = "World does not exist, it will be created.";
            }
        }
        move(6, 1);
        clrtoeol();
        printw(s.c_str());

        printConsole();

        refresh();
    }
    
}