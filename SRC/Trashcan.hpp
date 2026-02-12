#pragma once
#include <SFML/Graphics.hpp>

class TrashCan : public sf::Drawable {
public:
    TrashCan();

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;

    sf::FloatRect getBounds() const;

    sf::FloatRect getOpeningRect() const;

    void setScale(float s);

private:
    sf::Texture texture;
    sf::Sprite sprite;

    // arbitrary opening definition
    float openingLeftPct = 0.22f;
    float openingTopPct = 0.08f;
    float openingWidthPct = 0.56f;
    float openingHeightPct = 0.18f;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
