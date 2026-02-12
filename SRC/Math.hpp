#pragma once
#include <algorithm>

inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
};

inline float clamp01(float t) {
    return std::clamp(t, 0.f, 1.f);
}

inline float remap(float x, float inA, float inB, float outA, float outB, bool clamp = true) {
    float t = (x - inA) / (inB - inA);
    if (clamp) t = clamp01(t);
    return lerp(outA, outB, t);
}

