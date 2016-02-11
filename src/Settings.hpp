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
    extern int musicVolume; //0-100

    extern bool autoSave;

    struct Setting{

        string name = "";

        Setting(string name){
            this->name = name;
        }

        virtual ~Setting(){}

        virtual string renderValue(){
            return "";
        }

        virtual string stringValue(){
            return "ERROR";
        }

        virtual bool setValue(string text){
            return false;
        }

        virtual void cycleValue(bool right){

        }
    };

    struct SettingLabel : Setting{

        SettingLabel(string name) : Setting(name){

        }

        ~SettingLabel(){}

        string renderValue(){
            return "";
        }

        string stringValue(){
            return "";
        }

        bool setValue(string text){
            return true;
        }

        virtual void cycleValue(bool right){}
    };

    struct SettingBool : Setting{

        bool* value;

        SettingBool(string name, bool* value) : Setting(name){
            this->value = value;
        }

        ~SettingBool(){}

        string renderValue(){
            return (*value)?" On":"Off";
        }

        string stringValue(){
            return (*value)?"true":"false";
        }

        bool setValue(string text){
            if(text.compare("true") == 0){
                *value = true;
                return true;
            }else if(text.compare("false") == 0){
                *value = false;
                return true;
            }
            return false;
        }

        virtual void cycleValue(bool right){
            *value = !*value;
        }
    };

    struct SettingPercent : Setting{

        int* value;

        int step;

        SettingPercent(string name, int* value, int step) : Setting(name){
            this->value = value;
            this->step = step;
        }

        ~SettingPercent(){}

        string renderValue(){
            return to_string(*value)+"%";
        }

        string stringValue(){
            return to_string(*value);
        }

        bool setValue(string text){
            *value = atoi(text.c_str());
            return false;
        }

        virtual void cycleValue(bool right){
            *value += right?step:-step;
            if(*value > 100){
                *value = 0;
            }
            while(*value < 0){
                *value = 100;
            }
        }
    };


    extern vector<Setting*> settingList;


    bool saveSettings(string path);

    bool loadSettings(string path);

}

#endif /* defined(__Underneath__Settings__) */
