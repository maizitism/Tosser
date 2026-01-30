#include "Ball.hpp"
#include "constants.hpp"

Ball::Ball() {
    shape.setRadius(Const::BallSize);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin({ Const::BallSize * 0.5f, Const::BallSize * 0.5f });
}

void Ball::setPosition(sf::Vector2f pos) {
    shape.setPosition(pos);
}

sf::Vector2f Ball::getPosition() const {
    return shape.getPosition();
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(shape, states);
}
