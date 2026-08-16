//
// Created by Vihaan Zinjuvadia on 7/9/23.
//

#ifndef VIHAANSGRAPHINGCALC_MAIN_LOOP_H
#define VIHAANSGRAPHINGCALC_MAIN_LOOP_H

#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <limits>
#include <cmath>
#include <thread>
#include <SFML/Graphics.hpp>
#include "FunctionParser/Utility/UtilityVariables.h"
#include "Draw.h"

using PlottingData = std::pair<std::vector<double>, std::vector<double>>;

// Sampled on three threads, joined before anything reads the results back.
inline void threaded_data_generator(const std::string &f1, const std::string &f2, const std::string &f3,
                                    PlottingData &plotting_data_f1,
                                    PlottingData &plotting_data_f2,
                                    PlottingData &plotting_data_f3) {
    std::thread f1_data_gen_thread {data_to_plot_simple, f1, &plotting_data_f1};
    std::thread f2_data_gen_thread {data_to_plot_simple, f2, &plotting_data_f2};
    std::thread f3_data_gen_thread {data_to_plot_advanced, f3, &plotting_data_f3};

    f1_data_gen_thread.join();
    f2_data_gen_thread.join();
    f3_data_gen_thread.join();
}

inline void on_resize(int width, int height, sf::RenderWindow &window, sf::View &view) {
    SCREEN_WIDTH_ACTUAL = width;
    SCREEN_HEIGHT_ACTUAL = height;
    MIDDLE_X = SCREEN_WIDTH_ACTUAL / 2.0;
    MIDDLE_Y = SCREEN_HEIGHT_ACTUAL / 2.0;

    // Zoom is deliberately left alone here. Resetting it to 10 is what made the range
    // set with F appear not to work: any resize silently threw it away.
    MAX_SEGMENT_PIXELS = SCREEN_HEIGHT_ACTUAL;

    view.setSize({
                         static_cast<float>(SCREEN_WIDTH_ACTUAL),
                         static_cast<float>(SCREEN_HEIGHT_ACTUAL)
                 });
    // Without recentring, the view keeps its old centre and everything looks shifted.
    view.setCenter({
                           static_cast<float>(MIDDLE_X),
                           static_cast<float>(MIDDLE_Y)
                   });
    window.setView(view);
}

// Returns true when the zoom actually changed.
inline bool prompt_for_range() {
    std::cout << "Enter x_range and y_range, separated by space: " << std::flush;

    double x_range {};
    double y_range {};
    if (!(std::cin >> x_range >> y_range)) {
        // Without clearing, the stream stays bad and every later prompt returns
        // instantly, spinning the loop.
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Could not read two numbers. Leaving the range unchanged.\n";
        return false;
    }

    // A range of zero makes pixels-per-unit infinite and the grid mark loop never end.
    if (!std::isfinite(x_range) || !std::isfinite(y_range) || x_range <= 0 || y_range <= 0) {
        std::cout << "Both ranges have to be positive numbers. Leaving the range unchanged.\n";
        return false;
    }

    const int remainder_x = SCREEN_WIDTH_ACTUAL % 100;
    const int remainder_y = SCREEN_HEIGHT_ACTUAL % 100;

    PIXELS_PER_UNIT_X = (SCREEN_WIDTH_ACTUAL - remainder_x) / (2.0 * x_range);
    PIXELS_PER_UNIT_Y = (SCREEN_HEIGHT_ACTUAL - remainder_y) / (2.0 * y_range);
    return true;
}

inline int main_loop() {
    sf::RenderWindow window;
    window.create(sf::VideoMode{static_cast<unsigned int>(SCREEN_WIDTH_ACTUAL), static_cast<unsigned int>(SCREEN_HEIGHT_ACTUAL)},
                  "My Graphing Calculator", sf::Style::Default);
    // Otherwise the loop redraws as fast as it can and pins a core for no benefit.
    window.setFramerateLimit(60);

    // Creating my own view (views are useful for being able to scroll, rotate, or zoom in your program)
    sf::View view = window.getDefaultView();

    const std::string f1 = "y = 1/x";
    const std::string f2 = "20arcsin(cos(pi/10 * x))";
    const std::string f3 = "((x-30)^2)/900 + ((y-15)^2)/225 = 1";

    std::cout << "Graphing:\n  " << f1 << "\n  y = " << f2 << "\n  " << f3
              << "\nPress F with the window focused to set the x and y range.\n";

    PlottingData plotting_data_f1 {};
    PlottingData plotting_data_f2 {};
    PlottingData plotting_data_f3 {};
    threaded_data_generator(f1, f2, f3, plotting_data_f1, plotting_data_f2, plotting_data_f3);

    // Main Loop
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                if (event.size.width < MIN_SIZE_WIDTH || event.size.height < MIN_SIZE_HEIGHT) {
                    window.setSize(sf::Vector2u(MIN_SIZE_WIDTH, MIN_SIZE_HEIGHT));
                    on_resize(static_cast<int>(MIN_SIZE_WIDTH), static_cast<int>(MIN_SIZE_HEIGHT), window, view);
                } else {
                    on_resize(static_cast<int>(event.size.width), static_cast<int>(event.size.height), window, view);
                }
                // Sample range depends on window size, so the data is stale either way.
                threaded_data_generator(f1, f2, f3, plotting_data_f1, plotting_data_f2, plotting_data_f3);
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F) {
                    if (prompt_for_range()) {
                        threaded_data_generator(f1, f2, f3, plotting_data_f1, plotting_data_f2, plotting_data_f3);
                    }
                }
            }
        }

        window.clear(sf::Color::White);
        draw_simple(window, plotting_data_f1, sf::Color(123, 234, 74));
        draw_simple(window, plotting_data_f2, sf::Color::Magenta);
        draw_advanced(window, plotting_data_f3, sf::Color::Cyan);
        draw_axes(window);
        window.display();
    }

    return 0;
}

#endif //VIHAANSGRAPHINGCALC_MAIN_LOOP_H
