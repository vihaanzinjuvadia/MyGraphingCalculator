//
// Created by Vihaan Zinjuvadia on 4/28/23.
//

#ifndef VIHAANSGRAPHICCALC_DATAGENERATOR_H
#define VIHAANSGRAPHICCALC_DATAGENERATOR_H

#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include <limits>
#include <iostream>
#include "variables.h"
#include "Math.h"
#include "FunctionParser/Lexer.h"
#include "FunctionParser/Parser.h"

// Both generators run on their own thread, so errors get caught here: an exception
// escaping a std::thread calls std::terminate.

inline std::string right_hand_side(const std::string &function) {
    const auto equals_index = function.find('=');
    return equals_index == std::string::npos ? function : function.substr(equals_index + 1);
}

// For Advanced Functions
// f(x,y) = g(x,y). Samples the residual f - g on a grid and keeps the places where it
// changes sign, since that's where the curve passes between two samples.
inline void data_to_plot_advanced(const std::string &function,
                                  std::pair<std::vector<double>, std::vector<double>> *plotting_data) {
    // clearing plotting_data so that it regenerates data instead of just adding on and on
    plotting_data->first.clear();
    plotting_data->second.clear();

    const auto equals_index = function.find('=');
    if (equals_index == std::string::npos) {
        std::cerr << "Cannot graph \"" << function << "\": an implicit equation needs an '=' sign.\n";
        return;
    }

    try {
        // Splitting Equation into two parts
        const std::string lhs {function.substr(0, equals_index)};
        const std::string rhs {function.substr(equals_index + 1)};

        // tokenizing both parts
        Lexer lhs_lexer {lhs};
        Lexer rhs_lexer {rhs};

        // parsing tokens into a tree
        Parser lhs_parser {lhs_lexer.tokenize()};
        Parser rhs_parser {rhs_lexer.tokenize()};

        const std::unique_ptr<Node> lhs_tree {lhs_parser.parse()};
        const std::unique_ptr<Node> rhs_tree {rhs_parser.parse()};

        const int samples {NUM_POINTS_TO_SAMPLE_ADVANCED};
        if (samples < 1) {
            return;
        }
        const int stride {samples + 1};

        const double x_min {-MIDDLE_X / PIXELS_PER_UNIT_X};
        const double x_max {MIDDLE_X / PIXELS_PER_UNIT_X};
        const double y_min {-MIDDLE_Y / PIXELS_PER_UNIT_Y};
        const double y_max {MIDDLE_Y / PIXELS_PER_UNIT_Y};

        std::vector<double> residual (static_cast<std::size_t>(stride) * stride);
        for (int ix {}; ix <= samples; ix++) {
            const double x {x_min + (x_max - x_min) * ix / samples};
            for (int iy {}; iy <= samples; iy++) {
                const double y {y_min + (y_max - y_min) * iy / samples};
                residual.at(static_cast<std::size_t>(ix) * stride + iy) =
                        evaluate_tree(lhs_tree.get(), x, y) - evaluate_tree(rhs_tree.get(), x, y);
            }
        }

        // Keep a point where the residual hits zero or flips sign against the
        // neighbour to the right or above.
        for (int ix {}; ix <= samples; ix++) {
            const double x {x_min + (x_max - x_min) * ix / samples};
            for (int iy {}; iy <= samples; iy++) {
                const double here {residual.at(static_cast<std::size_t>(ix) * stride + iy)};
                if (!std::isfinite(here)) {
                    continue;
                }

                bool on_curve {here == 0.0};
                if (!on_curve && ix < samples) {
                    const double next {residual.at(static_cast<std::size_t>(ix + 1) * stride + iy)};
                    on_curve = std::isfinite(next) && ((here < 0.0) != (next < 0.0));
                }
                if (!on_curve && iy < samples) {
                    const double next {residual.at(static_cast<std::size_t>(ix) * stride + iy + 1)};
                    on_curve = std::isfinite(next) && ((here < 0.0) != (next < 0.0));
                }

                if (on_curve) {
                    const double y {y_min + (y_max - y_min) * iy / samples};
                    plotting_data->first.push_back(MIDDLE_X + (PIXELS_PER_UNIT_X * x));
                    plotting_data->second.push_back(MIDDLE_Y - (PIXELS_PER_UNIT_Y * y));
                }
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Cannot graph \"" << function << "\": " << e.what() << "\n";
    }
}

// For Simple Functions
// y = f(x). Undefined points are stored as NaN so the drawing code leaves a gap
// there instead of inventing a line.
inline void data_to_plot_simple(const std::string &function,
                                std::pair<std::vector<double>, std::vector<double>> *plotting_data) {
    // clearing plotting_data so that it regenerates data instead of just adding on and on
    plotting_data->first.clear();
    plotting_data->second.clear();

    try {
        // tokenizing and parsing the right hand side
        Lexer rhs_lexer {right_hand_side(function)};
        Parser rhs_parser {rhs_lexer.tokenize()};
        const std::unique_ptr<Node> rhs_tree {rhs_parser.parse()};

        const int samples {NUM_POINTS_TO_SAMPLE_SIMPLE};
        if (samples < 1) {
            return;
        }

        // Stepping by index instead of accumulating a float keeps the spacing exact
        // and always lands on both edges of the window.
        const double x_min {-MIDDLE_X / PIXELS_PER_UNIT_X};
        const double x_max {MIDDLE_X / PIXELS_PER_UNIT_X};

        plotting_data->first.reserve(samples + 1);
        plotting_data->second.reserve(samples + 1);

        for (int i {}; i <= samples; i++) {
            const double x {x_min + (x_max - x_min) * i / samples};
            const double value {evaluate_tree(rhs_tree.get(), x, 0)};

            // Coordinate space to pixel space.
            plotting_data->first.push_back(MIDDLE_X + (PIXELS_PER_UNIT_X * x));
            plotting_data->second.push_back(std::isfinite(value)
                                            ? MIDDLE_Y - (PIXELS_PER_UNIT_Y * value)
                                            : std::numeric_limits<double>::quiet_NaN());
        }
    } catch (const std::exception &e) {
        std::cerr << "Cannot graph \"" << function << "\": " << e.what() << "\n";
        plotting_data->first.clear();
        plotting_data->second.clear();
    }
}

// Grid Marks Stuff
inline std::pair<std::vector<double>, std::vector<double>> grid_marks_data(const sf::RenderWindow &window) {
    const auto size = window.getSize();
    int height = static_cast<int>(size.y);
    int width = static_cast<int>(size.x);

    // because i still wanna draw like -80, -70, ..., 80 and so on. Without this, I would be drawing weird grid marks.
    const int remainder_x = width % 100;
    const int remainder_y = height % 100;

    height -= remainder_y;
    width -= remainder_x;

    // grid marks on the coordinate plane
    std::vector<double> grid_mark_x {};
    std::vector<double> grid_mark_y {};

    const double end_x = (MIDDLE_X - remainder_x / 2.0) / PIXELS_PER_UNIT_X;
    const double end_y = (MIDDLE_Y - remainder_y / 2.0) / PIXELS_PER_UNIT_Y;

    // Max number of grid marks on x and y axis are 10;
    // Dividing by five so that each axis has ten gridmarks + the extra gridmark for 0.
    const double x_interval {end_x / 5.0};
    const double y_interval {end_y / 5.0};

    // A zero or non-finite interval would loop forever.
    if (!std::isfinite(x_interval) || !std::isfinite(y_interval) ||
        x_interval <= 0.0 || y_interval <= 0.0) {
        return {grid_mark_x, grid_mark_y};
    }

    for (int i {-5}; i <= 5; i++) {
        grid_mark_x.push_back(i * x_interval);
        grid_mark_y.push_back(i * y_interval);
    }

    return {grid_mark_x, grid_mark_y};
}

#endif //VIHAANSGRAPHICCALC_DATAGENERATOR_H
