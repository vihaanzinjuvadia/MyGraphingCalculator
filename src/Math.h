//
// Created by Vihaan Zinjuvadia on 4/29/23.
//

#ifndef VIHAANSGRAPHICCALC_MATH_H
#define VIHAANSGRAPHICCALC_MATH_H

#include <cmath>

double distance2Points(const std::vector<double> &point1, const std::vector<double> &point2) {
    return sqrt(pow(point2.at(0) - point1.at(0), 2) +
                pow(point2.at(1) - point1.at(1), 2));
}
auto angle_to_rotate_degrees(const std::vector<double> &point1, const std::vector<double> &point2) {
    auto dx{point2.at(0) - point1.at(0)};
    auto dy{point2.at(1) - point1.at(1)};

    if (dx > 0 && dy == 0) { // directly to the right
        return 0.0;
    } else if (dx < 0 && dy == 0) {  // directly to the left
        return 180.0;
    } else if (dx == 0 && dy < 0) { // directly up
        return 270.0;
    } else if (dx == 0 && dy > 0) { // directly down
        return 90.0;
    } else if (dx < 0 && dy < 0) { // left and up
        return 180.0 + atan(dy/dx) * 180.0/M_PI;
    } else if (dx > 0 && dy < 0) { // right and up
        return 180.0/M_PI * atan(dy/dx);
    } else if (dx > 0 && dy > 0) { // right and down
        return atan(dy/dx) * 180/M_PI;
    } else if (dx < 0 && dy > 0) { // left and down
        return 90.0 + atan(-dx/dy) * 180/M_PI;
    }
    return 0.0;
}

#endif //VIHAANSGRAPHICCALC_MATH_H
