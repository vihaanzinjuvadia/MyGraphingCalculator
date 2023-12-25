//
// Created by Vihaan Zinjuvadia on 7/9/23.
//

#ifndef VIHAANSGRAPHINGCALC_MAIN_LOOP_H
#define VIHAANSGRAPHINGCALC_MAIN_LOOP_H

#include <vector>
#include "FunctionParser/Utility/UtilityVariables.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Draw.h"
#include <thread>

void threaded_data_generator(std::string &f1, std::string &f2, std::string &f3,
                             std::pair<std::vector<double>, std::vector<double>> *&plotting_data_f1,
                             std::pair<std::vector<double>, std::vector<double>> *&plotting_data_f2,
                             std::pair<std::vector<double>, std::vector<double>> *&plotting_data_f3,
                             std::thread &f1_data_gen_thread, std::thread &f2_data_gen_thread,
                             std::thread &f3_data_gen_thread);

void on_resize(int width, int height, sf::RenderWindow &window, sf::View &view) {
    SCREEN_WIDTH_ACTUAL = width;
    SCREEN_HEIGHT_ACTUAL = height;
    MIDDLE_X = SCREEN_WIDTH_ACTUAL/2.0;
    MIDDLE_Y = SCREEN_HEIGHT_ACTUAL/2.0;

    PIXELS_PER_UNIT_X = 10;
    PIXELS_PER_UNIT_Y = 10;

    view.setSize({
                         static_cast<float>(SCREEN_WIDTH_ACTUAL),
                         static_cast<float>(SCREEN_HEIGHT_ACTUAL)
                 });
    window.setView(view);
}

int main_loop() {
    sf::RenderWindow window;
    window.create(sf::VideoMode{static_cast<unsigned int>(SCREEN_WIDTH_ACTUAL), static_cast<unsigned int>(SCREEN_HEIGHT_ACTUAL)}, "My Graphing Calculator", sf::Style::Default);


    // Creating my own view (views are useful for being able to scroll, rotate, or zoom in your program)
    sf::View view = window.getDefaultView();

    std::string f1 = "y=21x";
    std::string f2 = "20arcsin(cos(pi/10 * x))";
    std::string f3 =  "(x^2)/900 + (y^2)/225 = 1";

    // Threading
    auto plotting_data_f1 = new std::pair<std::vector<double>, std::vector<double>>;
    auto plotting_data_f2 = new std::pair<std::vector<double>, std::vector<double>>;
    auto plotting_data_f3 = new std::pair<std::vector<double>, std::vector<double>>;
    std::thread f1_data_gen_thread;
    std::thread f2_data_gen_thread;
    std::thread f3_data_gen_thread;
    threaded_data_generator(f1, f2, f3,
                            plotting_data_f1, plotting_data_f2, plotting_data_f3,
                            f1_data_gen_thread,f2_data_gen_thread,f3_data_gen_thread);


    // Main Loop
    double x_range, y_range;
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                if (event.size.width < MIN_SIZE_WIDTH || event.size.height < MIN_SIZE_HEIGHT) {
                    window.setSize(sf::Vector2u(MIN_SIZE_WIDTH, MIN_SIZE_HEIGHT));
                    on_resize(MIN_SIZE_WIDTH, MIN_SIZE_HEIGHT, window, view);

                } else {
                    on_resize(event.size.width, event.size.height, window, view);
                    threaded_data_generator(f1, f2, f3,
                                            plotting_data_f1, plotting_data_f2, plotting_data_f3,
                                            f1_data_gen_thread,f2_data_gen_thread,f3_data_gen_thread);
                }
            }


            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F) {
                    std::cout << "Enter x_range and y_range, separated by space: ";
                    std::cin >> x_range >> y_range;

                    // Ranges aren't working --> Fullscreen and then scaling to 10, 10 doesn't work
                    int remainder_x = SCREEN_WIDTH_ACTUAL % 100;
                    int remainder_y = SCREEN_HEIGHT_ACTUAL % 100;

                    PIXELS_PER_UNIT_X = (SCREEN_WIDTH_ACTUAL-remainder_x)/(2.0 * x_range);
                    PIXELS_PER_UNIT_Y = (SCREEN_HEIGHT_ACTUAL-remainder_y)/(2.0 * y_range);

                    // Plotting Data for simple functions
                    threaded_data_generator(f1, f2, f3,
                                            plotting_data_f1, plotting_data_f2, plotting_data_f3,
                                            f1_data_gen_thread,f2_data_gen_thread,f3_data_gen_thread);
                }
            }
        }

        window.clear(sf::Color::White);
        draw_simple(window, *plotting_data_f1, {123, 234, 74});
        draw_simple(window, *plotting_data_f2, sf::Color::Magenta);
        draw_advanced(window, *plotting_data_f3, sf::Color::Magenta);
        draw_axes(window);
        window.display();
    }
//

    return 0;
}

void threaded_data_generator(std::string &f1, std::string &f2, std::string &f3,
                             std::pair<std::vector<double>, std::vector<double>> *&plotting_data_f1,
                             std::pair<std::vector<double>, std::vector<double>> *&plotting_data_f2,
                             std::pair<std::vector<double>, std::vector<double>> *&plotting_data_f3,
                             std::thread &f1_data_gen_thread, std::thread &f2_data_gen_thread,
                             std::thread &f3_data_gen_thread) {

    f1_data_gen_thread = std::thread(data_to_plot_simple, f1, plotting_data_f1);
    f2_data_gen_thread = std::thread(data_to_plot_simple, f2, plotting_data_f2);
    f3_data_gen_thread = std::thread(data_to_plot_advanced, f3, plotting_data_f3);
    f1_data_gen_thread.join();
    f2_data_gen_thread.join();
    f3_data_gen_thread.join();
}

#endif //VIHAANSGRAPHINGCALC_MAIN_LOOP_H
