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
#include "MenuTempYesNo.h"

namespace Ui {

    void MenuPreGame::openUi(Menu* oldMenu) {
        if(deleteAnswer != nullptr && *deleteAnswer){
            WorldLoader::deleteWorld(name);
            *deleteAnswer = false;
            delete deleteAnswer;
        }
    }

    void MenuPreGame::closeUi(Menu* newMenu) {

    }

    void MenuPreGame::handleInput(int in) {

        const int selPlay = 0;
        const int selDel = 1;
        const int selBack = 2;

        const int maxUiSelection = selBack;

        move(selection+2, 0);
        clrtoeol();

        switch (in) {
            case KEY_ENTER:
            case 13:
            case '\n':
                switch (selection) {
                    case selPlay:
                        if(name.length() > 0){
                        	changeMenu(new MenuGame(name));
                        }
                        break;

                    case selDel:
                        deleteAnswer = new bool(false);
                        if(WorldLoader::exists(name)){
                        	changeMenu(new MenuTempYesNo("Are you sure you want to delete '"+name+"' ?", deleteAnswer));
                		}
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
                if(name.length() > 0){
                    name = name.substr(0, name.length()-1);
                }
                break;

            default:
                if((in=='_') || (in>='0' && in<='9') || (in>='A' && in<='Z') || (in>='a' && in<='z')){
                    name += in;
                }
                break;
        }

    }

    void MenuPreGame::update() {

        bool e = WorldLoader::exists(name);

        setColor(C_WHITE);
        
        printCenter(2, "Enter Name");

        printCenter(5, e?"  %sContinue%s  ":"%sCreate New%s",
                    selection==0?"- ":"  ", selection==0?" -":"  ");

        setColor(e?C_LIGHT_RED:C_DARK_GRAY);
        printCenter(6, "%sDelete%s",
                    selection==1?"- ":"  ", selection==1?" -":"  ");
        setColor(C_WHITE);

        printCenter(8, "%sBack%s", selection==2?"- ":"  ",
                    selection==2?" -":"  ");

		
        setColor(e?C_LIGHT_GREEN:C_LIGHT_YELLOW);
        move(3, 0);
        clrtoeol();
        printCenter(3, name);
        setColor(C_WHITE, C_BLACK, A_BLINK);
        printCenterOffset(3, (int)(name.length()/2)+1, "_");

        printConsole();

        refresh();
    }
    
}