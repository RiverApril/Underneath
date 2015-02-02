//
//  Art.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/1/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Art.h"
#include "Math.h"
#include "Menu.h"
#include "Utility.h"
#include "ItemGenerator.h"


namespace Arts{

    vector<Art*> artList;

    int artTitle;
    int artKnife;

    void loadArts(){
        try{
            mkdir(ArtDir.c_str(), 0777);

            Arts::artTitle = Arts::loadNew(ArtDir+"/title");

            Arts::artKnife = Arts::loadNew(ArtDir+"/knife");
            ItemGenerator::wKnife->artIndex = Arts::artKnife;
            ItemGenerator::wMase->artIndex = Arts::artKnife;
            ItemGenerator::wShortSword->artIndex = Arts::artKnife;
            ItemGenerator::wSpear->artIndex = Arts::artKnife;
            ItemGenerator::wBattleAxe->artIndex = Arts::artKnife;
            ItemGenerator::wLongbow->artIndex = Arts::artKnife;
            ItemGenerator::wRecurveBow->artIndex = Arts::artKnife;
            ItemGenerator::wCrossbow->artIndex = Arts::artKnife;

            debug("Knife art index: "+to_string(ItemGenerator::wKnife->artIndex));

        }catch(FileUtility::FileExceptionLoad e){
            console(e.description);
        }
    }

    int loadNew(string name, string ext, string dot){
        string path = name+dot+ext;

        Art* art = new Art();


        FILE* file;
        file = fopen(path.c_str(), "r");
        if(file != NULL){
            int c;
            string line = "";
            do{
                c = fgetc(file);
                if(c == '\n'){
                    art->lines.push_back(line);
                    line = "";
                }else{
                    line += c;
                }
            }while(c != EOF);
            art->lines.push_back(line);
            fclose(file);
        }else{
            throw FileUtility::FileExceptionLoad("Failed to load file: "+path);
        }
        artList.push_back(art);
        return ((int)artList.size())-1;
    }

}


Point2 Art::getSize(){
    Point2 p;
    int w = 0;
    for(string line : lines){
        w = Math::max(w, (int)line.length());
    }
    p.x = w;
    p.y = (int)lines.size();
    return p;
}

void Art::printAt(Point2 pos){
    for(int i=0;i<lines.size();i++){
    	mvaddstr(pos.y+i, pos.x, lines[i].c_str());
    }
}

void Art::printCenter(int y, int xOffset){
    int w = getSize().x;
    for(int i=0;i<lines.size();i++){
        mvprintw(y+i, (Ui::terminalSize.x/2)-((int)(w-1)/2), lines[i].c_str());
    }
}



