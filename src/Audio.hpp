//
//  Audio.hpp
//  Underneath
//
//  Created by Braeden Atlee on 1/11/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Audio__
#define __Underneath__Audio__

namespace Audio {

    struct Sound;

    extern vector<Sound*> soundList;

    extern Sound* soundMenu;
    extern Sound* soundTheme;


    void initAudio();
    void cleanupAudio();

    void update();

    Sound* loadAudioFile(string path);

    bool playSound(Sound* s);
    bool stopSound(Sound* s);

    bool setBgMusic(Sound* s);



}

#endif /* __Underneath__Audio__ */
