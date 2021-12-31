//
//  Audio.hpp
//  Underneath
//
//  Created by Emily Atlee on 1/11/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__Audio__
#define __Underneath__Audio__

#define FADE_TIME 2000

namespace Audio {

    struct Sound;

    extern vector<Sound*> soundList;

    extern Sound* soundMenu;
    extern Sound* soundTheme;


    void initAudio();
    void cleanupAudio();

    void update();

    Sound* loadAudioFile(string path);

    bool setBgMusic(Sound* s);



}

#endif /* __Underneath__Audio__ */
