#pragma once
#include <SFML/Graphics.hpp>
#include "GradientBar.hpp"

class PowerMeter : public sf::Drawable {
public:
    PowerMeter();

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;

    // Call while "charging"
    void update(float dt);

    // Current power 0..1
    float power() const;

private:
    GradientBar meter;
    sf::RectangleShape marker;

    float power01 = 0.f;
    bool increasing = true;

    void updateMarkerPosition();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
