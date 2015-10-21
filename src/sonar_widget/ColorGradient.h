/*
 * ColorGradient.hpp
 *
 *  Created on: Oct 21, 2015
 *      Author: romulo
 */

#include <vector>

class ColorGradient {

    struct ColorPoint {     // Internal class used to store colors at different points in the gradient.
        float r, g, b;      // Red, green and blue values of our color.
        float val;          // Position of our color along the gradient (between 0 and 1).
        ColorPoint(float red, float green, float blue, float value) :
                r(red), g(green), b(blue), val(value) {
        }
    };

private:
    std::vector<ColorPoint> color;      // An array of color points in ascending value.

public:
    ColorGradient()  { }
    void addColorPoint(float red, float green, float blue, float value);
    void clearGradient();
    void createDefaultHeatMapGradient();
    void getColorAtValue(const float value, float &red, float &green, float &blue);
};
