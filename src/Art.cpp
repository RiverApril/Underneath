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

    Art* defaultArt;

    int artTitle;

    int artScroll;
    int artCoin;

    int artKnife;
    int artShortSword;
    int artLongSword;
    int artMase;
    int artSpear;
    int artBattleAxe;

    int artLongbow;
    int artRecurveBow;
    int artCrossbow;

    void loadArts(){

        mkdir(ArtDir.c_str(), 0777);

        defaultArt = new Art();
        defaultArt->lines.push_back("Failed to load art.");

        Arts::artTitle = Arts::loadNew(ArtDir+"/title");

        Arts::artScroll = Arts::loadNew(ArtDir+"/scroll");

        Arts::artKnife = Arts::loadNew(ArtDir+"/knife");
        ItemGenerator::wKnife->artIndex = Arts::artKnife;

        Arts::artShortSword = Arts::loadNew(ArtDir+"/shortsword");
        ItemGenerator::wShortSword->artIndex = Arts::artShortSword;

        Arts::artMase = Arts::loadNew(ArtDir+"/mase");
        ItemGenerator::wMase->artIndex = Arts::artMase;

        Arts::artSpear = Arts::loadNew(ArtDir+"/spear");
        ItemGenerator::wSpear->artIndex = Arts::artSpear;

        Arts::artBattleAxe = Arts::loadNew(ArtDir+"/battleaxe");
        ItemGenerator::wBattleAxe->artIndex = Arts::artBattleAxe;

        Arts::artLongSword = Arts::loadNew(ArtDir+"/longsword");
        ItemGenerator::wLongSword->artIndex = Arts::artLongSword;

        Arts::artLongbow = Arts::loadNew(ArtDir+"/longbow");
        ItemGenerator::wLongbow->artIndex = Arts::artLongbow;

        Arts::artRecurveBow = Arts::loadNew(ArtDir+"/recurvebow");
        ItemGenerator::wRecurveBow->artIndex = Arts::artRecurveBow;

        Arts::artCrossbow = Arts::loadNew(ArtDir+"/crossbow");
        ItemGenerator::wCrossbow->artIndex = Arts::artCrossbow;

        Arts::artCoin = Arts::loadNew(ArtDir+"/coin");
        ItemGenerator::iCoin->artIndex = Arts::artCoin;

        //TODO make the rest of the ascii art


    }

    void cleanupArts(){
        delete defaultArt;
        for(Art* a : artList){
            delete a;
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
                    line += (char)c;
                }
            }while(c != EOF);
            //art->lines.push_back(line);
            fclose(file);
        }else{
            debug("Failed to load: "+path);
            return -1;
        }
        art->calcSize();
        artList.push_back(art);
        return (artList.size())-1;
    }


    Art* getArt(int index){
        if(index >= 0 && index < artList.size()){
            return artList[index];
        }else{
            return defaultArt;
        }
    }

}

Point2 Art::calcSize(){
    int w = 0;
    for(string line : lines){
        w = Math::max(w, (int)line.length());
    }
    size.x = w;
    size.y = lines.size();
    return size;
}


Point2 Art::getSize(){
    return size;
}

void Art::printAt(Point2 pos){
    for(size_t i=0;i<lines.size();i++){
    	mvaddstr(pos.y+i, pos.x, lines[i].c_str());
    }
}

void Art::printCenter(int y, int xOffset){
    int w = getSize().x;
    for(size_t i=0;i<lines.size();i++){
        mvprintw(y+i, (Ui::terminalSize.x/2)-((w-1)/2), lines[i].c_str());
    }
}



