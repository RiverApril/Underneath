//
//  Settings.h
//  Underneath
//
//  Created by Braeden Atlee on 4/14/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Settings__
#define __Underneath__Settings__

#include "Global.hpp"

namespace Settings{

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

    struct SettingBool : Setting{

        bool* value;

        SettingBool(string name, bool* value) : Setting(name){
            this->value = value;
        }

        ~SettingBool(){}

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

        virtual void cycleValue(){
            *value = !*value;
        }
    };


    extern vector<Setting*> settingList;


    bool saveSettings(string path);

    bool loadSettings(string path);

}

#endif /* defined(__Underneath__Settings__) */
