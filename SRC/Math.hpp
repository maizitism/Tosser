#pragma once
#include <algorithm>

inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
};

inline float smoothstep(float x) {
    x = std::clamp(x, 0.f, 1.f);
    return x * x * (3.f - 2.f * x);
}