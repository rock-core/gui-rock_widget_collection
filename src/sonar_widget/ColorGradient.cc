#include "ColorGradient.h"
#include <algorithm>

namespace colorgradient {
// Inserts a new color point into its correct position
void ColorGradient::addColorPoint(const float red, const float green, const float blue, const float value) {
    for (uint i = 0; i < color.size(); i++) {
        if (value < color[i].val) {
            color.insert(color.begin() + i, ColorPoint(red, green, blue, value));
            return;
        }
    }
    color.push_back(ColorPoint(red, green, blue, value));
}

// Inserts a new color point into its correct position
void ColorGradient::clearGradient() {
    color.clear();
}

// Places a jet color gradient into the color vector
void ColorGradient::createJetMapGradient() {
    color.push_back(ColorPoint(0, 0, 1, 0.0f));      // blue
    color.push_back(ColorPoint(0, 1, 1, 0.25f));     // cyan
    color.push_back(ColorPoint(0, 1, 0, 0.5f));      // green
    color.push_back(ColorPoint(1, 1, 0, 0.75f));     // yellow
    color.push_back(ColorPoint(1, 0, 0, 1.0f));      // red
}

// Places a hot color gradient into the color vector
void ColorGradient::createHotMapGradient() {
    color.push_back(ColorPoint(0, 0, 0, 0.0f));     // black
    color.push_back(ColorPoint(1, 0, 0, 0.125f));   // red
    color.push_back(ColorPoint(1, 1, 0, 0.55f));    // yellow
    color.push_back(ColorPoint(1, 1, 1, 1.0f));     // white
}

// Places a grayscale color gradient into the color vector
void ColorGradient::createGrayscaleMapGradient() {
    color.push_back(ColorPoint(0, 0, 0, 0.0f));     // black
    color.push_back(ColorPoint(1, 1, 1, 1.0f));     // white
}

// Inputs a (value) between 0 and 1 and outputs the (red), (green) and (blue)
// values representing that position in the gradient
void ColorGradient::getColorAtValue(const float value, float &red, float &green, float &blue) const {
    if (color.empty())
        throw std::out_of_range("ERROR: There is no color in the current palette.");

    for (int i = 0; i < color.size(); i++) {
        ColorPoint currC = color[i];
        if (value < currC.val) {
            ColorPoint prevC = color[std::max(0, i - 1)];
            float valueDiff = (prevC.val - currC.val);
            float fractBetween = (valueDiff == 0) ? 0 : (value - currC.val) / valueDiff;
            red = (prevC.r - currC.r) * fractBetween + currC.r;
            green = (prevC.g - currC.g) * fractBetween + currC.g;
            blue = (prevC.b - currC.b) * fractBetween + currC.b;
            return;
        }
    }
    red = color.back().r;
    green = color.back().g;
    blue = color.back().b;
    return;
}

}
