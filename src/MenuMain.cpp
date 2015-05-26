//
//  MenuMain.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuMain.hpp"
#include "MenuGame.hpp"
#include "MenuWorldSelect.hpp"
#include "Global.hpp"
#include "Controls.hpp"
#include "MenuControls.hpp"

namespace Ui {

    MenuMain::MenuMain() : Menu() {
    }

    MenuMain::~MenuMain() {
    }

    bool MenuMain::openUi() {
        move(0, 0);
        clrtobot();
        return true;
    }

    void MenuMain::closeUi() {

    }

    void MenuMain::handleInput(int in) {
        const int maxUiSelection = 2;

        move(selection + 2, 0);
        clrtoeol();

        if(in == KEY_ESCAPE){
            running = false;
        } else if(in == '\n' || in == ' '){
            switch (selection) {
                case 0:
                    openMenu(new MenuWorldSelect());
                    break;

                case 1:
                    openMenu(new MenuControls());
                    break;

                case 2:
                    running = false;
                    break;

                default:
                    break;
            }
        }else if(in == Key::uiUp){
            selection--;
            if (selection < 0) {
                selection = maxUiSelection;
            }

        }else if(in == Key::uiDown){
            selection++;
            if (selection > maxUiSelection) {
                selection = 0;
            }
        }

    }

    void MenuMain::update() {

        int a = 2;

        setColor(C_WHITE);
        /*printCenter(a++, "  _ _ ___ ___ ___ ___ ___ ___ ___ ___ ___  ");
        printCenter(a++, " | | |   |   \\ __|  _|   | __| _ |_ _| | | ");
        printCenter(a++, " | | | | | | | __| | | | | __|   || ||   | ");
        printCenter(a++, " |___|_|_|___/___|_| |_|_|___|_|_||_||_|_| ");
        printCenter(a++, " (    )     (    (       )        (      ) ");
        printCenter(a++, "  )  (       )    )     (          )    (  ");
        printCenter(a++, " (    )     (    (                (      ) ");
        printCenter(a++, "  )  (       )                     )       ");
        printCenter(a++, " (          (                     (        ");
        printCenter(a++, "  )                                )       ");
        printCenter(a++, "                                  (        ");*/
        Arts::getArt(Arts::artTitle)->printCenter(a);
        a += Arts::getArt(Arts::artTitle)->getSize().y;
        printCenter(a++, "%sPlay%s", selection == 0 ? "- " : "  ", selection == 0 ? " -" : "  ");

        //setColor(C_DARK_GRAY);
        printCenter(a++, "%sControls%s", selection == 1 ? "- " : "  ", selection == 1 ? " -" : "  ");
        //setColor(C_BLACK);

        printCenter(a++, "%sExit%s", selection == 2 ? "- " : "  ", selection == 2 ? " -" : "  ");

        a += 4;

        if (limitedColorMode) {
            mvaddstr(a, 0, "Limited Color Mode - For best results use a terminal with 256 colors.");
        }
		/*
        a++;

        move(a++, 0);
        setColor(C_DARK_BLACK);
        addstr(" D_BLACK");
        setColor(C_DARK_RED);
        addstr(" D_RED");
        setColor(C_DARK_GREEN);
        addstr(" D_GREEN");
        setColor(C_DARK_YELLOW);
        addstr(" D_YELLOW");
        setColor(C_DARK_BLUE);
        addstr(" D_BLUE");
        setColor(C_DARK_MAGENTA);
        addstr(" D_MAGENTA");
        setColor(C_DARK_CYAN);
        addstr(" D_CYAN");
        setColor(C_DARK_WHITE);
        addstr(" D_WHITE");

        move(a++, 0);
        setColor(C_LIGHT_BLACK);
        addstr(" L_BLACK");
        setColor(C_LIGHT_RED);
        addstr(" L_RED");
        setColor(C_LIGHT_GREEN);
        addstr(" L_GREEN");
        setColor(C_LIGHT_YELLOW);
        addstr(" L_YELLOW");
        setColor(C_LIGHT_BLUE);
        addstr(" L_BLUE");
        setColor(C_LIGHT_MAGENTA);
        addstr(" L_MAGENTA");
        setColor(C_LIGHT_CYAN);
        addstr(" L_CYAN");
        setColor(C_LIGHT_WHITE);
        addstr(" L_WHITE");

        move(a++, 0);
        setColor(C_DARK_BLACK, C_WHITE);
        addstr(" D_BLACK");
        setColor(C_DARK_RED, C_WHITE);
        addstr(" D_RED");
        setColor(C_DARK_GREEN, C_WHITE);
        addstr(" D_GREEN");
        setColor(C_DARK_YELLOW, C_WHITE);
        addstr(" D_YELLOW");
        setColor(C_DARK_BLUE, C_WHITE);
        addstr(" D_BLUE");
        setColor(C_DARK_MAGENTA, C_WHITE);
        addstr(" D_MAGENTA");
        setColor(C_DARK_CYAN, C_WHITE);
        addstr(" D_CYAN");
        setColor(C_DARK_WHITE, C_WHITE);
        addstr(" D_WHITE");

        move(a++, 0);
        setColor(C_LIGHT_BLACK, C_WHITE);
        addstr(" L_BLACK");
        setColor(C_LIGHT_RED, C_WHITE);
        addstr(" L_RED");
        setColor(C_LIGHT_GREEN, C_WHITE);
        addstr(" L_GREEN");
        setColor(C_LIGHT_YELLOW, C_WHITE);
        addstr(" L_YELLOW");
        setColor(C_LIGHT_BLUE, C_WHITE);
        addstr(" L_BLUE");
        setColor(C_LIGHT_MAGENTA, C_WHITE);
        addstr(" L_MAGENTA");
        setColor(C_LIGHT_CYAN, C_WHITE);
        addstr(" L_CYAN");
        setColor(C_LIGHT_WHITE, C_WHITE);
        addstr(" L_WHITE");
		*/
        refresh();
    }

}
