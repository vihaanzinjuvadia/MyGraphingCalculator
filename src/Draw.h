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
#include <cmath>
#include "Math.h"

void draw_axes(sf::RenderWindow &window);
void draw_arrows(sf::RenderWindow &window);
void label_axes(sf::RenderWindow &window, const sf::Font &font);
void draw_grid_marks_and_grid_text(sf::RenderWindow &window, const sf::Font &font);

// Finite and small enough that narrowing to the int SFML wants is defined behaviour.
inline bool is_plottable(const double x, const double y) {
    return std::isfinite(x) && std::isfinite(y) &&
           std::fabs(x) < 1.0e7 && std::fabs(y) < 1.0e7;
}

inline sf::Vector2f to_screen(const sf::RenderWindow &window, const double x, const double y) {
    return window.mapPixelToCoords(sf::Vector2i(static_cast<int>(x), static_cast<int>(y)));
}

inline void draw_segment(sf::RenderWindow &window,
                         const double x1, const double y1,
                         const double x2, const double y2,
                         const double line_width,
                         const sf::Color &color) {
    const auto line_height = distance2Points(x1, y1, x2, y2);
    const auto line_height_delta = 0.5;
    auto line {sf::RectangleShape(sf::Vector2f(static_cast<float>(line_height + line_height_delta),
                                               static_cast<float>(line_width)))};
    line.setFillColor(color);
    line.setPosition(to_screen(window, x1, y1));
    line.rotate(static_cast<float>(angle_to_rotate_degrees(x1, y1, x2, y2)));
    window.draw(line);
}

// Loaded once. Reloading the file every frame cost more than the rest of the frame.
inline const sf::Font* graph_font() {
    static sf::Font font;
    static const bool loaded = [] {
        const char *candidates[] = {
#ifdef GRAPH_FONT_PATH
            GRAPH_FONT_PATH,
#endif
            "JetBrainsMonoNerdFont-Regular.ttf",
            "src/Font/JetBrainsMonoNerdFont-Regular.ttf",
        };
        for (const char *path : candidates) {
            if (font.loadFromFile(path)) {
                return true;
            }
        }
        std::cerr << "Could not load the graph font; axis labels will be hidden.\n";
        return false;
    }();
    return loaded ? &font : nullptr;
}

// SIMPLE FUNCTIONS //
// Joins consecutive samples, except across a NaN or a jump too tall to be anything
// but an asymptote.
inline void draw_line_plot_simple(sf::RenderWindow &window,
                                  const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                                  const sf::Color color) {
    const auto line_width {2};
    const auto &xs = plotting_data.first;
    const auto &ys = plotting_data.second;

    // i + 1 < size() rather than i < size() - 1, which wraps around on empty data.
    for (std::size_t i {}; i + 1 < xs.size() && i + 1 < ys.size(); i++) {
        if (!is_plottable(xs.at(i), ys.at(i)) || !is_plottable(xs.at(i + 1), ys.at(i + 1))) {
            continue;
        }
        if (std::fabs(ys.at(i + 1) - ys.at(i)) > MAX_SEGMENT_PIXELS) {
            continue;
        }
        draw_segment(window, xs.at(i), ys.at(i), xs.at(i + 1), ys.at(i + 1), line_width, color);
    }
}
inline void draw_simple(sf::RenderWindow &window,
                        const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                        const sf::Color color) {
    draw_line_plot_simple(window, plotting_data, color);
}
inline void draw_simple(sf::RenderWindow &window,
                        const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                        const int color) {
    if (color < 0 || static_cast<std::size_t>(color) >= GRAPH_COLORS.size()) {
        return;
    }
    draw_line_plot_simple(window, plotting_data, GRAPH_COLORS.at(color));
}

// ADVANCED FUNCTIONS //
// The grid points an implicit curve passes through have no meaningful order, so they
// get drawn as dots. Joining them by nearest neighbour scrambled the shape.
inline void draw_point_plot_advanced(sf::RenderWindow &window,
                                     const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                                     const sf::Color &color) {
    const auto &xs = plotting_data.first;
    const auto &ys = plotting_data.second;
    const float dot_size {2.0f};

    for (std::size_t i {}; i < xs.size() && i < ys.size(); i++) {
        if (!is_plottable(xs.at(i), ys.at(i))) {
            continue;
        }
        auto dot {sf::RectangleShape(sf::Vector2f(dot_size, dot_size))};
        dot.setFillColor(color);
        dot.setPosition(to_screen(window, xs.at(i), ys.at(i)));
        window.draw(dot);
    }
}
inline void draw_advanced(sf::RenderWindow &window,
                          const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                          const sf::Color &color) {
    draw_point_plot_advanced(window, plotting_data, color);
}
inline void draw_advanced(sf::RenderWindow &window,
                          const std::pair<std::vector<double>, std::vector<double>> &plotting_data,
                          const int color) {
    if (color < 0 || static_cast<std::size_t>(color) >= GRAPH_COLORS.size()) {
        return;
    }
    draw_point_plot_advanced(window, plotting_data, GRAPH_COLORS.at(color));
}


// AXES STUFF //
inline void draw_axes(sf::RenderWindow &window) {
    ////////////
    // X-Axis //
    ////////////
    sf::RectangleShape x_axis = sf::RectangleShape(sf::Vector2f(static_cast<float>(SCREEN_WIDTH_ACTUAL), 2));
    x_axis.setPosition(to_screen(window, 0, MIDDLE_Y));
    x_axis.setFillColor(sf::Color::Black);
    window.draw(x_axis);

    ////////////
    // Y-Axis //
    ////////////
    sf::RectangleShape y_axis = sf::RectangleShape(sf::Vector2f(2, static_cast<float>(SCREEN_HEIGHT_ACTUAL)));
    // window.mapPixelToCoords resets where the thingy is drawn. So use it for resizing.
    y_axis.setPosition(to_screen(window, MIDDLE_X - 2, 0));
    y_axis.setFillColor(sf::Color::Black);
    window.draw(y_axis);

    // Drawing Arrows
    draw_arrows(window);

    const sf::Font *font = graph_font();
    if (font == nullptr) {
        return;
    }

    ///////////////////
    // Labeling Axes //
    ///////////////////
    label_axes(window, *font);

    ////////////////////////
    // Drawing Grid Marks //
    ////////////////////////
    draw_grid_marks_and_grid_text(window, *font);
}
inline void label_axes(sf::RenderWindow &window,
                       const sf::Font &font) {
    sf::Text x_axis_label;
    sf::Text y_axis_label;

    // labeling x-axis
    x_axis_label.setFont(font); // font is a sf::Font
    x_axis_label.setString("X Axis");
    x_axis_label.setCharacterSize(16); // character size in pixels, not points!
    x_axis_label.setFillColor(sf::Color::Black);
    x_axis_label.setStyle(sf::Text::Bold | sf::Text::Italic);
    x_axis_label.setPosition(to_screen(window, SCREEN_WIDTH_ACTUAL - 100, MIDDLE_Y - 45));

    // labeling y-axis
    y_axis_label.setFont(font);
    y_axis_label.setString("Y Axis");
    y_axis_label.setCharacterSize(16);
    y_axis_label.setFillColor(sf::Color::Black);
    y_axis_label.setStyle(sf::Text::Bold | sf::Text::Italic);
    y_axis_label.setPosition(to_screen(window, MIDDLE_X - 125, 0));

    // drawing text
    window.draw(x_axis_label);
    window.draw(y_axis_label);
}
inline void draw_arrows(sf::RenderWindow &window) {
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
        arrow_part.rotate(static_cast<float>(rotate_angle));
        arrows.push_back(arrow_part);
        rotate_angle += 90;
    }

    // x-axis arrows
    arrows.at(0).setPosition(to_screen(window, 0, MIDDLE_Y + 2));
    arrows.at(1).setPosition(to_screen(window, SCREEN_WIDTH_ACTUAL - 1, MIDDLE_Y + 3));
    arrows.at(2).setPosition(to_screen(window, SCREEN_WIDTH_ACTUAL - 1, MIDDLE_Y - 1));
    arrows.at(3).setPosition(to_screen(window, 0, MIDDLE_Y - 2));

    // y-axis arrows
    arrows.at(4).setPosition(to_screen(window, MIDDLE_X, 0));
    arrows.at(5).setPosition(to_screen(window, MIDDLE_X - 2, 0));
    arrows.at(6).setPosition(to_screen(window, MIDDLE_X - 3, SCREEN_HEIGHT_ACTUAL));
    arrows.at(7).setPosition(to_screen(window, MIDDLE_X - 2, SCREEN_HEIGHT_ACTUAL));

    for (const auto& line: arrows) {
        window.draw(line);
    }
}
inline void draw_grid_marks_and_grid_text(sf::RenderWindow &window,
                                          const sf::Font &font) {
    const auto all_grid_marks = grid_marks_data(window);
    const auto &grid_marks_x = all_grid_marks.first;
    const auto &grid_marks_y = all_grid_marks.second;

    sf::Text grid_text_x;
    grid_text_x.setFont(font);
    grid_text_x.setFillColor(sf::Color::Black);
    grid_text_x.setCharacterSize(16);
    grid_text_x.setStyle(sf::Text::Bold);

    sf::Text grid_text_y;
    grid_text_y.setFont(font);
    grid_text_y.setFillColor(sf::Color::Black);
    grid_text_y.setCharacterSize(16);
    grid_text_y.setStyle(sf::Text::Bold);

    for (const double mark : grid_marks_x) {
        const double at_pixel {MIDDLE_X + (mark * PIXELS_PER_UNIT_X)};
        if (!is_plottable(at_pixel, MIDDLE_Y)) {
            continue;
        }

        sf::RectangleShape grid_mark_x = sf::RectangleShape(sf::Vector2f(2, 20));
        grid_mark_x.setFillColor(sf::Color::Black);
        grid_mark_x.setPosition(to_screen(window, at_pixel - 2, MIDDLE_Y - 9));
        window.draw(grid_mark_x);

        std::ostringstream oss_x;
        oss_x << std::fixed << std::setprecision(2) << mark;
        grid_text_x.setString(std::fabs(mark) < 0.0001 ? "0" : oss_x.str());
        grid_text_x.setPosition(to_screen(window, at_pixel - 2, MIDDLE_Y));
        window.draw(grid_text_x);
    }

    for (const double mark : grid_marks_y) {
        const double at_pixel {MIDDLE_Y - (mark * PIXELS_PER_UNIT_Y)};
        if (!is_plottable(MIDDLE_X, at_pixel)) {
            continue;
        }

        sf::RectangleShape grid_mark_y = sf::RectangleShape(sf::Vector2f(20, 2));
        grid_mark_y.setFillColor(sf::Color::Black);
        grid_mark_y.setPosition(to_screen(window, MIDDLE_X - 2 - 9, at_pixel));
        window.draw(grid_mark_y);

        std::ostringstream oss_y;
        oss_y << std::fixed << std::setprecision(2) << mark;
        grid_text_y.setString(std::fabs(mark) < 0.0001 ? "0" : oss_y.str());
        grid_text_y.setPosition(to_screen(window, MIDDLE_X - 2, at_pixel));
        window.draw(grid_text_y);
    }
}

#endif //VIHAANSGRAPHICCALC_DRAW_H
