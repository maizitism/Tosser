#include "Audio.hpp"
#include <iostream>

bool Audio::init() {
    const std::string musicPath = "ASSETS/audio/menu.mp3";
    const std::string scorePath = "ASSETS/audio/score.wav";
    const std::string missPath = "ASSETS/audio/miss.wav";

    if (!music.openFromFile(musicPath)) {
        std::cerr << "Failed to load music: " << musicPath << "\n";
        // not fatal
    }
    else {
        music.setLooping(true);
        music.setVolume(musicVol);
    }

    auto loadBuf = [](sf::SoundBuffer& b, const std::string& p) {
        if (!b.loadFromFile(p)) {
            std::cerr << "Failed to load sfx: " << p << "\n";
            return false;
        }
        return true;
        };

    bool ok = true;
    ok &= loadBuf(bufScore, scorePath);
    ok &= loadBuf(bufMiss, missPath);

    sClick.setBuffer(bufClick);
    sScore.setBuffer(bufScore);
    sMiss.setBuffer(bufMiss);

    sClick.setVolume(sfxVol);
    sScore.setVolume(sfxVol);
    sMiss.setVolume(sfxVol);

    return ok;
}

void Audio::startMusic() {
    if (music.getStatus() != sf::SoundSource::Status::Playing) {
        music.setVolume(musicVol);
        music.play();
    }
}

void Audio::stopMusic() {
    if (music.getStatus() == sf::SoundSource::Status::Playing) {
        music.stop();
    }
}

void Audio::setMusicVolume(float v) {
    musicVol = std::clamp(v, 0.f, 100.f);
    music.setVolume(musicVol);
}

void Audio::setSfxVolume(float v) {
    sfxVol = std::clamp(v, 0.f, 100.f);
    sClick.setVolume(sfxVol);
    sScore.setVolume(sfxVol);
    sMiss.setVolume(sfxVol);
}

void Audio::playClick() { sClick.play(); }
void Audio::playScore() { sScore.play(); }
void Audio::playMiss() { sMiss.play(); }