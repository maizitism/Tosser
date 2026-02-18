#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <string>

class UI {
public:
    enum class Scene { Menu, Options, Playing };

    struct Callbacks {
        std::function<void()> onStart;
        std::function<void()> onExit;
        std::function<void()> onBackToMenu;     // from Options
        std::function<void()> onResetDefaults;  // from Options
    };

    UI(sf::RenderWindow& window, sf::Font& font);

    void setCallbacks(Callbacks cb);
    void setScene(Scene s);
    Scene getScene() const { return scene_; }

    // Event handling and rendering for UI scenes
    void handleEvent(const sf::Event& e);
    void update(float dt);
    void render(sf::RenderTarget& target);


    void setTitle(const std::string& title);
    void setHintMenu(const std::string& hint);
    void setHintOptions(const std::string& hint);

private:
    struct Button {
        sf::RectangleShape box;
        sf::Text text;
        std::function<void()> onClick;
        bool hovered = false;

        Button(sf::Font& font) : text(font) {}

        bool contains(sf::Vector2f p) const {
            return box.getGlobalBounds().contains(p);
        }
    };


    struct Slider {
        sf::Text label;
        sf::RectangleShape track;
        sf::RectangleShape knob;
        sf::Text valueText;

        float* value = nullptr;
        float minV = 0.f;
        float maxV = 1.f;
        float step = 0.f;
        bool dragging = false;

        bool editing = false;
        std::string editBuf;

        Slider(sf::Font& font) : label(font), valueText(font) {}

        bool hitKnob(sf::Vector2f p) const { return knob.getGlobalBounds().contains(p); }
        bool hitTrack(sf::Vector2f p) const { return track.getGlobalBounds().contains(p); }
        bool hitValue(sf::Vector2f p) const { return valueText.getGlobalBounds().contains(p); }
    };


    sf::RenderWindow& window_;
    sf::Font& font_;
    Scene scene_ = Scene::Menu;
    Callbacks cb_{};

    sf::Text title_;
    sf::Text hint_;

    std::vector<Button> menuButtons_;
    std::vector<Button> optionsButtons_;
    std::vector<Slider> sliders_;

    // internals
    sf::Vector2f mousePos_{};

    void buildMenu();
    void buildOptions();

    void handleMenuEvent(const sf::Event& e);
    void handleOptionsEvent(const sf::Event& e);

    void updateMenu(float dt);
    void updateOptions(float dt);

    void renderMenu(sf::RenderTarget& target);
    void renderOptions(sf::RenderTarget& target);

    // utilities
    sf::Vector2f mouseWorld() const;
    void updateButtonHover(std::vector<Button>& buttons);
    void drawButtons(sf::RenderTarget& target, const std::vector<Button>& buttons);

    void sliderSetFromMouse(Slider& s, float mx);
    void sliderSyncVisuals(Slider& s);
    void slidersSyncAll();

    static std::string fmtFloat(float v);
    static bool isNumberChar(char c);
};
