//
//  MenuEscape.cpp
//  Underneath
//
//  Created by Emily Atlee on 5/26/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#include "MenuEscape.hpp"
#include "MenuYesNo.hpp"
#include "Settings.hpp"
#include "MenuControls.hpp"
#include "MenuSettings.hpp"

namespace Ui {
    
    MenuEscape::MenuEscape(MenuGame* game) : Menu() {
        menuGame = game;
    }
    
    MenuEscape::~MenuEscape() {
    }
    
    bool MenuEscape::openUi() {
        if(*(menuGame->saveAnswer) != aUndefined){
            closeThisMenu();
        }
        return true;
    }
    
    void MenuEscape::closeUi() {
        
    }
    
    void MenuEscape::handleInput(int in) {
        const int maxUiSelection = 3;
        
        move(selection + 2, 0);
        clrtoeol();
        
        if(in == KEY_ESCAPE){
            closeThisMenu();
        } else if(in == Key::interact || in == '\n'){
            switch (selection) {
                case 0:
                    closeThisMenu();
                    break;
                    
                case 1:
                    openMenu(new MenuControls());
                    break;
                    
                case 2:
                    openMenu(new MenuSettings());
                    break;
                    
                case 3:
                    if(Settings::autoSave && menuTime > Settings::autoSaveDelay){
                        closeAllMenus();
                    } else {
                        openMenu(new MenuYesNo("Do you want to save '" + menuGame->currentWorld->name + "' ?", menuGame->saveAnswer, true));
                    }
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
    
    void MenuEscape::update() {
        move(0, 0);
        clrtobot();
        setColor(C_WHITE);
        
        int a = ( terminalSize.y / 2 ) - 4;
        
        printCenter(a++, "%sResume%s", selection == 0 ? "- " : "  ", selection == 0 ? " -" : "  ");
        
        printCenter(a++, "%sControls%s", selection == 1 ? "- " : "  ", selection == 1 ? " -" : "  ");
        
        printCenter(a++, "%sSettings%s", selection == 2 ? "- " : "  ", selection == 2 ? " -" : "  ");
        
        printCenter(a++, " %sMain Menu%s", selection == 3 ? "- " : "  ", selection == 3 ? " -" : "  ");
        
        a = terminalSize.y - 2;
        
        if(Settings::autoSave && menuGame->currentWorld && menuTime == Settings::autoSaveDelay){
            printCenter(a++, "   Auto Saving...");
            WorldLoader::save(menuGame->currentWorld);
        }
        
        if(menuTime > Settings::autoSaveDelay){
            printCenter(a++, "   Auto Saved    ");
        }
        
        menuTime++;
    }
    
}
