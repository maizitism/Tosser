#include <iostream>
#include <algorithm>
#include <cmath>
#include "Ball.hpp"
#include "constants.hpp"

Ball::Ball() : texture(), sprite(texture) {
    if (!texture.loadFromFile("ASSETS/paper_ball.png")) {
        std::cerr << "Failed to load paperball\n";
    }

    sprite = sf::Sprite(texture);

    // Center origin
    const sf::Vector2u size = texture.getSize();
    sprite.setOrigin({ size.x * 0.5f, size.y * 0.5f });

    // Scale sprite to match your old BallSize
    float desiredDiameter = Const::BallSize * 2.f;
    float scale = desiredDiameter / size.x;
    baseScale = { scale, scale };
    sprite.setScale({ scale, scale });
}

void Ball::setPosition(sf::Vector2f pos) {
    sprite.setPosition(pos);
}

sf::Vector2f Ball::getPosition() const {
    return sprite.getPosition();
}

void Ball::setSpawnPosition(sf::Vector2f pos) {
    spawnPos = pos;
    sprite.setPosition(pos);
    sprite.setScale(baseScale);

    state_ = State::Ready;
    resetTimer = 0.f;
    t = 0.f;
}

float Ball::perspective(float time) const {
    float k = 1.f / (1.f + time * Const::DepthFactor);
    return std::max(k, Const::t_clamp);
}

void Ball::throwBall(sf::Vector2f vel0,
    float gravity,
    sf::Vector2f vanishingPoint,
    sf::FloatRect b) {
    // Only allow throw from Ready
    if (!isReady()) return;

    p0 = sprite.getPosition();
    bounds = b;
    v0 = vel0;
    g = gravity;
    vp = vanishingPoint;

    t = 0.f;
    state_ = State::InFlight;
}


void Ball::resetToSpawn() {
    sprite.setPosition(spawnPos);
    sprite.setScale(baseScale);

    state_ = State::Ready;
    resetTimer = 0.f;
    t = 0.f;

    justReset_ = true;
}

bool Ball::consumeJustReset() {
    if (justReset_) {
        justReset_ = false;
        return true;
    }
    return false;
}

void Ball::update(float dt) {
    switch (state_) {
    case State::Ready:
        // Nothing to do
        return;

    case State::Resetting:
        resetTimer += dt;
        if (resetTimer >= resetDelay) {
            resetToSpawn();
        }
        return;

    case State::InFlight:
        break; // simulate below
    }

    // --- InFlight simulation ---
    t += dt;

    const sf::Vector2f gvec{ 0.f, g };
    const sf::Vector2f p = p0 + v0 * t + 0.5f * gvec * (t * t);

    float k = perspective(t);
    const sf::Vector2f pp = vp + (p - vp) * k;

    if (!bounds.contains(pp)) {
        state_ = State::Resetting;
        resetTimer = 0.f;
        return;
    }

    sprite.setPosition(pp);
    sprite.setScale({ baseScale.x * k, baseScale.y * k });
}


void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}
