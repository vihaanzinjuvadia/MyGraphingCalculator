//
// Created by Vihaan Zinjuvadia on 4/29/23.
//

#ifndef VIHAANSGRAPHICCALC_VARIABLES_H
#define VIHAANSGRAPHICCALC_VARIABLES_H

#include <SFML/Graphics.hpp>
#include <vector>

// inline so every translation unit including this shares one copy.

// number of pixels per one coordinate plane unit;
inline double PIXELS_PER_UNIT_X = 10;
inline double PIXELS_PER_UNIT_Y = 10;

inline int SCREEN_WIDTH_ACTUAL = 800;
inline int SCREEN_HEIGHT_ACTUAL = 800;
inline double MIDDLE_X = 400.0;
inline double MIDDLE_Y = 400.0;

// minimum window size
inline unsigned int MIN_SIZE_WIDTH = 800;
inline unsigned int MIN_SIZE_HEIGHT = 800;

inline int NUM_POINTS_TO_SAMPLE_SIMPLE = 400;
inline int NUM_POINTS_TO_SAMPLE_ADVANCED = 400;

// A segment taller than this is an asymptote jump, not part of the curve, so 1/x
// doesn't get drawn joined through x = 0.
inline double MAX_SEGMENT_PIXELS = 800.0;

// make a mapping of colors to choose from
inline std::vector<sf::Color> GRAPH_COLORS {sf::Color::Black, sf::Color::Blue, sf::Color::Cyan, sf::Color::Green,
                                            sf::Color::Magenta, sf::Color::Red, sf::Color::Yellow};

#endif //VIHAANSGRAPHICCALC_VARIABLES_H
