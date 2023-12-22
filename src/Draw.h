//
// Created by Vihaan Zinjuvadia on 4/29/23.
//

#ifndef VIHAANSGRAPHICCALC_DRAW_H
#define VIHAANSGRAPHICCALC_DRAW_H

#include "DataGenerator.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "Math.h"

void draw_line_plot_advanced(sf::RenderWindow &window,
                             const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                             const int color);
void draw_line_plot_advanced(sf::RenderWindow &window,
                             const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                             const sf::Color &color);
void draw_axes(sf::RenderWindow &window);
void draw_arrows(sf::RenderWindow &window);
void label_axes(sf::RenderWindow &window,
                const sf::Font &font);
void draw_grid_marks_and_grid_text(sf::RenderWindow &window,
                                   const sf::Font &font);
bool check_if_in_excludes_list(const std::pair<std::vector<double>, std::vector<double>> &excludes_list,
                               const std::pair<double, double> point);

std::pair<double, int> find_closest_point(const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                                          const std::pair<std::vector<double>, std::vector<double>> &exclude_list,
                                          int i,
                                          const std::vector<double> &x_vals,
                                          const std::vector<double> &y_vals);

// SIMPLE FUNCTIONS //
void draw_line_plot_simple(sf::RenderWindow &window,
                           const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                           const sf::Color color) {
    const auto line_width {5};

    for (int i {}; i < plotting_data.first.size() - 1; i++) {
        auto line_height = distance2Points({plotting_data.first.at(i),
                                                             plotting_data.second.at(i)},
                                                    {plotting_data.first.at(i+1),
                                                             plotting_data.second.at(i+1)});
        auto line {sf::RectangleShape(sf::Vector2f(line_height, line_width))};
        line.setFillColor(color);
        line.setPosition(window.mapPixelToCoords(sf::Vector2i(plotting_data.first.at(i),
                                                              plotting_data.second.at(i))));
        auto angle_to_rotate = angle_to_rotate_degrees({plotting_data.first.at(i), plotting_data.second.at(i)},
                                                       {plotting_data.first.at(i+1), plotting_data.second.at(i+1)});
        line.rotate(angle_to_rotate);
        window.draw(line);
    }
}
void draw_simple(sf::RenderWindow &window,
                 const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                 const sf::Color color){
    draw_line_plot_simple(window, plotting_data, color);
}
void draw_line_plot_simple(sf::RenderWindow &window,
                           const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                           const int color) {
    const auto line_width {5};

    for (int i {}; i < plotting_data.first.size() - 1; i++) {
        auto line_height = distance2Points({plotting_data.first.at(i), plotting_data.second.at(i)},
                                           {plotting_data.first.at(i+1), plotting_data.second.at(i+1)});
        auto line {sf::RectangleShape(sf::Vector2f(line_height, line_width))};
        line.setFillColor(GRAPH_COLORS.at(color));
        line.setPosition(window.mapPixelToCoords(sf::Vector2i(plotting_data.first.at(i),
                                                              plotting_data.second.at(i))));
        auto angle_to_rotate = angle_to_rotate_degrees({plotting_data.first.at(i), plotting_data.second.at(i)},
                                                       {plotting_data.first.at(i+1), plotting_data.second.at(i+1)});
        line.rotate(angle_to_rotate);
        window.draw(line);
    }
}
void draw_simple(sf::RenderWindow &window,
                 const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                 const int color){
    draw_line_plot_simple(window, plotting_data, color);
}

// ADVANCED FUNCTIONS //
void draw_advanced(sf::RenderWindow &window,
                   const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                   const sf::Color &color) {
    // Line Plot
    draw_line_plot_advanced(window, plotting_data, color);
}
void draw_advanced(sf::RenderWindow &window,
                   std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                   const int color) {
    // Line Plot
    draw_line_plot_advanced(window, plotting_data, color);
}
void draw_line(sf::RenderWindow &window,
               const std::pair<std::vector<double>, std::vector<double>> points_vec,
               const double line_width,
               const sf::Color &color,
               const int current_index,
               const int next_index,
               const double max_x) {
    auto line_height = distance2Points({points_vec.first.at(current_index), points_vec.second.at(current_index)},
                                       {points_vec.first.at(next_index), points_vec.second.at(next_index)}) + line_width/2;
    auto line {sf::RectangleShape(sf::Vector2f(line_height, line_width))};
    line.setFillColor(color);
    line.setPosition(window.mapPixelToCoords(sf::Vector2i(points_vec.first.at(current_index),
                                                               points_vec.second.at(current_index))));
    auto angle_to_rotate = angle_to_rotate_degrees({points_vec.first.at(current_index), points_vec.second.at(current_index)},
                                                   {points_vec.first.at(next_index), points_vec.second.at(next_index)});
    line.rotate(angle_to_rotate);
    // to make sure that advanced graphs dont connect points that it shouldn't
    window.draw(line);
}
void draw_line(sf::RenderWindow &window,
               const std::pair<std::vector<double>, std::vector<double>> points_vec,
               const double line_width,
               const int color,
               const int current_index,
               const int next_index,
               const double max_x) {
    auto line_height = distance2Points({points_vec.first.at(current_index), points_vec.second.at(current_index)},
                                       {points_vec.first.at(next_index), points_vec.second.at(next_index)}) + line_width/2;
    auto line {sf::RectangleShape(sf::Vector2f(line_height, line_width))};
    line.setFillColor(GRAPH_COLORS.at(color));
    line.setPosition(window.mapPixelToCoords(sf::Vector2i(points_vec.first.at(current_index),
                                                          points_vec.second.at(current_index))));
    auto angle_to_rotate = angle_to_rotate_degrees({points_vec.first.at(current_index), points_vec.second.at(current_index)},
                                                   {points_vec.first.at(next_index), points_vec.second.at(next_index)});
    line.rotate(angle_to_rotate);
    // to make sure that advanced graphs dont connect points that it shouldn't
    window.draw(line);
}
void draw_line_plot_advanced(sf::RenderWindow &window,
                             const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                             const int color) {
    const auto line_width {5};
    std::pair<std::vector<double>, std::vector<double>> exclude_list {};

    exclude_list.first.push_back(plotting_data.first.at(0));
    exclude_list.second.push_back(plotting_data.second.at(0));

    // checking if there any points to plotor not
    if (plotting_data.first.size() == 0) {
        return;
    }

    auto max_x = (*std::max_element(plotting_data.first.begin(), plotting_data.first.end())-800)/PIXELS_PER_UNIT_X;

    auto i = 0;

    while (exclude_list.first.size() != plotting_data.first.size()) {
        auto x_vals = plotting_data.first;
        auto y_vals = plotting_data.second;

        auto result = find_closest_point(plotting_data, exclude_list, i, x_vals, y_vals);
        auto possible_closest_distance= result.first;
        auto closest_point_index = result.second;

        draw_line(window, plotting_data, line_width, color, i, closest_point_index, max_x);

        exclude_list.first.push_back(plotting_data.first.at(closest_point_index));
        exclude_list.second.push_back(plotting_data.second.at(closest_point_index));

        i = closest_point_index;
    }
    // connect last pt to first
    draw_line(window, exclude_list, line_width, color, exclude_list.first.size()-1, 0, max_x);
}
void draw_line_plot_advanced(sf::RenderWindow &window,
                             const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                             const sf::Color &color) {
    // checking if there any points to plotor not
    if (plotting_data.first.size() == 0) {
        return;
    }

    const auto line_width {5};

    std::pair<std::vector<double>, std::vector<double>> exclude_list {};
    exclude_list.first.push_back(plotting_data.first.at(0));
    exclude_list.second.push_back(plotting_data.second.at(0));


    auto max_x = (*std::max_element(plotting_data.first.begin(), plotting_data.first.end())-800)/PIXELS_PER_UNIT_X;

    auto i = 0;

    while (exclude_list.first.size() != plotting_data.first.size()) {
        auto x_vals = plotting_data.first;
        auto y_vals = plotting_data.second;

        auto result = find_closest_point(plotting_data, exclude_list, i, x_vals, y_vals);
        auto possible_closest_distance= result.first;
        auto closest_point_index = result.second;

        draw_line(window, plotting_data, line_width, color, i, closest_point_index, max_x);

        exclude_list.first.push_back(plotting_data.first.at(closest_point_index));
        exclude_list.second.push_back(plotting_data.second.at(closest_point_index));

        i = closest_point_index;
    }
    // connect last pt to first
    draw_line(window, exclude_list, line_width, color, exclude_list.first.size()-1, 0, max_x);
}


bool check_if_in_excludes_list(const std::pair<std::vector<double>, std::vector<double>> &excludes_list,
                               const std::pair<double, double> point) {
    for (int i {}; i < excludes_list.first.size(); i++){
        if (excludes_list.first.at(i) == point.first && excludes_list.second.at(i) == point.second) {
            return true;
        }
    }
    return false;
}
std::pair<double, int> find_closest_point(const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                                          const std::pair<std::vector<double>, std::vector<double>> &exclude_list,
                                          int i,
                                          const std::vector<double> &x_vals,
                                          const std::vector<double> &y_vals) {
    double closest_distance {std::numeric_limits<double>::max()};
    double possible_closest_distance {closest_distance};
    int closest_point_index {};

    for (int j {0}; j < plotting_data.first.size(); j++) {
        if (check_if_in_excludes_list(exclude_list,
                                      {plotting_data.first.at(j), plotting_data.second.at(j)})) {
            continue;
        } else {
            possible_closest_distance = distance2Points({x_vals.at(j), y_vals.at(j)},
                                                        {x_vals.at(i), y_vals.at(i)});
            if (possible_closest_distance < closest_distance && possible_closest_distance > 0) {
                closest_distance = possible_closest_distance;
                closest_point_index = j;
            }
        }
    }
    return {possible_closest_distance, closest_point_index};
}


// AXES STUFF //
void draw_axes(sf::RenderWindow &window) {
    ////////////
    // X-Axis //
    ////////////
    sf::RectangleShape x_axis = sf::RectangleShape(sf::Vector2f(SCREEN_WIDTH_ACTUAL, 2));
    x_axis.setPosition(window.mapPixelToCoords(sf::Vector2i(0, MIDDLE_Y)));
    x_axis.setFillColor(sf::Color::Black);
    window.draw(x_axis);

    ////////////
    // Y-Axis //
    ////////////
    sf::RectangleShape y_axis = sf::RectangleShape(sf::Vector2f(2, SCREEN_HEIGHT_ACTUAL));
    // window.mapPixelToCoords resets where the thingy is drawn. So use it for resizing.
    y_axis.setPosition(window.mapPixelToCoords(sf::Vector2i(MIDDLE_X-2, 0)));
    y_axis.setFillColor(sf::Color::Black);
    window.draw(y_axis);

    // Drawing Arrows
    draw_arrows(window);

    sf::Font font;
    if (!font.loadFromFile("/Users/vihaanzinjuvadia/CLionProjects/Relearning/VihaansGraphingCalculator/src/Font/OldNewspaperTypes.ttf"))
    {
        // Error
    }

    ///////////////////
    // Labeling Axes //
    ///////////////////
    label_axes(window, font);

    ////////////////////////
    // Drawing Grid Marks //
    ////////////////////////
    draw_grid_marks_and_grid_text(window, font);

}
void label_axes(sf::RenderWindow &window,
                const sf::Font &font) {
    sf::Text x_axis_label;
    sf::Text y_axis_label;

    // labeling x-axis
    x_axis_label.setFont(font); // font is a sf::Font
    x_axis_label.setString("X Axis");
    x_axis_label.setCharacterSize(24); // character size in pixels, not points!
    x_axis_label.setFillColor(sf::Color::Black);
    x_axis_label.setStyle(sf::Text::Bold | sf::Text::Italic);
    x_axis_label.setPosition(window.mapPixelToCoords(sf::Vector2i(SCREEN_WIDTH_ACTUAL-100, MIDDLE_Y-45)));

    // labeling y-axis
    y_axis_label.setFont(font);
    y_axis_label.setString("Y Axis");
    y_axis_label.setCharacterSize(24);
    y_axis_label.setFillColor(sf::Color::Black);
    y_axis_label.setStyle(sf::Text::Bold | sf::Text::Italic);
    y_axis_label.setPosition(window.mapPixelToCoords(sf::Vector2i(MIDDLE_X-125, 0)));

    // drawing text
    window.draw(x_axis_label);
    window.draw(y_axis_label);
}
void draw_arrows(sf::RenderWindow &window) {
    /////////////////////////
    // Creating the arrows //
    /////////////////////////
    std::vector<sf::RectangleShape> arrows {};

    int rotate_angle = 45;
    for (int i {}; i < 8; i++) {
        if (rotate_angle > 315) {
            rotate_angle = 45;
        }
        sf::RectangleShape arrow_part = sf::RectangleShape(sf::Vector2f(15, 2));
        arrow_part.setFillColor(sf::Color::Black);
        arrow_part.rotate(rotate_angle);
        arrows.push_back(arrow_part);
        rotate_angle += 90;
    }

    // x-axis arrows
    arrows.at(0).setPosition(window.mapPixelToCoords(sf::Vector2i(0, MIDDLE_Y+2)));
    arrows.at(1).setPosition(window.mapPixelToCoords(sf::Vector2i(SCREEN_WIDTH_ACTUAL-1, MIDDLE_Y+3)));
    arrows.at(2).setPosition(window.mapPixelToCoords(sf::Vector2i(SCREEN_WIDTH_ACTUAL-1, MIDDLE_Y-1)));
    arrows.at(3).setPosition(window.mapPixelToCoords(sf::Vector2i(0, MIDDLE_Y-2)));

    // y-axis arrows
    arrows.at(4).setPosition(window.mapPixelToCoords(sf::Vector2i(MIDDLE_X, 0)));
    arrows.at(5).setPosition(window.mapPixelToCoords(sf::Vector2i(MIDDLE_X-2, 0)));
    arrows.at(6).setPosition(window.mapPixelToCoords(sf::Vector2i(MIDDLE_X-3, SCREEN_HEIGHT_ACTUAL)));
    arrows.at(3).setPosition(window.mapPixelToCoords(sf::Vector2i(MIDDLE_X-2, SCREEN_HEIGHT_ACTUAL)));

    for (const auto& line: arrows) {
        window.draw(line);
    }
}
void draw_grid_marks_and_grid_text(sf::RenderWindow &window,
                                   const sf::Font &font){
    auto all_grid_marks = grid_marks_data(window);
    auto grid_marks_x = all_grid_marks.first;
    auto grid_marks_y = all_grid_marks.second;

    sf::Text grid_text_x;
    grid_text_x.setFont(font);
    grid_text_x.setFillColor(sf::Color::Black);
    grid_text_x.setCharacterSize(24);
    grid_text_x.setStyle(sf::Text::Bold);

    sf::Text grid_text_y;
    grid_text_y.setFont(font);
    grid_text_y.setFillColor(sf::Color::Black);
    grid_text_y.setCharacterSize(24);
    grid_text_y.setStyle(sf::Text::Bold);

    for (int i {}; i < grid_marks_x.size(); i++) {
        sf::RectangleShape grid_mark_x = sf::RectangleShape(sf::Vector2f(2, 20));
        grid_mark_x.setFillColor(sf::Color::Black);
        grid_mark_x.setPosition(
                window.mapPixelToCoords(
                        sf::Vector2i(MIDDLE_X - (grid_marks_x.at(i) * PIXELS_PER_UNIT_X) - 2, MIDDLE_Y - 9))
        );
        window.draw(grid_mark_x);

        auto num {-1*grid_marks_x.at(i)};
        std::ostringstream oss_x;
        oss_x << std::fixed << std::setprecision(2) << num;
        if (num < 0.0001 && num > -0.0001) {
            grid_text_x.setString(std::to_string(0));
        } else {
            grid_text_x.setString(oss_x.str());
        }
        grid_text_x.setPosition(
                window.mapPixelToCoords(sf::Vector2i(MIDDLE_X - (grid_marks_x.at(i) * PIXELS_PER_UNIT_X) - 2, MIDDLE_Y))
        );
        window.draw(grid_text_x);
    }

    for (int i {}; i < grid_marks_y.size(); i++) {
        sf::RectangleShape grid_mark_y = sf::RectangleShape(sf::Vector2f(20, 2));
        grid_mark_y.setFillColor(sf::Color::Black);
        grid_mark_y.setPosition(
                window.mapPixelToCoords(
                        sf::Vector2i(MIDDLE_X - 2 - 9, MIDDLE_Y - (grid_marks_y.at(i) * PIXELS_PER_UNIT_Y)))
        );
        window.draw(grid_mark_y);

        auto num = grid_marks_y.at(i);
        std::ostringstream oss_y;
        oss_y << std::fixed << std::setprecision(2) << num;
        if (num < 0.0001 && num > -0.0001) {
            grid_text_y.setString(std::to_string(0));
        } else {
            grid_text_y.setString(oss_y.str());
        }
        grid_text_y.setPosition(
                window.mapPixelToCoords(sf::Vector2i(MIDDLE_X - 2, MIDDLE_Y - (grid_marks_y.at(i) * PIXELS_PER_UNIT_Y)))
        );
        window.draw(grid_text_y);
    }
}

#endif //VIHAANSGRAPHICCALC_DRAW_H
