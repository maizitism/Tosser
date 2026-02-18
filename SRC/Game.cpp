#include "Game.hpp"
#include "constants.hpp"
#include "data.hpp"
#include "Math.hpp"
#include <iostream>
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
    ball.setSpawnPosition({ 750.f, 350.f });
    powerMeter.setPosition({ 900.f, 250.f });
    powerMeter.reset();

    // (alleged) Trashcan placement
    trashCan.setPosition({ 270.f, 470.f });
    trashCan.setDepthRange(
        420.f,  // closeX (easier)
        270.f,  // farX   (harder)
        395.f,  // closeY (higher/nearer in your illusion)
        470.f   // farY   (lower/farther)
    );

    trashCan.setScaleRange(
        0.36f,  // closeScale (bigger)
        0.20f   // farScale   (smaller)
    );

    trashCan.advance(score); // initial placement

    // UI
    if (!uiFont.openFromFile("ASSETS/ui.ttf")) {
        std::cout << "Font file could not be loaded. Text will not be rendered." << std::endl;
    }
    scoreText.setFont(uiFont);
    scoreText.setCharacterSize(28);
    scoreText.setPosition({ 16.f, 12.f });
    scoreText.setString("Score: 0");

    // Background
    if (!backgroundTex.loadFromFile("ASSETS/blurred_office_background.jpg")) {
        std::cerr << "Background could not be loaded.\n";
    }
    else {
        backgroundSpr.setTexture(backgroundTex, true);
        backgroundSpr.setPosition({ 0.f, 0.f });
    }

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
    const sf::FloatRect bounds = Const::ScreenBounds();
    ball.update(dt);

    if (ball.consumeJustReset()) {
        powerMeter.reset();
        scoredThisFlight = false; // ready for next throw
    }

    // --- scoring: detect ball center entering trashcan opening rect ---
    if (ball.isInFlight() && !scoredThisFlight) {
        const sf::FloatRect open = trashCan.getOpeningRect(); // SFML3 rect type
        if (open.findIntersection(ball.getBounds()).has_value()) {
            scoredThisFlight = true;
            score += 1;
            scoreText.setString("Score: " + std::to_string(score));
            trashCan.advance(score);

            // reset ball quickly after scoring
            ball.beginReset(0.35f);

            // hide trajectory
            trajectory.clear();
        }
    }

    // Only show/update trajectory while aiming (ball not in flight)
    const bool charging = isCharging();
    if (charging && !ball.isInFlight()) {
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
            bounds
        );
    }

    // On release: only throw if not already in flight
    if (wasCharging && !charging && !ball.isInFlight()) {
        const float speed = lerp(MarkerData::throwSpeedMin, MarkerData::throwSpeedMax, powerMeter.power());
        const float a = degToRad(angleDeg);

        sf::Vector2f v0{
            -std::cos(a) * speed,
            -std::sin(a) * speed
        };

        sf::Vector2f vp = ball.getPosition() + sf::Vector2f(Const::vp_x, Const::vp_y);

        ball.throwBall(v0, Const::Gravity, vp, bounds);

        // Hide the aiming line once the ball is thrown
        trajectory.clear();
    }

    if (ball.isInFlight()) {
        trajectory.clear();
    }

    wasCharging = charging;
}


void Game::render() {
    window.clear();
    window.draw(backgroundSpr);
    window.draw(trashCan);    
    window.draw(trajectory);
    window.draw(ball);
    window.draw(powerMeter);
    window.draw(scoreText);
    window.display();
}
