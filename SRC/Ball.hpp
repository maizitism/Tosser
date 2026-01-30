#pragma once
#include <SFML/Graphics.hpp>

class Ball : public sf::Drawable {
public:
    Ball();

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;

private:
    sf::Texture texture;
    sf::Sprite sprite;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
