#pragma once
#include <SFML/Graphics.hpp>
#include "PowerMeter.hpp"
#include "Ball.hpp"
#include "Trajectory.hpp"
#include "Constants.hpp"
#include "Trashcan.hpp"
#include "UI.hpp"

#include <memory>


class Game {
public:
    int run();

private:
    std::unique_ptr<UI> ui;

    sf::RenderWindow window{
    sf::VideoMode({ (unsigned)Const::ScreenWidth,
                    (unsigned)Const::ScreenHeight }),
    "SFML works!"
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
