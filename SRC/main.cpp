#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

#define BALLSIZE 50.f
#define POWERMETERX 30.f
#define POWERMETERY 400.f
#define GRAVITY 980.f

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

void advancePowerMarker(sf::RectangleShape& marker, MarkerData& markerData, const float dt) {

}


int main()
{
    // initialise window
    sf::RenderWindow window(sf::VideoMode({ 960, 540 }), "SFML works!");
    
    // initialise all colors
    sf::Color maxforce(216, 27, 23); // red color for power meter
    sf::Color minforce(28, 124, 6); // green color for power meter

    // initialise all shapes
    sf::CircleShape paperball(BALLSIZE);
    GradientBar powermeter({POWERMETERX, POWERMETERY}, maxforce, minforce);
    sf::RectangleShape powermarker({ POWERMETERX + 15.f, 5.f });


    // set initial layout of everything
    // paper ball
    paperball.setFillColor(sf::Color::White);
    paperball.setOrigin({ BALLSIZE / 2, BALLSIZE / 2 });
    paperball.setPosition({ 750.f, 350.f });
    // power meter
    powermeter.setPosition({ 900.f, 70.f });
    // power meter indicator
    powermarker.setOrigin({ (POWERMETERX + 15) / 2, 7.5f });
    powermarker.setFillColor(sf::Color::White);


    // initialise time variable
    sf::Clock clock;
    // initialise struct for values around power marker
    struct MarkerData {
        float power = 0.0f;
        float sweepspeed = 15.f; // px/s
        bool charging = false;
        bool increasing = false;
    };

    MarkerData markerdata = { 0, false, false };
    
    while (window.isOpen())
    {   
        float dt = clock.restart().asSeconds();

        // -- EVENTS --
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                advancePowerMarker(powermarker, markerdata, dt);
            }
        }


        // first poll for spacebar input
        // while held, that starts a sweeping meter which between a MAXFORCE and a MINFORCE
        // when released, ball gets thrown with appropriate velocity
        // when OOB, gets reset
       

        // -- RENDER --
        window.clear();
        window.draw(paperball);
        window.draw(powermeter);
        window.draw(powermarker);
        window.display();

    }
}