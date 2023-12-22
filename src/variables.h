//
// Created by Vihaan Zinjuvadia on 4/29/23.
//

#ifndef VIHAANSGRAPHICCALC_VARIABLES_H
#define VIHAANSGRAPHICCALC_VARIABLES_H

#include <SFML/Graphics.hpp>

// number of pixels per one coordinate plane unit;

double PIXELS_PER_UNIT_X = 10;
double PIXELS_PER_UNIT_Y = 10;

int SCREEN_WIDTH_ACTUAL = 1600;
int SCREEN_HEIGHT_ACTUAL = 1600;
double MIDDLE_X = 800.0;
double MIDDLE_Y = 800.0;

// minimum window size
unsigned int MIN_SIZE_WIDTH = 1600;
unsigned int MIN_SIZE_HEIGHT = 1600;

int NUM_POINTS_TO_SAMPLE = 780;

// make a mapping of colors to choose from
std::vector<sf::Color> GRAPH_COLORS {sf::Color::Black, sf::Color::Blue, sf::Color::Cyan, sf::Color::Green,
                                     sf::Color::Magenta, sf::Color::Red, sf::Color::Yellow};

#endif //VIHAANSGRAPHICCALC_VARIABLES_H
