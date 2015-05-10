//
//  Art.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/1/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Art.hpp"
#include "Math.hpp"
#include "Menu.hpp"
#include "Utility.hpp"


namespace Arts {

    vector<Art*> artList;

    Art* defaultArt;

    vector<int> compassInList;
    int compassOut;
    Point2 compassInOffset;

    int artTitle;

    int artCoin;

    int artScroll;
    int artScrollFire;
    int artScrollFrost;
    int artScrollShock;
    int artScrollHand;
    int artScrollHeal;
    int artScrollPerson;

    int artPotion1;
    int artPotion2;
    int artPotion3;
    int artPotion4;
    
    int artBomb;

    int artKnife;
    int artDagger;
    int artCleaver;

    int artLongsword;
    int artCutlass;
    int artKatana;
    int artMachete;
    int artGladius;
    int artScimitar;
    int artRapier;
    int artShortSword;
    int artBroadsword;
    int artSaber;
    int artClaymore;

    int artAxe;
    int artDoubleAxe;

    int artMace;
    int artClub;
    int artFlail;
    int artWarHammer;

    int artSpear;
    int artHalberd;


    int artLongbow;
    int artRecurveBow;

    int artCrossbow;

    void loadArts() {

        mkdir(ArtDir.c_str(), 0777);

        defaultArt = new Art();
        defaultArt->lines.push_back("Failed to load art.");

        Arts::artTitle = Arts::loadNew(ArtDir + "/title");

        Arts::artScroll = Arts::loadNew(ArtDir + "/scroll");
        Arts::artScrollFire = Arts::loadNew(ArtDir + "/scrollFire");
        Arts::artScrollFrost = Arts::loadNew(ArtDir + "/scrollFrost");
        Arts::artScrollShock = Arts::loadNew(ArtDir + "/scrollShock");
        Arts::artScrollHand = Arts::loadNew(ArtDir + "/scrollHand");
        Arts::artScrollHeal = Arts::loadNew(ArtDir + "/scrollHeal");
        Arts::artScrollPerson = Arts::loadNew(ArtDir + "/scrollPerson");

        Arts::artPotion1 = Arts::loadNew(ArtDir + "/potion1");
        Arts::artPotion2 = Arts::loadNew(ArtDir + "/potion2");
        Arts::artPotion3 = Arts::loadNew(ArtDir + "/potion3");
        Arts::artPotion4 = Arts::loadNew(ArtDir + "/potion4");
        
        Arts::artBomb = Arts::loadNew(ArtDir + "/bomb");

        Arts::artKnife = Arts::loadNew(ArtDir + "/knife");
        Arts::artDagger = Arts::loadNew(ArtDir + "/dagger");
        Arts::artCleaver = Arts::loadNew(ArtDir + "/cleaver");

        Arts::artLongsword = Arts::loadNew(ArtDir + "/longsword");
        Arts::artCutlass = Arts::loadNew(ArtDir + "/cutlass");
        Arts::artKatana = Arts::loadNew(ArtDir + "/katana");
        Arts::artMachete = Arts::loadNew(ArtDir + "/machete");
        Arts::artGladius = Arts::loadNew(ArtDir + "/gladius");
        Arts::artScimitar = Arts::loadNew(ArtDir + "/scimitar");
        Arts::artRapier = Arts::loadNew(ArtDir + "/rapier");
        Arts::artShortSword = Arts::loadNew(ArtDir + "/shortsword");
        Arts::artBroadsword = Arts::loadNew(ArtDir + "/broadsword");
        Arts::artSaber = Arts::loadNew(ArtDir + "/saber");
        Arts::artClaymore = Arts::loadNew(ArtDir + "/claymore");


        Arts::artAxe = Arts::loadNew(ArtDir + "/axe");
        Arts::artDoubleAxe = Arts::loadNew(ArtDir + "/doubleaxe");

        Arts::artMace = Arts::loadNew(ArtDir + "/mace");
        Arts::artClub = Arts::loadNew(ArtDir + "/club");
        Arts::artFlail = Arts::loadNew(ArtDir + "/flail");
        Arts::artWarHammer = Arts::loadNew(ArtDir + "/warhammer");

        Arts::artSpear = Arts::loadNew(ArtDir + "/spear");
        Arts::artHalberd = Arts::loadNew(ArtDir + "/halberd");


        Arts::artLongbow = Arts::loadNew(ArtDir + "/longbow");

        Arts::artRecurveBow = Arts::loadNew(ArtDir + "/recurvebow");

        Arts::artCrossbow = Arts::loadNew(ArtDir + "/crossbow");

        Arts::artCoin = Arts::loadNew(ArtDir + "/coin");

        Point2 compassInSize = Point2Neg1;

        //Compass

        {

            int fullCompassIn = loadNew(ArtDir+"/compassInner");

            Art* fullCompassInArt = getArt(fullCompassIn);

            Point2 fullInSize = fullCompassInArt->getSize();

            int w = 0;
            int h = 0;

            for(int i=0;i<fullInSize.x;i++){
                if(fullCompassInArt->lines[0][i] == '#'){
                    w++;
                }else{
                    compassInSize.x = w;
                    break;
                }
            }

            for(int j=0;j<fullInSize.y;j++){
                if(fullCompassInArt->lines[j][0] == '#'){
                    h++;
                }else{
                    compassInSize.y = h;
                    break;
                }
            }

            for(int j=compassInSize.y;j<fullInSize.y;j+=compassInSize.y){
                Art* a = new Art();

                for(int i=0;i<compassInSize.y;i++){
                    if(j+i < fullCompassInArt->lines.size()){
                        a->lines.push_back(fullCompassInArt->lines[j+i]);
                    }
                }
                
                a->calcSize();
                artList.push_back(a);
                compassInList.push_back(((int)artList.size())-1);
            }

        }

        //
        {
            int fullCompassOut = loadNew(ArtDir+"/compassOuter");

            Art* fullCompassOutArt = getArt(fullCompassOut);

            Point2 fullOutSize = fullCompassOutArt->getSize();

            for(int i=0;i<fullOutSize.x;i++){
                if(fullCompassOutArt->lines[0][i] == '#'){
                    compassInOffset.x++;
                }else{
                    break;
                }
            }

            for(int j=0;j<fullOutSize.y;j++){
                if(fullCompassOutArt->lines[j][0] == '#'){
                    compassInOffset.y++;
                }else{
                    break;
                }
            }
            Art* a = new Art();

            for(int i=compassInOffset.y;i<fullOutSize.y;i++){
                a->lines.push_back(fullCompassOutArt->lines[i]);
            }

            artList.push_back(a);
            compassOut = ((int)artList.size())-1;
        }

        //end




    }

    void cleanupArts() {
        delete defaultArt;
        for (Art* a : artList) {
            delete a;
        }
        artList.clear();
    }

    int loadNew(string name, string ext, string dot) {
        string path = name + dot + ext;

        Art* art = new Art();


        FILE* file;
        file = fopen(path.c_str(), "r");
        if (file != NULL) {
            int c;
            string line = "";
            do {
                c = fgetc(file);
                if (c == '\n') {
                    art->lines.push_back(line);
                    line = "";
                } else {
                    line += (char) c;
                }
            } while (c != EOF);
            //art->lines.push_back(line);
            fclose(file);
        } else {
            debug("Failed to load: " + path);
            return -1;
        }
        art->calcSize();
        artList.push_back(art);
        return ((int) artList.size()) - 1;
    }

    Art* getArt(int index) {
        if (index >= 0 && index < artList.size()) {
            return artList[index];
        } else {
            return defaultArt;
        }
    }

}

Point2 Art::calcSize() {
    int w = 0;
    for (string line : lines) {
        w = Math::max(w, (int) line.length());
    }
    size.x = w;
    size.y = (int) lines.size();
    return size;
}

Point2 Art::getSize() {
    return size;
}

void Art::printAt(Point2 pos) {
    for (size_t i = 0; i < lines.size(); i++) {
        mvaddstr(pos.y + i, pos.x, lines[i].c_str());
    }
}

void Art::printCenter(int y, int xOffset) {
    int w = getSize().x;
    for (int i = 0; i < lines.size(); i++) {
        mvprintw(y + i, (Ui::terminalSize.x / 2)-((w - 1) / 2), lines[i].c_str());
    }
}
