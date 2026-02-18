#pragma once
#include <SFML/Graphics.hpp>
#include <random>

class TrashCan : public sf::Drawable {
public:
    TrashCan();

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;

    sf::FloatRect getBounds() const;

    sf::FloatRect getOpeningRect() const;

    void setScale(float s);

    void setDepthRange(float closeX, float farX, float closeY, float farY);
    void setScaleRange(float closeScale, float farScale);
    void advance(int score);

private:
    sf::Texture texture;
    sf::Sprite sprite;

    // arbitrary opening definition
    float openingLeftPct = 0.22f;
    float openingTopPct = 0.08f;
    float openingWidthPct = 0.56f;
    float openingHeightPct = 0.18f;

    float closeX_ = 420.f, farX_ = 270.f;
    float closeY_ = 395.f, farY_ = 470.f;
    float closeScale_ = 0.36f, farScale_ = 0.20f;
    int   rampScore_ = 12;
    float jitterX_ = 35.f;
    float jitterY_ = 18.f;

    // stuff for scaling the same as the ball
    float tMax_ = 0.f;
    float perspective(float t) const;



    std::mt19937 rng_{ std::random_device{}() };

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
