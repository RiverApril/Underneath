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


World::World(string n){
    levels = vector<shared_ptr<Level>>();
    this->name = n;
}

World::~World(){


}

namespace WorldLoader {

    //World* loadedWorld;

    bool exists(std::string name){
        //debug("Does "+name+" Exist?");

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

        //debug(l?"Yes ":"No ");

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

    World* load(std::string name){

        try{
            debug("Loading "+name+"...");

            std::string dir = WorldsDir+"/"+name+"/";

            World* world = nullptr;

            //world.info
            FILE* fileWorldInfo;

            fileWorldInfo = std::fopen((dir+"world"+".info").c_str(), "rb");
            if(fileWorldInfo != nullptr){

                char* data = readData(fileWorldInfo);

                int* position = new int(0);

                world = new World(name);

                world->worldTime = FileUtility::loadNumber<unsigned long>(data, position);

                int levelCount = FileUtility::loadInt8Bit(data, position);

                std::string currentLevelName = FileUtility::loadString(data, position);
                int playerUniqueId = FileUtility::loadInt(data, position);

                world->currentLevel = nullptr;

                for(int i=0;i<levelCount;i++){
                    std::string levelName;

                    levelName = FileUtility::loadString(data, position);

                    //levelName.lvl
                    FILE* fileLevel;

                    fileLevel = std::fopen((dir+(levelName)+".lvl").c_str(), "rb");
                    if(fileLevel != nullptr){

                        char* levelData = readData(fileLevel);

                        int* levelPosition = new int(0);

                        Point2 levelSize = Point2::load(levelData, levelPosition);

                        shared_ptr<Level> level = make_shared<Level>(world, levelName, levelSize);

                        level->load(levelData, levelPosition);

                        if(levelName.compare(currentLevelName) == 0){
                            world->currentLevel = level;
                            debug("Found current level: "+currentLevelName);
                        }

                        world->levels.push_back(level);

                        delete levelData;

                        delete levelPosition;

                    }

                    fclose(fileLevel);
                    //
                }

                if(world->currentLevel == nullptr && world->levels.size() > 0){
                    debug("Failed to find current level, defaulting to: "+world->levels.at(0)->getName());
                    world->currentLevel = world->levels.at(0);
                }

                if(world->currentLevel != nullptr){
                    for(int i=0;i<world->currentLevel->entityCount();i++){
                        if(world->currentLevel->entityList[i]->uniqueId == playerUniqueId){
                            world->currentPlayer = dynamic_cast<Player*>(world->currentLevel->entityList[i]);
                        }
                    }
                }

                delete data;

                delete position;
            }
            fclose(fileWorldInfo);
            //


        	debug(world==nullptr?"Load Failed":"Loaded");

        	return world;
        }catch(FileUtility::ExceptionLoad e){
            debug("Load Failed: "+e.description);
        }
        return nullptr;

    }

    bool save(World* loadedWorld){
        debug("Saving "+(loadedWorld->name)+"...");
        bool failed = false;

        mkdir(UnderneathDir.c_str(), 0777);
        mkdir(WorldsDir.c_str(), 0777);
        std::string dir = WorldsDir+"/"+loadedWorld->name.c_str()+"/";
        mkdir(dir.c_str(), 0777);

        std::remove((dir+"world"+".info.backup").c_str());
        std::rename((dir+"world"+".info").c_str(),
                    (dir+"world"+".info.backup").c_str());
        std::remove((dir+"world"+".info").c_str());

        //world.info
        FILE* fileWorldInfo;

        fileWorldInfo = std::fopen((dir+"world"+".info").c_str(), "wb");
        if(fileWorldInfo != nullptr){
            std::string* data = new std::string();

            FileUtility::saveNumber(data, loadedWorld->worldTime);

            FileUtility::saveInt8Bit(data, (int8_t)loadedWorld->levels.size());

            FileUtility::saveString(data, loadedWorld->currentLevel->getName());
            FileUtility::saveInt(data, loadedWorld->currentPlayer->uniqueId);

            for(int j=0;j<loadedWorld->levels.size();j++){
            	FileUtility::saveString(data, loadedWorld->levels.at(j)->getName());
            }

            for(int j=0;j<data->size();j++){
                fputc(data->at(j), fileWorldInfo);
            }

            delete data;

            for(int i=0;i<loadedWorld->levels.size();i++){
                shared_ptr<Level> l = loadedWorld->levels.at(i);

                //levelName.lvl
                FILE* fileLevel;

                std::remove((dir+(l->getName())+".lvl.backup").c_str());
                std::rename((dir+(l->getName())+".lvl").c_str(),
                            (dir+(l->getName())+".lvl.backup").c_str());
                std::remove((dir+(l->getName())+".lvl").c_str());

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

    World* create(std::string name){

        World* world = new World(name);

        world->currentLevel = make_shared<Level>(world, "startingFloor", Point2(300, 300));
        Point2 p = world->currentLevel->generate(static_cast<unsigned int>(time(NULL)));
        world->levels.push_back(world->currentLevel);


        world->currentPlayer = new Player(name, '@', p, Ui::C_WHITE);
        world->currentPlayer->setActiveWeapon(new Weapon(3, "Training Sword"));

        for(int i=0;i<10;i++){
        	world->currentPlayer->inventory.push_back(new Item("Test Item "+to_string(i)));
        }
        

        world->currentLevel->newEntity(world->currentPlayer);

        save(world);

        return world;
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

            FileUtility::loadNumber<unsigned long>(data, position);
            int levelCount = FileUtility::loadInt8Bit(data, position);
            FileUtility::loadString(data, position);
            FileUtility::loadInt(data, position);


            for(int i=0;i<levelCount;i++){
                std::string levelName;

                levelName = FileUtility::loadString(data, position);

                std::remove((dir+levelName+".lvl").c_str());
                std::rename((dir+levelName+".lvl.backup").c_str(), (dir+levelName+".lvl.deleted").c_str());
            }
            
            delete position;
        }
        fclose(fileWorldInfo);
        //

        std::remove((dir+"world"+".info").c_str());
        std::remove((dir+"world"+".info.backup").c_str());

        debug("Deleted");

        return true;
    }
}
