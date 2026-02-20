#include "Trashcan.hpp"
#include "Math.hpp"
#include "Constants.hpp"   
#include <iostream>
#include <algorithm>


TrashCan::TrashCan() : texture(), sprite(texture) {
    if (!texture.loadFromFile("ASSETS/trashcan.png")) {
        std::cerr << "Failed to load trash can texture (ASSETS/trashcan.png)\n";
    }

    sprite = sf::Sprite(texture);

    // Set default origin
    const sf::Vector2u s = texture.getSize();
    sprite.setOrigin({ s.x * 0.5f, static_cast<float>(s.y) });
}

void TrashCan::setPosition(sf::Vector2f pos) {
    sprite.setPosition(pos);
}

sf::Vector2f TrashCan::getPosition() const {
    return sprite.getPosition();
}

sf::FloatRect TrashCan::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::FloatRect TrashCan::getOpeningRect() const {
    sf::FloatRect b = sprite.getGlobalBounds();

    sf::FloatRect r;
    r.position.x = b.position.x + b.size.x * openingLeftPct;
    r.position.y = b.position.y + b.size.y * openingTopPct;
    r.size.x = b.size.x * openingWidthPct;
    r.size.y = b.size.y * openingHeightPct;

    return r;
}

void TrashCan::setScale(float s) {
    sprite.setScale({ s, s });
}

void TrashCan::setDepthRange(float closeX, float farX, float closeY, float farY) {
    closeX_ = closeX; farX_ = farX;
    closeY_ = closeY; farY_ = farY;
}

void TrashCan::setScaleRange(float closeScale, float farScale) {
    closeScale_ = closeScale;
    farScale_ = farScale;

    float kFar = farScale_ / closeScale_;
    kFar = std::max(kFar, Const::t_clamp); // respect the same clamp as the ball

    tMax_ = (1.f / kFar - 1.f) / Const::DepthFactor;
    tMax_ = std::max(tMax_, 0.f);
}


float TrashCan::perspective(float t) const {
    float k = 1.f / (1.f + t * Const::DepthFactor);
    return std::max(k, Const::t_clamp);
}

void TrashCan::advance(int score) {
    // 0 -> close (easy), 1 -> far (hard)
    float t = std::clamp(static_cast<float>(score) / static_cast<float>(rampScore_), 0.f, 1.f);

    // Move away as score grows (position still lerped)
    float x = lerp(closeX_, farX_, t);
    float y = lerp(closeY_, farY_, t);

    // Scale uses the same k-law as the ball
    float depthT = lerp(0.f, tMax_, t);  // score -> depth time
    float k = perspective(depthT);
    float s = closeScale_ * k;

    float jxAmt = lerp(jitterX_ * 0.35f, jitterX_, t);
    float jyAmt = lerp(jitterY_ * 0.35f, jitterY_, t);

    std::uniform_real_distribution<float> jx(-jxAmt, jxAmt);
    std::uniform_real_distribution<float> jy(-jyAmt, jyAmt);

    x += jx(rng_);
    y += jy(rng_);

    // Clamp within the close<->far corridor so it never jumps wildly
    const float xMin = std::min(closeX_, farX_);
    const float xMax = std::max(closeX_, farX_);
    const float yMin = std::min(closeY_, farY_);
    const float yMax = std::max(closeY_, farY_);

    x = std::clamp(x, xMin, xMax);
    y = std::clamp(y, yMin, yMax);

    basePos_ = { x, y };
    setPosition(basePos_);
    setScale(s);
}

void TrashCan::setWobble(float ampX, float speed) {
    wobbleAmpX_ = ampX;
    wobbleSpeed_ = speed;
}

void TrashCan::update(float dt) {
    wobbleT_ += dt;

    // Sine wobble around basePos_
    const float dx = std::sin(wobbleT_ * wobbleSpeed_) * wobbleAmpX_;
    sprite.setPosition(basePos_ + sf::Vector2f(dx, 0.f));
}

void TrashCan::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);

    // debug stuff
    sf::RectangleShape dbg;
    auto r = getOpeningRect();
    dbg.setPosition(r.position);
    dbg.setSize(r.size);
    dbg.setFillColor(sf::Color(255, 0, 0, 80));
    target.draw(dbg, states);
    
}
