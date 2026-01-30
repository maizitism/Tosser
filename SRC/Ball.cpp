#include <iostream>
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

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}
