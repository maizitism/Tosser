#pragma once
#include <SFML/Graphics.hpp>

class Ball : public sf::Drawable {
public:
    Ball();

    void setPosition(sf::Vector2f pos);
    void throwBall(sf::Vector2f vel0,
        float gravity,
        sf::Vector2f vanishingPoint,
        sf::FloatRect bounds);
    
    void update(float dt);

    sf::Vector2f getPosition() const;

private:
    sf::FloatRect bounds{};
    sf::Texture texture;
    sf::Sprite sprite;

    bool inFlight = false;
    float t = 0.f;
    float g = 0.f;
    sf::Vector2f p0{};
    sf::Vector2f v0{};
    sf::Vector2f vp{};

    sf::Vector2f baseScale{ 1.f, 1.f };
    float perspective(float time) const;
    bool isInFlight() const { return inFlight; };

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
