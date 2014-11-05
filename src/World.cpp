//
//  World.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "World.h"
#include "Utility.h"

#include <sys/stat.h>
#include <sys/types.h>


World::World(std::string n){
    levels = new std::vector<Level*>();
    this->name = new std::string(n);
}

World::~World(){

    if(currentPlayer != nullptr){
    	delete currentPlayer;
    	currentPlayer = nullptr;
    }

    if(name != nullptr){
        delete name;
        name = nullptr;
    }

    if(levels != nullptr){

        for (int i=0; i<levels->size(); i++) {
            if(levels->at(i) != nullptr){
                delete levels->at(i);
                levels->at(i) = nullptr;
            }
        }

        levels->empty();

        delete levels;
        levels = nullptr;
    }

}

namespace WorldLoader {

    World* loadedWorld;

    bool exists(std::string name){

        std::string dir = WorldsDir+"/"+name+"/";

        bool l = false;

        //world.info
        FILE* fileWorldInfo;

        fileWorldInfo = std::fopen((dir+"world"+".info").c_str(), "rb");
        if(fileWorldInfo != nullptr){
            l = true;
        }
        fclose(fileWorldInfo);
        //

        return l;
    }

    char* readData(FILE* file){
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);

        char* buffer = new char[fileSize];
        fread(buffer, fileSize, 1, file);
        printf("Length: %ld\n", fileSize);
        for(int i=0;i<fileSize;i++){
            printf("%d ", (int)buffer[i]);
        }
        return buffer;
    }

    World* load(std::string name){

        std::string dir = WorldsDir+"/"+name+"/";


        World* world = nullptr;

        //world.info
        FILE* fileWorldInfo;

        fileWorldInfo = std::fopen((dir+"world"+".info").c_str(), "rb");
        if(fileWorldInfo != nullptr){

            char* data = readData(fileWorldInfo);

            int* position = new int(0);

            int levelCount = Utility::loadInt8Bit(data, position);

            std::string currentLevelName = Utility::loadString(data, position);
            int playerUniqueId = Utility::loadInt(data, position);

            world = new World(name);



            for(int i=-1;i<levelCount;i++){
                std::string levelName;

                if(i == -1){
                    levelName = currentLevelName;
                }else{
					levelName = Utility::loadString(data, position);
                }

                //levelName.lvl
                FILE* fileLevel;

                fileLevel = std::fopen((dir+(levelName)+".lvl").c_str(), "rb");
                if(fileLevel != nullptr){

                    char* levelData = readData(fileLevel);

                    int* levelPosition = new int(0);

                    Point2 levelSize = Point2::load(levelData, levelPosition);

                    Level* level = new Level(levelName, levelSize);

                    level->load(levelData, levelPosition);


                    if(i == -1){
                        world->currentLevel = level;
                    }else{
                        world->levels->push_back(level);
                    }

                    delete levelPosition;

                }

                fclose(fileLevel);
                //
            }

            if(world->currentLevel == nullptr && world->levels->size() > 0){
                world->currentLevel = world->levels->at(0);
            }

            for(int i=0;i<world->currentLevel->entityCount();i++){
                if(world->currentLevel->entityList[i]->uniqueId == playerUniqueId){
                    world->currentPlayer = (Player*)world->currentLevel->entityList[i];
                }
            }

            delete position;
        }
        fclose(fileWorldInfo);
		//

        return world;
    }

    bool save(World* loadedWorld){

        mkdir(UnderneathDir.c_str(), 0777);
        mkdir(WorldsDir.c_str(), 0777);
        std::string dir = WorldsDir+"/"+loadedWorld->name->c_str()+"/";
        mkdir(dir.c_str(), 0777);

        std::remove((dir+"world"+".info.back").c_str());
        std::rename((dir+"world"+".info").c_str(),
                    (dir+"world"+".info.back").c_str());

        //world.info
        FILE* fileWorldInfo;

        fileWorldInfo = std::fopen((dir+"world"+".info").c_str(), "wb");
        if(fileWorldInfo != nullptr){
            std::string* data = new std::string();

            Utility::saveInt8Bit(data, (int8_t)loadedWorld->levels->size());

            Utility::saveString(data, loadedWorld->currentLevel->getName());
            Utility::saveInt(data, loadedWorld->currentPlayer->uniqueId);

            for(int j=0;j<loadedWorld->levels->size();j++){
            	Utility::saveString(data, loadedWorld->levels->at(j)->getName());
            }

            for(int j=0;j<data->size();j++){
                fputc(data->at(j), fileWorldInfo);
            }

            delete data;
        }
        fclose(fileWorldInfo);
        //


        for(int i=0;i<loadedWorld->levels->size();i++){
            Level* l = loadedWorld->levels->at(i);

            //levelName.lvl
            FILE* fileLevel;

            std::remove((dir+(l->getName())+".lvl.back").c_str());
            std::rename((dir+(l->getName())+".lvl").c_str(),
                        (dir+(l->getName())+".lvl.back").c_str());

            fileLevel = fopen((dir+(l->getName())+".lvl").c_str(), "wb");
            if(fileLevel != nullptr){
                std::string* data = new std::string();

                l->save(data);

                for(int j=0;j<data->size();j++){
                    fputc(data->at(j), fileLevel);
                }

                delete data;

                return true;
            }
            fclose(fileLevel);
            //
        }




        return false;
    }

    World* create(std::string name){

        World* loadedWorld = new World(name);

        loadedWorld->currentLevel = new Level("start");
        Point2 p = loadedWorld->currentLevel->generate(static_cast<unsigned int>(time(NULL)));
        loadedWorld->levels->push_back(loadedWorld->currentLevel);

        loadedWorld->currentPlayer = new Player(name, '@', p, Ui::C_WHITE);
        loadedWorld->currentLevel->newEntity(loadedWorld->currentPlayer);

        save(loadedWorld);

        return loadedWorld;
    }
}
