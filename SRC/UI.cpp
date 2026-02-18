#include "UI.hpp"
#include "constants.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>

UI::UI(sf::RenderWindow& window, sf::Font& font)
    : window_(window)
    , font_(font)
    , title_(font_)
    , hint_(font_) {

    title_.setFont(font_);
    title_.setCharacterSize(56);
    title_.setPosition({ 48.f, 38.f });
    title_.setString("Tosser");

    hint_.setFont(font_);
    hint_.setCharacterSize(18);
    hint_.setPosition({ 48.f, 110.f });

    buildMenu();
    buildOptions();
    setScene(Scene::Menu);
}

void UI::setCallbacks(Callbacks cb) { cb_ = std::move(cb); }

void UI::setScene(Scene s) {
    scene_ = s;

    // clear any edit/drag state when switching scenes
    for (auto& sl : sliders_) {
        sl.dragging = false;
        sl.editing = false;
        sl.editBuf.clear();
        sliderSyncVisuals(sl);
    }

    if (scene_ == Scene::Menu) {
        hint_.setString("Press Start to play. Options to tune physics.");
    }
    else if (scene_ == Scene::Options) {
        hint_.setString("Drag sliders or click the value to type. Enter to apply. Esc cancels.");
    }
}

void UI::setTitle(const std::string& title) { title_.setString(title); }
void UI::setHintMenu(const std::string& hint) {
    if (scene_ == Scene::Menu) hint_.setString(hint);
}
void UI::setHintOptions(const std::string& hint) {
    if (scene_ == Scene::Options) hint_.setString(hint);
}

sf::Vector2f UI::mouseWorld() const {
    auto p = sf::Mouse::getPosition(window_);
    return window_.mapPixelToCoords(p);
}

std::string UI::fmtFloat(float v) {
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.3f", v);
    return std::string(buf);
}

bool UI::isNumberChar(char c) {
    return (c >= '0' && c <= '9') || c == '.' || c == '-';
}

void UI::buildMenu() {
    menuButtons_.clear();

    auto makeBtn = [&](std::string label, sf::Vector2f pos, sf::Vector2f size, std::function<void()> onClick) {
        Button b(font_);
        b.box.setPosition(pos);
        b.box.setSize(size);
        b.box.setFillColor(sf::Color(0, 0, 0, 140));
        b.box.setOutlineThickness(2.f);
        b.box.setOutlineColor(sf::Color(255, 255, 255, 60));

        b.text.setFont(font_);
        b.text.setCharacterSize(28);
        b.text.setString(label);

        auto tb = b.text.getLocalBounds();
        b.text.setOrigin({ tb.position.x + tb.size.x * 0.5f, tb.position.y + tb.size.y * 0.5f });
        b.text.setPosition({ pos.x + size.x * 0.5f, pos.y + size.y * 0.5f });

        b.onClick = std::move(onClick);
        menuButtons_.push_back(std::move(b));
        };

    const sf::Vector2f btnSize{ 260.f, 60.f };
    const float x = 48.f;
    float y = 170.f;

    makeBtn("Start", { x, y }, btnSize, [&]() { if (cb_.onStart) cb_.onStart(); });
    y += 78.f;
    makeBtn("Options", { x, y }, btnSize, [&]() { setScene(Scene::Options); });
    y += 78.f;
    makeBtn("Exit", { x, y }, btnSize, [&]() { if (cb_.onExit) cb_.onExit(); });
}

void UI::buildOptions() {
    optionsButtons_.clear();
    sliders_.clear();

    auto makeBtn = [&](std::string label, sf::Vector2f pos, sf::Vector2f size, std::function<void()> onClick) {
        Button b(font_);
        b.box.setPosition(pos);
        b.box.setSize(size);
        b.box.setFillColor(sf::Color(0, 0, 0, 140));
        b.box.setOutlineThickness(2.f);
        b.box.setOutlineColor(sf::Color(255, 255, 255, 60));

        b.text.setFont(font_);
        b.text.setCharacterSize(22);
        b.text.setString(label);

        auto tb = b.text.getLocalBounds();
        b.text.setOrigin({ tb.position.x + tb.size.x * 0.5f, tb.position.y + tb.size.y * 0.5f });
        b.text.setPosition({ pos.x + size.x * 0.5f, pos.y + size.y * 0.5f });

        b.onClick = std::move(onClick);
        optionsButtons_.push_back(std::move(b));
        };

    const float left = 48.f;
    float y = 160.f;

    auto makeSlider = [&](std::string name, float* value, float minV, float maxV, float step) {
        Slider s(font_);
        s.value = value;
        s.minV = minV;
        s.maxV = maxV;
        s.step = step;

        s.label.setFont(font_);
        s.label.setCharacterSize(18);
        s.label.setString(name);
        s.label.setPosition({ left, y });

        s.track.setPosition({ left, y + 30.f });
        s.track.setSize({ 360.f, 6.f });
        s.track.setFillColor(sf::Color(255, 255, 255, 80));

        s.knob.setSize({ 14.f, 22.f });
        s.knob.setOrigin({ 7.f, 11.f });
        s.knob.setFillColor(sf::Color(0, 0, 0, 200));
        s.knob.setOutlineThickness(2.f);
        s.knob.setOutlineColor(sf::Color(255, 255, 255, 80));

        s.valueText.setFont(font_);
        s.valueText.setCharacterSize(18);
        s.valueText.setFillColor(sf::Color(255, 255, 255, 220));
        s.valueText.setPosition({ left + 390.f, y + 18.f });

        sliders_.push_back(std::move(s));
        y += 72.f;
        };

    makeSlider("Gravity", &Const::Gravity, 100.f, 2500.f, 10.f);
    makeSlider("DepthFactor", &Const::DepthFactor, 0.1f, 6.0f, 0.05f);
    makeSlider("t_clamp", &Const::t_clamp, 0.01f, 0.30f, 0.005f);
    makeSlider("vp_x", &Const::vp_x, -400.f, 400.f, 1.f);
    makeSlider("vp_y", &Const::vp_y, -400.f, 400.f, 1.f);

    makeBtn("Back", { left, 470.f }, { 140.f, 48.f }, [&]() {
        setScene(Scene::Menu);
        if (cb_.onBackToMenu) cb_.onBackToMenu();
        });

    makeBtn("Reset Defaults", { left + 160.f, 470.f }, { 200.f, 48.f }, [&]() {
        if (cb_.onResetDefaults) cb_.onResetDefaults();
        slidersSyncAll();
        });

    slidersSyncAll();
}

void UI::updateButtonHover(std::vector<Button>& buttons) {
    mousePos_ = mouseWorld();
    for (auto& b : buttons) {
        bool h = b.contains(mousePos_);
        if (h != b.hovered) {
            b.hovered = h;
            if (b.hovered) {
                b.box.setFillColor(sf::Color(0, 0, 0, 190));
                b.box.setOutlineColor(sf::Color(255, 255, 255, 130));
            }
            else {
                b.box.setFillColor(sf::Color(0, 0, 0, 140));
                b.box.setOutlineColor(sf::Color(255, 255, 255, 60));
            }
        }
    }
}

void UI::drawButtons(sf::RenderTarget& target, const std::vector<Button>& buttons) {
    for (const auto& b : buttons) {
        target.draw(b.box);
        target.draw(b.text);
    }
}

void UI::sliderSetFromMouse(Slider& s, float mx) {
    auto tr = s.track.getGlobalBounds();
    float u = (mx - tr.position.x) / tr.size.x;
    u = std::clamp(u, 0.f, 1.f);

    float v = s.minV + (s.maxV - s.minV) * u;
    if (s.step > 0.f) v = std::round(v / s.step) * s.step;
    v = std::clamp(v, s.minV, s.maxV);
    *s.value = v;
}

void UI::sliderSyncVisuals(Slider& s) {
    auto tr = s.track.getGlobalBounds();
    float u = (*s.value - s.minV) / (s.maxV - s.minV);
    u = std::clamp(u, 0.f, 1.f);

    float x = tr.position.x + u * tr.size.x;
    float y = tr.position.y + tr.size.y * 0.5f;
    s.knob.setPosition({ x, y });

    if (!s.editing) s.valueText.setString(fmtFloat(*s.value));
    else s.valueText.setString(s.editBuf + "_");
}

void UI::slidersSyncAll() {
    for (auto& s : sliders_) sliderSyncVisuals(s);
}

void UI::handleMenuEvent(const sf::Event& e) {
    if (e.is<sf::Event::MouseMoved>()) {
        updateButtonHover(menuButtons_);
    }
    if (e.is<sf::Event::MouseButtonPressed>()) {
        const auto* mb = e.getIf<sf::Event::MouseButtonPressed>();
        if (mb && mb->button == sf::Mouse::Button::Left) {
            mousePos_ = mouseWorld();
            for (auto& b : menuButtons_) {
                if (b.contains(mousePos_) && b.onClick) {
                    b.onClick();
                    return;
                }
            }
        }
    }
}

void UI::handleOptionsEvent(const sf::Event& e) {
    if (e.is<sf::Event::MouseMoved>()) {
        updateButtonHover(optionsButtons_);

        mousePos_ = mouseWorld();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            for (auto& s : sliders_) {
                if (s.dragging) {
                    sliderSetFromMouse(s, mousePos_.x);
                    sliderSyncVisuals(s);
                    return;
                }
            }
        }
    }

    if (e.is<sf::Event::MouseButtonPressed>()) {
        const auto* mb = e.getIf<sf::Event::MouseButtonPressed>();
        if (mb && mb->button == sf::Mouse::Button::Left) {
            mousePos_ = mouseWorld();

            for (auto& b : optionsButtons_) {
                if (b.contains(mousePos_) && b.onClick) {
                    b.onClick();
                    return;
                }
            }

            for (auto& s : sliders_) {
                if (s.hitKnob(mousePos_) || s.hitTrack(mousePos_)) {
                    for (auto& other : sliders_) { other.editing = false; other.editBuf.clear(); }
                    s.dragging = true;
                    sliderSetFromMouse(s, mousePos_.x);
                    sliderSyncVisuals(s);
                    return;
                }
                if (s.hitValue(mousePos_)) {
                    for (auto& other : sliders_) { other.dragging = false; other.editing = false; other.editBuf.clear(); }
                    s.editing = true;
                    s.editBuf = fmtFloat(*s.value);
                    sliderSyncVisuals(s);
                    return;
                }
            }
        }
    }

    if (e.is<sf::Event::MouseButtonReleased>()) {
        const auto* mb = e.getIf<sf::Event::MouseButtonReleased>();
        if (mb && mb->button == sf::Mouse::Button::Left) {
            for (auto& s : sliders_) s.dragging = false;
        }
    }

    if (e.is<sf::Event::TextEntered>()) {
        const auto* te = e.getIf<sf::Event::TextEntered>();
        if (!te) return;

        if (te->unicode == 8) { // backspace
            for (auto& s : sliders_) {
                if (s.editing && !s.editBuf.empty()) {
                    s.editBuf.pop_back();
                    sliderSyncVisuals(s);
                    return;
                }
            }
        }

        if (te->unicode > 127) return;
        char c = static_cast<char>(te->unicode);

        for (auto& s : sliders_) {
            if (s.editing) {
                if (isNumberChar(c)) {
                    s.editBuf.push_back(c);
                    sliderSyncVisuals(s);
                }
                return;
            }
        }
    }

    if (e.is<sf::Event::KeyPressed>()) {
        const auto* kp = e.getIf<sf::Event::KeyPressed>();
        if (!kp) return;

        if (kp->code == sf::Keyboard::Key::Escape) {
            for (auto& s : sliders_) {
                if (s.editing) {
                    s.editing = false;
                    s.editBuf.clear();
                    sliderSyncVisuals(s);
                    return;
                }
            }
            setScene(Scene::Menu);
            if (cb_.onBackToMenu) cb_.onBackToMenu();
            return;
        }

        if (kp->code == sf::Keyboard::Key::Enter) {
            for (auto& s : sliders_) {
                if (!s.editing) continue;

                try {
                    float v = std::stof(s.editBuf);
                    v = std::clamp(v, s.minV, s.maxV);
                    if (s.step > 0.f) v = std::round(v / s.step) * s.step;
                    *s.value = v;
                }
                catch (...) { /* ignore */ }

                s.editing = false;
                s.editBuf.clear();
                sliderSyncVisuals(s);
                return;
            }
        }
    }
}

void UI::handleEvent(const sf::Event& e) {
    if (scene_ == Scene::Menu) handleMenuEvent(e);
    else if (scene_ == Scene::Options) handleOptionsEvent(e);
}

void UI::update(float dt) {
    if (scene_ == Scene::Menu) updateMenu(dt);
    else if (scene_ == Scene::Options) updateOptions(dt);
}

void UI::updateMenu(float) { updateButtonHover(menuButtons_); }
void UI::updateOptions(float) { updateButtonHover(optionsButtons_); slidersSyncAll(); }

void UI::render(sf::RenderTarget& target) {
    // UI only renders in Menu / Options
    if (scene_ == Scene::Menu) renderMenu(target);
    else if (scene_ == Scene::Options) renderOptions(target);
}

void UI::renderMenu(sf::RenderTarget& target) {
    target.draw(title_);
    target.draw(hint_);
    drawButtons(target, menuButtons_);
}

void UI::renderOptions(sf::RenderTarget& target) {
    target.draw(title_);
    target.draw(hint_);

    for (auto& s : sliders_) {
        target.draw(s.label);
        target.draw(s.track);
        target.draw(s.knob);
        target.draw(s.valueText);
    }
    drawButtons(target, optionsButtons_);
}
