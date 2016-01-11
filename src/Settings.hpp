//
//  Settings.hpp
//  Underneath
//
//  Created by Braeden Atlee on 4/14/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Settings__
#define __Underneath__Settings__

#include "Global.hpp"

namespace Settings{

    extern bool godMode;
    extern bool debugMode;
    extern bool logFile;
    extern bool showFollowPaths;
    extern bool seeEverything;
    extern bool cheatKeysEnabled;

    extern bool autoSave;

    struct Setting{

        string name = "";

        Setting(string name){
            this->name = name;
        }

        virtual ~Setting(){}

        virtual string stringValue(){
            return "ERROR";
        }

        virtual bool setValue(string text){
            return false;
        }

        virtual void cycleValue(){

        }
    };

    struct SettingLabel : Setting{

        SettingLabel(string name) : Setting(name){

        }

        ~SettingLabel(){}

        string stringValue(){
            return "";
        }

        bool setValue(string text){
            return true;
        }

        virtual void cycleValue(){}
    };

    struct SettingBool : Setting{

        bool* value;

        SettingBool(string name, bool* value) : Setting(name){
            this->value = value;
        }

        ~SettingBool(){}

        string stringValue(){
            return (*value)?" ON":"OFF";
        }

        bool setValue(string text){
            if(text.compare(" ON") == 0){
                *value = true;
                return true;
            }else if(text.compare("OFF") == 0){
                *value = false;
                return true;
            }
            return false;
        }

        virtual void cycleValue(){
            *value = !*value;
        }
    };


    extern vector<Setting*> settingList;


    bool saveSettings(string path);

    bool loadSettings(string path);

}

#endif /* defined(__Underneath__Settings__) */
