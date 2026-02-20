#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <memory>
#include "PowerMeter.hpp"
#include "Ball.hpp"
#include "Trajectory.hpp"
#include "Constants.hpp"
#include "Trashcan.hpp"
#include "UI.hpp"
#include "Audio.hpp"


class Game {
public:
    int run();

private:
    std::unique_ptr<UI> ui;
    Audio audio;

    sf::RenderWindow window{
    sf::VideoMode({ (unsigned)Const::ScreenWidth,
                    (unsigned)Const::ScreenHeight }),
    "Tosser by Marks Janis Maizitis. Intake project for PR2026",
    sf::Style::Titlebar | sf::Style::Close
    };
    sf::Clock clock;

    Ball ball;
    PowerMeter powerMeter;
    Trajectory trajectory;
    TrashCan trashCan;

    int score = 0;
    bool scoredThisFlight = false;
    bool advanceAfterReset = false;

    sf::Font uiFont;
    sf::Text scoreText { uiFont };

    bool gameOver = false;
    sf::Text finalScoreText{ uiFont };

    // Wind system
    sf::Text windText{ uiFont };
    float windAx = 0.f;          // current wind acceleration (px/s^2)
    int throwsTaken = 0;         // total throws made this round
    std::mt19937 windRng{ std::random_device{}() };
    std::uniform_real_distribution<float> windDist{ -200.f, 200.f };

    // Arcade "fixed throw" system
    int maxThrows = 20;
    int throwsLeft = 20;
    sf::Text throwsText{ uiFont };

    sf::Text gameOverText{ uiFont };

    sf::Texture backgroundTex;
    sf::Sprite  backgroundSpr {backgroundTex};

    float angleDeg = 45.f;
    bool wasCharging = false;

    void initLayout();
    void processEvents();
    void update(float dt);
    void render();

    bool isCharging() const;
};
