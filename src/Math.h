//
// Created by Vihaan Zinjuvadia on 4/29/23.
//

#ifndef VIHAANSGRAPHICCALC_MATH_H
#define VIHAANSGRAPHICCALC_MATH_H

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

inline double distance2Points(const double x1, const double y1,
                              const double x2, const double y2) {
    return std::hypot(x2 - x1, y2 - y1);
}

// Degrees, measured the way SFML rotates shapes: clockwise, y growing downwards.
inline double angle_to_rotate_degrees(const double x1, const double y1,
                                      const double x2, const double y2) {
    return std::atan2(y2 - y1, x2 - x1) * 180.0 / M_PI;
}

#endif //VIHAANSGRAPHICCALC_MATH_H
