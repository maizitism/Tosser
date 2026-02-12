#include "Trashcan.hpp"
#include <iostream>

TrashCan::TrashCan() : texture(), sprite(texture) {
    if (!texture.loadFromFile("ASSETS/trashcan.png")) {
        std::cerr << "Failed to load trash can texture (ASSETS/trashcan.png)\n";
    }

    sprite = sf::Sprite(texture);

    // Set default origin
    const sf::Vector2u s = texture.getSize();
    sprite.setOrigin({ s.x * 0.5f, (float)s.y });
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
