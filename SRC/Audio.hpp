#pragma once
#include <SFML/Audio.hpp>
#include <string>

class Audio {
public:
    bool init();                // loads files
    void startMusic();           // plays looping background
    void stopMusic();

    void setMusicVolume(float v); // 0..100
    void setSfxVolume(float v);   // 0..100

    void playScore();
    void playMiss();

private:
    // Music
    sf::Music music;
    float musicVol = 35.f;

    // SFX
    sf::SoundBuffer bufScore;
    sf::SoundBuffer bufMiss;

    sf::Sound sClick;
    sf::Sound sScore;
    sf::Sound sMiss;

    float sfxVol = 70.f;
};