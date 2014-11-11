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
    levels = shared_ptr<std::vector<shared_ptr<Level>>>(new std::vector<shared_ptr<Level>>());
    this->name = shared_ptr<string>(new string(n));
}

World::~World(){


}

namespace WorldLoader {

    shared_ptr<World> loadedWorld;

    bool exists(std::string name){
        debug("Does "+name+" Exist?");

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

        debug(l?"Yes ":"No ");

        return l;
    }

    char* readData(FILE* file){
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);

        char* buffer = new char[fileSize];
        fread(buffer, fileSize, 1, file);
        /*printf("Length: %ld\n", fileSize);
        for(int i=0;i<fileSize;i++){
            printf("%d ", (int)buffer[i]);
        }*/
        return buffer;
    }

    shared_ptr<World> load(std::string name){
        debug("Loading "+name+"...");

        std::string dir = WorldsDir+"/"+name+"/";


        shared_ptr<World> world = nullptr;

        //world.info
        FILE* fileWorldInfo;

        fileWorldInfo = std::fopen((dir+"world"+".info").c_str(), "rb");
        if(fileWorldInfo != nullptr){

            char* data = readData(fileWorldInfo);

            int* position = new int(0);

            world = shared_ptr<World>(new World(name));

            world->worldTime = Utility::loadNumber<unsigned long>(data, position);

            int levelCount = Utility::loadInt8Bit(data, position);

            std::string currentLevelName = Utility::loadString(data, position);
            int playerUniqueId = Utility::loadInt(data, position);


            for(int i=0;i<levelCount;i++){
                std::string levelName;

                levelName = Utility::loadString(data, position);

                //levelName.lvl
                FILE* fileLevel;

                fileLevel = std::fopen((dir+(levelName)+".lvl").c_str(), "rb");
                if(fileLevel != nullptr){

                    char* levelData = readData(fileLevel);

                    int* levelPosition = new int(0);

                    Point2 levelSize = Point2::load(levelData, levelPosition);

                    shared_ptr<Level> level = shared_ptr<Level>(new Level(levelName, levelSize));

                    level->load(levelData, levelPosition);

                    if(levelName == currentLevelName){
                        world->currentLevel = level;
                    }

                    world->levels->push_back(level);

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
                    world->currentPlayer = dynamic_pointer_cast<Player>(world->currentLevel->entityList[i]);
                }
            }

            delete position;
        }
        fclose(fileWorldInfo);
		//


        debug(world==nullptr?"Load Failed ":"Loaded");

        return world;
    }

    bool save(shared_ptr<World> loadedWorld){
        debug("Saving "+*(loadedWorld->name)+"...");
        bool failed = false;

        mkdir(UnderneathDir.c_str(), 0777);
        mkdir(WorldsDir.c_str(), 0777);
        std::string dir = WorldsDir+"/"+loadedWorld->name->c_str()+"/";
        mkdir(dir.c_str(), 0777);

        std::remove((dir+"world"+".info.backup").c_str());
        std::rename((dir+"world"+".info").c_str(),
                    (dir+"world"+".info.backup").c_str());

        //world.info
        FILE* fileWorldInfo;

        fileWorldInfo = std::fopen((dir+"world"+".info").c_str(), "wb");
        if(fileWorldInfo != nullptr){
            std::string* data = new std::string();

            Utility::saveNumber(data, loadedWorld->worldTime);

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

            for(int i=0;i<loadedWorld->levels->size();i++){
                shared_ptr<Level> l = loadedWorld->levels->at(i);

                //levelName.lvl
                FILE* fileLevel;

                std::remove((dir+(l->getName())+".lvl.backup").c_str());
                std::rename((dir+(l->getName())+".lvl").c_str(),
                            (dir+(l->getName())+".lvl.backup").c_str());

                fileLevel = fopen((dir+(l->getName())+".lvl").c_str(), "wb");
                if(fileLevel != nullptr){
                    std::string* data = new std::string();
                    

                    l->save(data);

                    for(int j=0;j<data->size();j++){
                        fputc(data->at(j), fileLevel);
                    }
                    
                    delete data;
                }else{
                    failed = true;
                    break;
                }
                fclose(fileLevel);
                //
            }

        }else{
            failed = true;
        }
        fclose(fileWorldInfo);
        //





        debug(failed?"Save Failed":"Saved");


        return false;
    }

    shared_ptr<World> create(std::string name){

        shared_ptr<World> loadedWorld = shared_ptr<World>(new World(name));

        loadedWorld->currentLevel = shared_ptr<Level>(new Level("start"));
        Point2 p = loadedWorld->currentLevel->generate(static_cast<unsigned int>(time(NULL)));
        loadedWorld->levels->push_back(loadedWorld->currentLevel);

        loadedWorld->currentPlayer = shared_ptr<Player>(new Player(name, '@', p, Ui::C_WHITE));
        loadedWorld->currentLevel->newEntity(loadedWorld->currentPlayer);

        save(loadedWorld);

        return loadedWorld;
    }

    bool deleteWorld(std::string name){
        debug("Deleting "+name+"...");

        std::string dir = WorldsDir+"/"+name+"/";

        //world.info
        FILE* fileWorldInfo;

        fileWorldInfo = std::fopen((dir+"world"+".info").c_str(), "rb");
        if(fileWorldInfo != nullptr){

            char* data = readData(fileWorldInfo);

            int* position = new int(0);

            Utility::loadNumber<unsigned long>(data, position);
            int levelCount = Utility::loadInt8Bit(data, position);
            Utility::loadString(data, position);
            Utility::loadInt(data, position);


            for(int i=0;i<levelCount;i++){
                std::string levelName;

                levelName = Utility::loadString(data, position);

                std::remove((dir+levelName+".lvl").c_str());
            }
            
            delete position;
        }
        fclose(fileWorldInfo);
        //

        std::remove((dir+"world"+".info").c_str());

        debug("Deleted");

        return true;
    }
}
