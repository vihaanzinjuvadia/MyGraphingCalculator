//
// Created by Vihaan Zinjuvadia on 4/28/23.
//

#ifndef VIHAANSGRAPHICCALC_DATAGENERATOR_H
#define VIHAANSGRAPHICCALC_DATAGENERATOR_H

#include <vector>
#include "variables.h"
#include "Math.h"
#include "FunctionParser/Lexer.h"
#include "FunctionParser/Parser.h"


// For Advanced Functions
void data_to_plot_advanced(const std::string &function,
                           std::pair<std::vector<double>, std::vector<double>> *plotting_data) {
    // clearing plotting_data so that it regenerates data instead of just adding on and on
    plotting_data->first.clear();
    plotting_data->second.clear();

    auto delta = 1;

    // Setting Variable
    variable1 = 'x';
    variable2 = 'y';

    // Splitting Equation into two parts
    auto equals_index = function.find('=');
    std::string lhs {function.substr(0, equals_index)};
    std::string rhs {function.substr(equals_index+1, function.size() - equals_index - 1)};

    // tokenizing both parts
    Lexer lhs_lexer {lhs};
    Lexer rhs_lexer {rhs};
    auto lhs_tokens {lhs_lexer.tokenize()};
    auto rhs_tokens {rhs_lexer.tokenize()};

    // parsing tokens into a tree
    //Maybe tree generation and evaluation takes a long time.
    Parser lhs_parser {lhs_tokens};
    Parser rhs_parser {rhs_tokens};

    Node* lhs_tree {lhs_parser.parse()};
    Node* rhs_tree {rhs_parser.parse()};

    // Have a set number of points to sample, like 500 or smthn, and then figure out a dynamic line interval
    double generate_line_interval_x {(2 * (MIDDLE_X/PIXELS_PER_UNIT_X)) / NUM_POINTS_TO_SAMPLE};
    double generate_line_interval_y {(2 * (MIDDLE_Y/PIXELS_PER_UNIT_Y)) / NUM_POINTS_TO_SAMPLE};

    for (double x {-MIDDLE_X/PIXELS_PER_UNIT_X}; x <= MIDDLE_X/PIXELS_PER_UNIT_X; x += generate_line_interval_x) {
        for (double y {-MIDDLE_Y/PIXELS_PER_UNIT_Y}; y <= MIDDLE_Y/PIXELS_PER_UNIT_Y; y += generate_line_interval_y) {


            // evaluating tree
            evaluate_tree(lhs_tree, x, y);
            evaluate_tree(rhs_tree, x, y);

            auto lhs_evaluation {*lhs_tree->numeric_value};
            auto rhs_evaluation {*rhs_tree->numeric_value};
            lhs_tree->numeric_value = nullptr;
            rhs_tree->numeric_value = nullptr;

            if (lhs_evaluation <= rhs_evaluation + delta && lhs_evaluation >= rhs_evaluation - delta) {
                plotting_data->first.push_back(MIDDLE_X + (PIXELS_PER_UNIT_X * x));
                plotting_data->second.push_back(MIDDLE_Y - (PIXELS_PER_UNIT_Y * y));
            }
        }
    }
}

// For Simple Functions
void data_to_plot_simple(const std::string function,
                         std::pair<std::vector<double>, std::vector<double>> *plotting_data) {
    // clearing plotting_data so that it regenerates data instead of just adding on and on
    plotting_data->first.clear();
    plotting_data->second.clear();

    variable1 = 'x';

    // To make sure that the resolution of the graph stays the same and doesn't look broken; figure out dynamically based on screen resolution and stuff
    double generate_line_interval_x{(2 * (MIDDLE_X / PIXELS_PER_UNIT_X)) / NUM_POINTS_TO_SAMPLE};

    // Splitting Equation into two parts
    auto equals_index = function.find('=');
    std::string rhs{function.substr(equals_index + 1, function.size() - equals_index - 1)};

    // tokenizing both parts
    Lexer rhs_lexer{rhs};
    auto rhs_tokens{rhs_lexer.tokenize()};

    // First, scaling the coordinates to the size it should be given the current view, and then translating it from coordinate to pixel space.
    // parsing tokens into a tree
    Parser rhs_parser{rhs_tokens};
    Node *rhs_tree{rhs_parser.parse()};

    for (double x{-MIDDLE_X / PIXELS_PER_UNIT_X};
         x <= MIDDLE_X / PIXELS_PER_UNIT_X + 1; x += generate_line_interval_x) {
        // evaluating tree
        evaluate_tree(rhs_tree, x, 0);

        auto rhs_evaluation{*rhs_tree->numeric_value};
        rhs_tree->numeric_value = nullptr;

        auto x_val = MIDDLE_X + (PIXELS_PER_UNIT_X * x);
        auto y_val = MIDDLE_Y - (PIXELS_PER_UNIT_Y * rhs_evaluation);
        plotting_data->first.push_back(x_val);
        plotting_data->second.push_back(y_val);
    }
}

// Grid Marks Stuff
std::pair<std::vector<double>, std::vector<double>> grid_marks_data(const sf::RenderWindow &window) {
    auto size = window.getSize();
    int height = size.y;
    int width = size.x;

    // because i still wanna draw like -80, -70, ..., 80 and so on. Without this, I would be drawing weird grid marks.
    int remainder_x = width % 100;
    int remainder_y = height % 100;

    height -= remainder_y;
    width -= remainder_x;

    // grid marks on the coordinate plane
    std::vector<double> grid_mark_x {};
    std::vector<double> grid_mark_y {};

    double start_x = -(MIDDLE_X - remainder_x/2.0) / PIXELS_PER_UNIT_X;
    double end_x = (MIDDLE_X - remainder_x/2.0) / PIXELS_PER_UNIT_X;

    double start_y = -(MIDDLE_Y - remainder_y/2.0) / PIXELS_PER_UNIT_Y;
    double end_y = (MIDDLE_Y - remainder_y/2.0) / PIXELS_PER_UNIT_Y;


    // Max number of grid marks on x and y axis are 10;
    double x_interval {}, y_interval {};
    x_interval = end_x/5.0; // Dividing by five so that each axis has ten gridmarks + the extra gridmark for 0.
    y_interval = end_y/5.0;

    for (double mark {start_x}; mark <= end_x; mark+=x_interval) {
        grid_mark_x.push_back(mark);
    }
    for (double mark {start_y}; mark <= end_y; mark+=y_interval) {
        grid_mark_y.push_back(mark);
    }

    return {grid_mark_x, grid_mark_y};
}

#endif //VIHAANSGRAPHICCALC_DATAGENERATOR_H
