//
//  Settings.cpp
//  Underneath
//
//  Created by Braeden Atlee on 4/14/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Settings.hpp"
#include "Controls.hpp"
#include "Ui.hpp"
#include "Utility.hpp"

namespace Settings{

    bool godMode = false;
    bool debugMode = false;
    bool logFile = false;
    bool showFollowPaths = false;
    bool seeEverything = false;
    bool cheatKeysEnabled = false;

    bool autoSave = true;

    vector<Setting*> settingList = {
        new SettingBool("Auto Save", &autoSave),
        new SettingLabel(""),
        new SettingBool("Debug Output", &debugMode),
        new SettingBool("Log File", &logFile),
        new SettingBool("God Mode", &godMode),
        new SettingBool("Show AI Paths", &showFollowPaths),
        new SettingBool("X-ray Vision", &seeEverything),
        new SettingBool("Cheat Keys", &cheatKeysEnabled),
    };

    vector<Setting*> hiddenSettingList = {
    };

    bool saveSettings(string path){
        FILE* file;
        file = fopen(path.c_str(), "w");
        if(file != NULL){

            string lines = "";

            for(KeyBind bind : keybindings){
                lines += bind.name;
                lines += ":";
                if(*bind.key >= 32 && *bind.key < 127){
                    lines += "'";
                    lines += ((char)*bind.key);
                    lines += "'";
                }else{
                    lines += formatString("%d", *bind.key);
                }
                lines += "\n";
            }

            lines += "\n";

            for(Setting* set : settingList){
                if(set->name.size() == 0){
                    continue;
                }
                lines += set->name;
                lines += ":";
                lines += set->stringValue();
                lines += "\n";
            }

            for(Setting* set : hiddenSettingList){
                if(set->name.size() == 0){
                    continue;
                }
                lines += set->name;
                lines += ":";
                lines += set->stringValue();
                lines += "\n";
            }

            const char* buffer = lines.c_str();

            fprintf(file, "%s", buffer);
            fclose(file);
            return true;
        } else {
            debug("Failed to save settings at: " + path);
            return false;
        }
    }

    bool loadSettings(string path){

        FILE* file;
        file = fopen(path.c_str(), "r");
        if (file != NULL) {
            int c;
            string line = "";
            string settingId = "";
            do {
                c = fgetc(file);
                if (c == '\n') {

                    for(KeyBind bind : keybindings){
                        if(settingId.compare(bind.name) == 0){
                            if(line.size() == 3 && line[0]=='\'' && line[2]=='\''){
                                *bind.key = (int)line[1];
                            }else{
                                try {
                                    int keyCode = Utility::parseIntUnstable(line);
                                    *bind.key = keyCode;
                                } catch (Utility::ParseException) {

                                }
                            }
                            break;
                        }
                    }

                    for(Setting* setting : settingList){
                        if(settingId.compare(setting->name) == 0){
                            try{
                                setting->setValue(line);
                            }catch(Utility::ParseException){

                            }
                            break;
                        }
                    }

                    for(Setting* setting : hiddenSettingList){
                        if(settingId.compare(setting->name) == 0){
                            try{
                                setting->setValue(line);
                            }catch(Utility::ParseException){

                            }
                            break;
                        }
                    }

                    settingId = "";
                    line = "";
                } else if (c == ':') {
                    settingId = line;
                    line = "";
                } else {
                    line += (char) c;
                }
            } while (c != EOF);
            fclose(file);
            return true;
        } else {
            debug("Failed to load settings at: " + path);
            return false;
        }
    }


}
