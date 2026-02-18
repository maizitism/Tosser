#pragma once
#include <SFML/Graphics/Rect.hpp>

namespace Const {
    constexpr float BallSize = 50.f;
    constexpr float PowerMeterW = 30.f;
    constexpr float PowerMeterH = 400.f;
    constexpr float PI = 3.1415926535f;
    constexpr float ScreenWidth = 960.f;
    constexpr float ScreenHeight = 540.f;

    // Runtime-tunable gameplay values (editable via Options menu)
    inline float Gravity = 980.f;
    inline float t_clamp = 0.08f;
    inline float DepthFactor = 1.5f;
    inline float vp_x = 120.f;
    inline float vp_y = -80.f;

    // Defaults for reset
    constexpr float GravityDefault = 980.f;
    constexpr float t_clampDefault = 0.08f;
    constexpr float DepthFactorDefault = 1.5f;
    constexpr float vp_xDefault = 120.f;
    constexpr float vp_yDefault = -80.f;

    inline sf::FloatRect ScreenBounds() {
        return sf::FloatRect({ 0.f, 0.f }, { ScreenWidth, ScreenHeight });
    }
}
