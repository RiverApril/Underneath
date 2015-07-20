//
//  MenuSettings.cpp
//  Underneath
//
//  Created by Braeden Atlee on 7/19/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "MenuSettings.hpp"
#include "Controls.hpp"
#include "Settings.hpp"

namespace Ui {

    MenuSettings::MenuSettings() : Menu() {

    }


    void MenuSettings::handleInput(int in){

        if(in == KEY_ESCAPE){
            Settings::saveSettings(UnderneathDir + "settings.txt");
            closeThisMenu();
            return;
        }else if(in == Key::uiUp){
            selected--;
            if (selected < 0) {
                selected = (int)Settings::settingList.size()-1;
            }
        }else if(in == Key::uiDown){
            selected++;
            if (selected >= Settings::settingList.size()) {
                selected = 0;
            }

        }else if(in == '\n'){
            Settings::settingList[selected]->cycleValue();
        }
    }

    void MenuSettings::update() {
        move(0, 0);
        clrtobot();
        setColor(C_WHITE);

        mvprintw(0, 0, "Settings - auto saves on escape");

        int a = 2;

        mvprintw(selected+a, 0, "-");

        for(Settings::Setting* setting : Settings::settingList){
            mvprintw(a++, 2, "%s: %s", setting->name.c_str(), setting->stringValue().c_str());
        }

    }
    
}