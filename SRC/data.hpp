#pragma once
#include "SFML/Graphics/Color.hpp"

namespace MarkerData {
    inline float power = 0.5f; // starting value
    inline float sweepSpeed = 1.5f; // power units/s
    constexpr float throwSpeedMax = 750;
    constexpr float throwSpeedMin = 350;
    inline bool charging = false;
    inline bool increasing = true;
};

namespace TrajectoryData {
    constexpr float r0 = 4.0f;
    constexpr float f = 2.0f;
};

namespace PowerMeterData {
    constexpr sf::Color c_maxforce{ 216, 27, 23 }; // red color for power meter
    constexpr sf::Color c_minforce{ 28, 124, 6 }; // green color for power meter
};