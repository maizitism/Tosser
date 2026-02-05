#pragma once
#include <SFML/Graphics.hpp>
#include "GradientBar.hpp"

class PowerMeter : public sf::Drawable {
public:
    PowerMeter();

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;

    void update(float dt);
    float power() const;

    void reset();

private:
    enum class SweepDirection {
        Up,
        Down
    };

    GradientBar meter;
    sf::RectangleShape marker;

    float power01 = 0.5f;
    SweepDirection direction = SweepDirection::Up;

    void updateMarkerPosition();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};