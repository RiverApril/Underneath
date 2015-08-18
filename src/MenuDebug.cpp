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

    MenuDebug::MenuDebug(World* w) : Menu() {
        this->currentWorld = w;

    }


    void MenuDebug::handleInput(int in){
        if(in == KEY_ESCAPE || in == Key::debugMenu){
            closeThisMenu();
            return;
        }else if(in == 'q'){
            openMenu(new MenuMessage("Test 1", new MenuMessage("Test 2")));
            return;
        }
    }

    void MenuDebug::update() {
        move(0, 0);
        clrtobot();
        setColor(C_WHITE);

        /*for(int i=0;i<terminalSize.y;i++){
            mvprintw(i, 0, "%2d: %d", i, currentWorld->currentPlayer->xpForLevel(i));
        }*/
        
        move(0, 0);

        for(int i=0;i<0x10;i++){
            for(int j=0;j<0x10;j++){
                setColor(i, j);
                addch('#');
            }
            addch('\n');
        }

        for(int i=0;i<0x10;i++){
            for(int j=0;j<0x10;j++){
                printMultiLineColoredString(i, j+20, colorCode(i, j)+"#");
            }
        }

        /*

        int a = 0;

        mvvline(a, column1 - 1, '|', terminalSize.y);
        mvvline(a, column2 - 1, '|', terminalSize.y);
        mvvline(a, column3 - 1, '|', terminalSize.y);

        mvprintw(a, column0, "Weapon");
        mvprintw(a, column1, "Damage");
        mvprintw(a, column2, "Delay");
        mvprintw(a, column3, "Dam/Time");

        a++;

        mvhline(a, 0, '-', terminalSize.x);

        a++;

        for (ItemGenerator::WeaponBase w : ItemGenerator::weaponList) {
            mvprintw(a, column0, "%s", w.names[0].c_str());
            mvprintw(a, column1, "%.2f", w.damage);
            mvprintw(a, column2, "%.2f", w.useDelay);
            mvprintw(a, column3, "%.2f", w.damage / w.useDelay);
            a++;
        }*/

    }

}
