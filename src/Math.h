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
    auto x_change {point2.at(0) - point1.at(0)};
    auto y_change {point2.at(1) - point1.at(1)};

    if (x_change == 0 && y_change < 0) { // directly below
        return 270.0;
    } else if (x_change == 0 && y_change > 0) { // directly below
        return 90.0;
    } else if (y_change == 0 && x_change < 0) { // directly left
        return 180.0;
    } else if (y_change == 0 && x_change > 0) { // directly right
        return 0.0;
    } else if (x_change < 0 && y_change < 0) {  // left and up
        return 360 - 90 - atan(x_change/y_change) * 180/M_PI;
    } else if (x_change < 0 && y_change > 0) {  // left and down
        return 90 + atan(-x_change/y_change) * 180/M_PI;
    }
    // for if the next point is to the left of the current point
    return atan(y_change/x_change) * 180/M_PI;
}

#endif //VIHAANSGRAPHICCALC_MATH_H
