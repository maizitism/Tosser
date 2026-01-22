#pragma once
#include "SFML/Graphics/Color.hpp"

struct MarkerData {
    float power = 0.5f;
    float sweepSpeed = 1.5f; // power units/s
    bool charging = false;
    bool increasing = true;
};

struct PowerMeterData {
    sf::Color c_maxforce{ 216, 27, 23 }; // red color for power meter
    sf::Color c_minforce{ 28, 124, 6 }; // green color for power meter
};