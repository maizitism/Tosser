#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

#include "data.hpp"
#include "constants.hpp"

class GradientBar : public sf::Drawable, public sf::Transformable {
public:
    GradientBar(sf::Vector2f size, sf::Color top, sf::Color bottom)
        : vertices(sf::PrimitiveType::TriangleStrip, 4) {
            
        vertices[0].position = { 0.f, 0.f };
        vertices[1].position = { size.x, 0.f };
        vertices[2].position = { 0.f, size.y };
        vertices[3].position = { size.x, size.y };

        setColors(top, bottom);
    }
    void setColors(sf::Color top, sf::Color bottom)
    {
        vertices[0].color = top;
        vertices[2].color = bottom;
        vertices[1].color = top;
        vertices[3].color = bottom;
    }

private:
    sf::VertexArray vertices;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(vertices, states);
    }

};

void advancePowerMarker(sf::RectangleShape& marker, GradientBar& powermeter, MarkerData& md, const float dt) {
    // Sweep md.power between 0 and 1
    if (md.increasing)
        md.power += md.sweepSpeed * dt;   // "units per second"
    else
        md.power -= md.sweepSpeed * dt;

    // Bounce at the ends
    if (md.power >= 1.f) { md.power = 1.f; md.increasing = false; }
    if (md.power <= 0.f) { md.power = 0.f; md.increasing = true; }

    // Convert power -> marker position over the meter
    const sf::Vector2f meterCenter = powermeter.getPosition();
    const float meterTop = meterCenter.y - (Const::PowerMeterH * 0.5f);

    // power=1 at top, power=0 at bottom
    const float y = meterTop + (1.f - md.power) * Const::PowerMeterH;
    const float x = meterCenter.x;

    marker.setPosition({ x, y });
}

static float lin_interp(float a, float b, float t) { return a + (b - a) * t; }

sf::VertexArray buildTrajectory(sf::Vector2f p0, sf::Vector2f v0,
    float gravity, float tMax, float dt,
    sf::FloatRect bounds) {

    sf::VertexArray line(sf::PrimitiveType::LineStrip);
    sf::Vector2f g{ 0.f, gravity };

    for (float t = 0.f; t <= tMax; t += dt) {
        sf::Vector2f p = p0 + v0 * t + 0.5f * g * (t * t);

        if (!bounds.contains(p)) break;
        line.append(sf::Vertex(p));
    }
    return line;
}



int main()
{
    // -- SETUP --
    sf::RenderWindow window(sf::VideoMode({ 960, 540 }), "SFML works!");
    window.setFramerateLimit(120);
  
    // variables
    sf::Clock clock;

    MarkerData markerdata;
    PowerMeterData meterData;

    // shapes
    sf::CircleShape paperball(Const::BallSize);
    GradientBar powermeter({Const::PowerMeterW, Const::PowerMeterH},
        meterData.c_maxforce, meterData.c_minforce);
    sf::RectangleShape powermarker({ Const::PowerMeterW + 15.f, 5.f });
    sf::VertexArray trajectory; // trajectory object
    

    // layout
    // paper ball
    paperball.setFillColor(sf::Color::White);
    paperball.setPosition({ 750.f, 350.f });
    paperball.setOrigin({ Const::BallSize / 2, Const::BallSize / 2 });
    // power meter
    powermeter.setOrigin({ Const::PowerMeterW / 2, Const::PowerMeterH / 2 });
    powermeter.setPosition({ 900.f, 250.f });
    // power meter indicator
    powermarker.setOrigin({ (Const::PowerMeterW + 15.f) * 0.5f, 5.f * 0.5f });
    powermarker.setFillColor(sf::Color::White);
    // set up trajectory object
    // pick arbitrary angle
    float a = 30.f;
    float a_rad = a * Const::PI / 180.f;



    /*sf::Vector2f v = paperball.getPosition();
    std::cout << "x " << static_cast<float>(v.x) << "y" << static_cast<float>(v.y) << std::endl;*/

    // display power marker immediately
    advancePowerMarker(powermarker, powermeter, markerdata, 0.f);

    // -- MAIN LOOP --
    while (window.isOpen())
    {   
        // time step
        float dt = clock.restart().asSeconds();

        // events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();            
        }

        // inputs
        bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

        // update
        if (spaceDown) {
            advancePowerMarker(powermarker, powermeter, markerdata, dt);
            // compute speed value at which ball would get thrown from markerdata.power
            float speed = lin_interp(markerdata.throwSpeedMin, markerdata.throwSpeedMax, markerdata.power);
            // compute new v0 based on markerdata.power
            sf::Vector2f v0{
                -std::cos(a_rad) * speed,
                -std::sin(a_rad) * speed
            };
            
            trajectory = buildTrajectory(
                paperball.getPosition(),
                v0,
                Const::Gravity,
                3.f,
                dt,
                sf::FloatRect({ 0.f, 0.f }, { 960.f, 540.f })
            );

        }



        // first poll for spacebar input
        // while held, that starts a sweeping meter which between a MAXFORCE and a MINFORCE
        // when released, ball gets thrown with appropriate velocity
        // compute trajectory and let ball travel along it, while slowly getting smaller, simulating a z axis
        // AABB for trashcan
        // if hit, count score or something idk
        // if not, continue its trajectory
        // when OOB, gets reset

        // happy days
       

        // render
        window.clear();
        window.draw(paperball);
        window.draw(powermeter);
        window.draw(powermarker);
        window.draw(trajectory);
        window.display();

    }
    std::cout << "Goodbye cruel world." << std::endl;
    return 0;
}