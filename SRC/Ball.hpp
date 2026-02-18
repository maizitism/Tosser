#pragma once
#include <SFML/Graphics.hpp>

class Ball : public sf::Drawable {
public:
    Ball();

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

    void setSpawnPosition(sf::Vector2f pos);

    void throwBall(sf::Vector2f vel0,
        float gravity,
        sf::Vector2f vanishingPoint,
        sf::FloatRect bounds);

    bool isInFlight() const { return state_ == State::InFlight; }
    void sinkInto(sf::Vector2f target,
        float sinkTime = 0.18f,
        float waitTime = 0.35f,
        float scaleFactor = 0.55f);


    void beginReset(float delaySeconds = 0.5f);
    bool consumeJustReset();
    void update(float dt);

private:
    enum class State {
        Ready,      // sitting at spawn
        InFlight,   // actively simulating
        Sinking,    // sink into trashcan
        Resetting   // waiting resetDelay, then snap back to spawn
    };

    // Visuals
    sf::Texture texture;
    sf::Sprite  sprite;
    sf::Vector2f baseScale{ 1.f, 1.f };

    // State
    State state_ = State::Ready;

    // Sink animation data
    sf::Vector2f sinkStartPos{};
    sf::Vector2f sinkTargetPos{};
    sf::Vector2f sinkStartScale{};
    sf::Vector2f sinkTargetScale{};
    float sinkT = 0.f;
    float sinkDuration = 0.18f;
    float sinkWait = 0.35f;

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

    // Reset info
    bool justReset_ = false;
private:
    float perspective(float time) const;
    bool isReady() const { return state_ == State::Ready; }

    void resetToSpawn();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
