#include <iostream>
#include <algorithm>
#include <cmath>
#include "Ball.hpp"
#include "constants.hpp"

Ball::Ball() : texture(), sprite(texture){
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
    sprite.setScale({ scale, scale });
}

void Ball::setPosition(sf::Vector2f pos) {
    sprite.setPosition(pos);
}

sf::Vector2f Ball::getPosition() const {
    return sprite.getPosition();
}

float Ball::perspective(float time) const {
    const float depthFactor = Const::DepthFactor;
    const float power = 2.0f;

    float k = 1.f / (1.f + std::pow(time, power) * depthFactor);
    return std::max(k, 0.05f);
}

void Ball::throwBall(sf::Vector2f vel0, float gravity, sf::Vector2f vanishingPoint) {
    p0 = sprite.getPosition();
    v0 = vel0;
    g = gravity;
    vp = vanishingPoint;
    
    t = 0.f;
    inFlight = true;

    sf::Vector2f scale = sprite.getScale();
    baseScale = { scale.x, scale.y };
    sprite.setScale(baseScale);
}

void Ball::update(float dt) {
    if (!inFlight) return;

    t += dt;

    const sf::Vector2f gvec{ 0.f, g };
    const sf::Vector2f p = p0 + v0 * t + 0.5f * gvec * (t * t);

    // Match Trajectory::rebuild EXACTLY (same k law + same clamp)
    const float depthFactor = Const::DepthFactor;

    float k = 1.f / (1.f + t * depthFactor);
    k = std::max(k, 0.08f);

    const sf::Vector2f pp = vp + (p - vp) * k;

    sprite.setPosition(pp);
    sprite.setScale({ baseScale.x * k, baseScale.y * k });
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}
