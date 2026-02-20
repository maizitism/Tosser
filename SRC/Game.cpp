#include "Game.hpp"
#include "constants.hpp"
#include "data.hpp"
#include "Math.hpp"
#include <iostream>
#include <optional>
#include <cmath>

int Game::run() {
    window.setFramerateLimit(120);

    // Load font once
    if (!uiFont.openFromFile("ASSETS/ui.ttf")) {
        std::cout << "Font file could not be loaded. Text will not be rendered.\n";
    }

    if (!backgroundTex.loadFromFile("ASSETS/blurred_office_background.jpg")) {
        std::cerr << "Background could not be loaded.\n";
    }
    else {
        backgroundSpr.setTexture(backgroundTex, true);
        backgroundSpr.setPosition({ 0.f, 0.f });
    }

    // Create UI
    ui = std::make_unique<UI>(window, uiFont);
    ui->setCallbacks(UI::Callbacks{
        .onStart = [&]() {
            // reset gameplay state & init layout
            score = 0;
            scoredThisFlight = false;
            advanceAfterReset = false;
            trajectory.clear();

            initLayout();             
            ui->setScene(UI::Scene::Playing);
        },
        .onExit = [&]() { window.close(); },
        .onBackToMenu = [&]() { },
        .onResetDefaults = [&]() {
            Const::Gravity = Const::GravityDefault;
            Const::DepthFactor = Const::DepthFactorDefault;
            Const::t_clamp = Const::t_clampDefault;
            Const::vp_x = Const::vp_xDefault;
            Const::vp_y = Const::vp_yDefault;
        }
        });

    audio.init();
    audio.startMusic();

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
        395.f,  // closeY (higher/nearer)
        470.f   // farY   (lower/farther)
    );

    trashCan.setScaleRange(
        0.36f,  // closeScale (bigger)
        0.20f   // farScale   (smaller)
    );

    trashCan.advance(score); // initial placement

    scoreText.setFont(uiFont);
    scoreText.setCharacterSize(28);
    scoreText.setPosition({ 16.f, 12.f });
    scoreText.setString("Score: 0");

    throwsLeft = maxThrows;

    throwsText.setFont(uiFont);
    throwsText.setCharacterSize(28);
    throwsText.setOutlineThickness(2.f);
    throwsText.setOutlineColor(sf::Color(0, 0, 0, 200));
    throwsText.setFillColor(sf::Color(255, 255, 255, 235));
    throwsText.setPosition({ 16.f, 76.f });
    throwsText.setString("Throws: " + std::to_string(throwsLeft));

    // Lives
    lives = maxLives;
    throwInProgress = false;
    lastThrowScored = false;
    gameOver = false;

    livesText.setFont(uiFont);
    livesText.setCharacterSize(28);
    livesText.setPosition({ 16.f, 44.f });
    livesText.setString("Lives: " + std::to_string(lives));

    gameOverText.setFont(uiFont);
    gameOverText.setCharacterSize(64);
    gameOverText.setString("GAME OVER");
    {
        auto b = gameOverText.getLocalBounds();
        gameOverText.setOrigin({ b.position.x + b.size.x * 0.5f, b.position.y + b.size.y * 0.5f });
        gameOverText.setPosition({ Const::ScreenWidth * 0.5f, Const::ScreenHeight * 0.45f });
    }

}

void Game::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            continue;
        }

        // Esc in Playing returns to menu
        if (ui && ui->getScene() == UI::Scene::Playing) {
            if (event->is<sf::Event::KeyPressed>()) {
                if (auto* kp = event->getIf<sf::Event::KeyPressed>()) {
                    if (kp->code == sf::Keyboard::Key::Escape) {
                        ui->setScene(UI::Scene::Menu);
                        continue;
                    }
                }
            }
        }

        // Let UI handle inputs when not playing
        if (ui && (ui->getScene() == UI::Scene::Menu || ui->getScene() == UI::Scene::Options)) {
            ui->handleEvent(*event);
        }
    }
}

bool Game::isCharging() const {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
}

void Game::update(float dt) {
    if (ui && ui->getScene() != UI::Scene::Playing) {
        ui->update(dt);
        return;
    }
    
    if (gameOver) {
        // Press Enter or Space to go back to menu
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            ui->setScene(UI::Scene::Menu);
        }
        return; // freeze gameplay
    }

    const sf::FloatRect bounds = Const::ScreenBounds();
    ball.update(dt);
    if (!gameOver) trashCan.update(dt);

    if (ball.consumeJustReset()) {
        powerMeter.reset();

        // If a throw ended and it wasn't scored -> lose a life
        if (throwInProgress && !lastThrowScored) {
            audio.playMiss();
            lives = std::max(0, lives - 1);
            livesText.setString("Lives: " + std::to_string(lives));

            if (lives == 0) {
                gameOver = true;
                trajectory.clear();
            }
        }

        // Clear throw state at reset
        throwInProgress = false;
        lastThrowScored = false;

        scoredThisFlight = false;

        if (advanceAfterReset) {
            trashCan.advance(score);
            advanceAfterReset = false;
        }

        if (throwsLeft == 0 && !gameOver) {
            gameOver = true;
            trajectory.clear();
        }
    }

    // --- scoring: detect ball center entering trashcan opening rect ---
    if (ball.isInFlight() && !scoredThisFlight) {
        const sf::FloatRect open = trashCan.getOpeningRect(); // SFML3 rect type
        if (open.findIntersection(ball.getBounds()).has_value()) {
            scoredThisFlight = true;
            lastThrowScored = true;
            audio.playScore();
            score += 1;
            trashCan.setWobble(lerp(6.f, 24.f, std::clamp(score / 20.f, 0.f, 1.f)), 2.2f);
            scoreText.setString("Score: " + std::to_string(score));

            // Defer trashcan movement until the ball actually resets
            advanceAfterReset = true;

            // Sink target: center of opening + a small downward push
            const sf::FloatRect openRect = trashCan.getOpeningRect();
            sf::Vector2f sinkTarget{
                openRect.position.x + openRect.size.x * 0.5f,
                openRect.position.y + openRect.size.y * 0.65f  // a bit down inside the opening
            };

            // Sink (0.18s), then wait (0.35s), then reset
            ball.sinkInto(sinkTarget, 0.18f, 0.35f, 0.55f);


            // hide trajectory
            trajectory.clear();
        }
    }

    // Only show/update trajectory while aiming (ball not in flight)
    const bool charging = isCharging();
    if (charging && ball.isReady() && !gameOver) {
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
    if (wasCharging && !charging && ball.isReady() && !gameOver) {
        const float speed = lerp(MarkerData::throwSpeedMin, MarkerData::throwSpeedMax, powerMeter.power());
        const float a = degToRad(angleDeg);

        sf::Vector2f v0{
            -std::cos(a) * speed,
            -std::sin(a) * speed
        };

        sf::Vector2f vp = ball.getPosition() + sf::Vector2f(Const::vp_x, Const::vp_y);

        ball.throwBall(v0, Const::Gravity, Const::WindAccelX, vp, bounds);
        throwsLeft = std::max(0, throwsLeft - 1);
        throwsText.setString("Throws: " + std::to_string(throwsLeft));
        
        throwInProgress = true;
        lastThrowScored = false;

        // Hide the aiming line once the ball is thrown
        trajectory.clear();
    }

    if (!ball.isReady()) {
        trajectory.clear();
    }

    wasCharging = charging;
}


void Game::render() {
    window.clear();
    window.draw(backgroundSpr);

    if (ui && (ui->getScene() == UI::Scene::Menu || ui->getScene() == UI::Scene::Options)) {
        ui->render(window);
        window.display();
        return;
    }

    // Playing render (your current render order)
    window.draw(trashCan);
    window.draw(trajectory);
    window.draw(ball);
    window.draw(powerMeter);
    window.draw(scoreText);
    window.draw(livesText);
    window.draw(throwsText);

    if (gameOver) {
        window.draw(gameOverText);
    }

    window.display();
}
