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

namespace Ui{
    class Menu;
}

namespace Settings{
    
    typedef char leftRight;
    
    const leftRight dLeft = 'L';
    const leftRight dRight = 'R';
    const leftRight dEnter = 'E';

    extern bool godMode;
    extern bool debugMode;
    extern bool logFile;
    extern bool showFollowPaths;
    extern bool seeEverything;
    extern bool cheatKeysEnabled;
    extern int musicVolume; //0-100

    extern bool autoSave;
    extern int autoSaveDelay;

    struct Setting{

        string name = "";
        
        function<bool()> condition = []{return true;};

        Setting(function<bool()> condition, string name){
            this->condition = condition;
            this->name = name;
        }
        
        Setting(string name){
            this->name = name;
        }

        virtual ~Setting(){}

        virtual string renderValue(unsigned long tick){
            return "";
        }

        virtual string stringValue(){
            return "ERROR";
        }

        virtual bool setValue(string text){
            return false;
        }

        virtual void cycleValue(leftRight dir, Ui::Menu* menu){

        }
        
        virtual void resetToDefault(){
            
        }
    };

    struct SettingLabel : Setting{

        SettingLabel(string name) : Setting(name){

        }

        ~SettingLabel(){}

        string renderValue(unsigned long tick){
            return name;
        }

        string stringValue(){
            return "";
        }

        bool setValue(string text){
            return true;
        }

        virtual void cycleValue(leftRight dir, Ui::Menu* menu){}
    };

    struct SettingBool : Setting{

        bool defaultValue;
        bool* value;

        SettingBool(string name, bool* value) : Setting(name){
            this->value = value;
            this->defaultValue = *value;
        }

        ~SettingBool(){}

        string renderValue(unsigned long tick){
            return "   " + name + " - " +  ((*value)?"On":"Off");
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

        virtual void cycleValue(leftRight dir, Ui::Menu* menu){
            *value = !*value;
        }
        
        virtual void resetToDefault(){
            *value = defaultValue;
        }
    };
    
    
    struct SettingExe : Setting{
        
        function<void(Ui::Menu*)> exe;
        
        SettingExe(string name, function<void(Ui::Menu*)> exe) : Setting(name){
            this->exe = exe;
        }
        
        ~SettingExe(){}
        
        string renderValue(unsigned long tick){
            return "   " + name;
        }
        
        string stringValue(){
            return "";
        }
        
        bool setValue(string text){
            return true;
        }
        
        virtual void cycleValue(leftRight dir, Ui::Menu* menu){
            if(dir == dEnter){
                exe(menu);
            }
        }
    };
    
    struct SettingIntFormat : Setting{
        
        int defaultValue;
        int* value;
        
        int min;
        int max;
        int step;
        string format;
        
        SettingIntFormat(string name, int* value, int min, int max, int step, string format) : Setting(name){
            this->value = value;
            this->defaultValue = *value;
            this->min = min;
            this->max = max;
            this->step = step;
            this->format = format;
        }
        
        SettingIntFormat(function<bool()> condition, string name, int* value, int min, int max, int step, string format) : Setting(condition, name){
            this->value = value;
            this->defaultValue = *value;
            this->min = min;
            this->max = max;
            this->step = step;
            this->format = format;
        }
        
        ~SettingIntFormat(){}
        
        string renderValue(unsigned long tick);
        
        string stringValue(){
            return to_string(*value);
        }
        
        bool setValue(string text){
            *value = atoi(text.c_str());
            return false;
        }
        
        virtual void cycleValue(leftRight dir, Ui::Menu* menu){
            if(dir == dRight){
                *value += step;
            }else if(dir == dLeft){
                *value -= step;
            }else{
                *value += step;
            }
            
            if(*value > max){
                *value = min;
            }
            if(*value < min){
                *value = max;
            }
        }
        
        virtual void resetToDefault(){
            *value = defaultValue;
        }
    };


    extern vector<Setting*> settingList;


    bool saveSettings(string path);

    bool loadSettings(string path);

}

#include "Menu.hpp"

#endif /* defined(__Underneath__Settings__) */
