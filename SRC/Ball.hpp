#pragma once
#include <SFML/Graphics.hpp>

class Ball : public sf::Drawable {
public:
    Ball();

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;

    void setSpawnPosition(sf::Vector2f pos);

    void throwBall(sf::Vector2f vel0,
        float gravity,
        sf::Vector2f vanishingPoint,
        sf::FloatRect bounds);

    void update(float dt);

private:
    enum class State {
        Ready,      // sitting at spawn
        InFlight,   // actively simulating
        Resetting   // waiting resetDelay, then snap back to spawn
    };

    // Visuals
    sf::Texture texture;
    sf::Sprite  sprite;
    sf::Vector2f baseScale{ 1.f, 1.f };

    // State
    State state_ = State::Ready;

    // Throw simulation data
    sf::FloatRect bounds{};
    float t = 0.f;
    float g = 0.f;
    sf::Vector2f p0{};
    sf::Vector2f v0{};
    sf::Vector2f vp{};

    // Reset timer
    float resetTimer = 0.f;
    float resetDelay = 2.f;
    sf::Vector2f spawnPos{};

private:
    float perspective(float time) const;
    bool isReady() const { return state_ == State::Ready; }
    bool isInFlight() const { return state_ == State::InFlight; }

    void resetToSpawn();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
