#pragma once
#include <SFML/Graphics.hpp>
#include "PowerMeter.hpp"
#include "Ball.hpp"
#include "Trajectory.hpp"

class Game {
public:
    int run();

private:
    sf::RenderWindow window{ sf::VideoMode({960, 540}), "SFML works!" };
    sf::Clock clock;

    Ball ball;
    PowerMeter powerMeter;
    Trajectory trajectory;

    float angleDeg = 45.f;
    float wasCharging = false;

    void initLayout();
    void processEvents();
    void update(float dt);
    void render();

    bool isCharging() const;
};
