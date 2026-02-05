#include "Game.hpp"
#include "constants.hpp"
#include "data.hpp"
#include "Math.hpp"
#include <optional>
#include <cmath>

static float degToRad(float deg) {
    return deg * Const::PI / 180.f;
}

int Game::run() {
    window.setFramerateLimit(120);

    initLayout();

    while (window.isOpen()) {
        const float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
    return 0;
}

void Game::initLayout() {
    ball.setPosition({ 750.f, 350.f });

    powerMeter.setPosition({ 900.f, 250.f });

    // ensure initial marker placement (already handled, but harmless)
    powerMeter.update(0.f);
}

void Game::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) window.close();
    }
}

bool Game::isCharging() const {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
}

void Game::update(float dt) {
    ball.update(dt);

    const bool charging = isCharging();

    if (charging) {
        powerMeter.update(dt);

        const float speed = lerp(MarkerData::throwSpeedMin, MarkerData::throwSpeedMax, powerMeter.power());
        const float a = degToRad(angleDeg);

        sf::Vector2f v0{
            -std::cos(a) * speed,
            -std::sin(a) * speed
        };

        trajectory.rebuild(
            ball.getPosition(),
            v0,
            Const::Gravity,
            3.f,
            0.035f,
            sf::FloatRect({ 0.f, 0.f }, { 960.f, 540.f })
        );
    }

    if (wasCharging && !charging) {
        const float speed = lerp(MarkerData::throwSpeedMin, MarkerData::throwSpeedMax, powerMeter.power());
        const float a = degToRad(angleDeg);

        sf::Vector2f v0{
            -std::cos(a) * speed,
            -std::sin(a) * speed
        };

        sf::Vector2f vp = ball.getPosition() + sf::Vector2f(Const::vp_x, Const::vp_y);

        ball.throwBall(v0, Const::Gravity, vp);
    }

    wasCharging = charging;
}

void Game::render() {
    window.clear();
    window.draw(ball);
    window.draw(powerMeter);
    window.draw(trajectory);
    window.display();
}
