//
//  Audio.cpp
//  Underneath
//
//  Created by Braeden Atlee on 1/11/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//


#include "Global.hpp"
#include "Ui.hpp"
#include "Audio.hpp"


namespace Audio{

    vector<Sound*> soundList;

    Sound* soundMenu;
    Sound* soundTheme;

}


#ifdef useYSEAudio
#define implementedAudio

#include <yse.hpp>

namespace Audio{


    struct Sound{
        YSE::sound yseSound;
    };

    Sound* bgMusic = nullptr;


    void initAudio(){
        YSE::System().init();

        soundMenu = loadAudioFile(AudioDir+"/menu.ogg");
        soundTheme = loadAudioFile(AudioDir+"/theme.ogg");
    }

    void cleanupAudio(){
        YSE::System().close();
    }

    void update(){
        YSE::System().update();
    }

    Sound* loadAudioFile(string path){
        Sound* s = new Sound();
        s->yseSound.create(path.c_str(), NULL, true);
        if (!s->yseSound.isValid()) {
            debugf("Failed to load sound file: %s", path.c_str());
            delete s;
            return nullptr;
        }
        soundList.push_back(s);
        return s;
    }

    void playSound(Sound* s){
        if(s){
        	s->yseSound.play();
        }
    }

    void stopSound(Sound* s){
        if(s){
        	s->yseSound.stop();
        }
    }

    void setBgMusic(Sound* s){
        if(s != bgMusic){
            if(bgMusic && bgMusic->yseSound.isPlaying()){
                bgMusic->yseSound.fadeAndStop(2000);
            }
            bgMusic = s;
            bgMusic->yseSound.setTime(0);
            bgMusic->yseSound.play();
            bgMusic->yseSound.setVolume(0, 0);
            bgMusic->yseSound.setVolume(1, 2000);
        }
    }
}


#endif



#ifndef implementedAudio

namespace Audio{
    
    struct Sound{};


    void initAudio(){
        debugf("Sound unimplemented, sound cannot be initalized.");
    }

    void cleanupAudio(){}

    void update(){}

    Sound* loadAudioFile(string path){
        debugf("Sound unimplemented, sound cannot be loaded.");
        return nullptr;
    }

    void playSound(Sound* s){
        debugf("Sound unimplemented, sound cannot be played.");
    }

    void stopSound(Sound* s){
        debugf("Sound unimplemented, sound cannot be stopped.");
    }

    void fadeOut(Sound* s, int time){}

    void fadeIn(Sound* s, int time){}
}

#endif

