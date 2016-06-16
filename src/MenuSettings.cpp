//
//  MenuSettings.cpp
//  Underneath
//
//  Created by Braeden Atlee on 7/19/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "MenuSettings.hpp"
#include "MenuYesNo.hpp"
#include "Controls.hpp"
#include "Settings.hpp"

namespace Ui {

    MenuSettings::MenuSettings() : Menu() {

    }
    
    bool MenuSettings::openUi() {
        return true;
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
            }else{
                if(dynamic_cast<Settings::SettingLabel*>(Settings::settingList[selected]) || !Settings::settingList[selected]->condition()){
                    MenuSettings::handleInput(Key::uiUp);
                }
            }
        }else if(in == Key::uiDown){
            selected++;
            if (selected >= (int)Settings::settingList.size()) {
                selected = 0;
            }else{
                if(dynamic_cast<Settings::SettingLabel*>(Settings::settingList[selected]) || !Settings::settingList[selected]->condition()){
                    MenuSettings::handleInput(Key::uiDown);
                }
            }

        }else if(in == Key::uiLeft){
            Settings::settingList[selected]->cycleValue(Settings::dLeft, this);
            
        }else if(in == Key::uiRight){
            Settings::settingList[selected]->cycleValue(Settings::dRight, this);

        }else if(in == '\n'){
            Settings::settingList[selected]->cycleValue(Settings::dEnter, this);
            
        }
    }

    void MenuSettings::update() {
        move(0, 0);
        clrtobot();
        setColor(C_WHITE);

        /*mvprintw(0, 0, "Settings");

        int a = 2;

        mvprintw(selected+a, 0, "-");

        for(Settings::Setting* setting : Settings::settingList){
            setColor(setting->condition() ? C_WHITE : C_DARK_GRAY);
            if(setting->renderValue(tick).size() == 0){
                mvprintw(a++, 1, "%s", setting->name.c_str());
            }else{
            	mvprintw(a++, 3, "%s - %s", setting->renderValue(tick).c_str(), setting->name.c_str());
            }
        }*/
        
        int y = 0;
        mvprintw(y++, 0, "Settings  -  Press [ %s ], [ %s ], or [ %s ] to cycle settings", keyDisplayName(Key::uiLeft).c_str(), keyDisplayName(Key::uiRight).c_str(), keyDisplayName('\n').c_str());
        mvhline(y++, 0, '-', terminalSize.x);
        
        
        int countI = terminalSize.y - 3;
        int midI = (countI / 2) + selected;
        int minI = max(0, midI - countI);
        int maxI = min(midI + countI, (int) Settings::settingList.size());
        
        for (int i = minI; i < maxI; i++) {
            if(i == selected){
                setColor(C_BLACK, C_WHITE);
            }else{
                setColor(Settings::settingList[i]->condition() ? C_WHITE : C_DARK_GRAY);
            }
            
            mvprintw(y++, 1, "%s", Settings::settingList[i]->renderValue(tick).c_str());
        }

    }

}










