//
//  Settings.cpp
//  Underneath
//
//  Created by Braeden Atlee on 4/14/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Settings.h"
#include "Controls.h"
#include "Ui.h"
#include "Utility.h"

namespace Setting{

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
                    lines += to_string(*bind.key);
                }
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
