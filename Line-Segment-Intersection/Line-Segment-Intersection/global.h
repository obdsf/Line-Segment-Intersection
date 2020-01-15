#pragma once
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <iostream>
#include <math.h>
// Boost Library
// SFML : Simple and Fast Multimedia Library
#include <SFML/Graphics.hpp>
// Custom Headers
#include "point.h"

// # ### #################################################################################### ### #

constexpr int windowWidth{ 1200 }; // the width of the window in pixels
constexpr int windowHeight{ 800 }; // the height of the window in pixels
constexpr int topMargin{ 100 }; // UI top margin
constexpr int bottomMargin{ 100 }; // UI bottom margin
constexpr int rightMargin{ 300 }; // UI right margin
constexpr int leftMargin{ 300 }; // UI left margin
constexpr int xBias{ windowWidth / 2 }; // bias of x axis due to window implementation
constexpr int yBias{ windowHeight / 2 }; // bias of y axis (reversed) due to window implementation
constexpr int distMin{ -300 }; // distribution minimum value
constexpr int distMax{ 300 }; // distribution maximum value
constexpr int fontSize{ 25 };
constexpr int intersectionPointsRadius{ 3 }; // the radius of the circles that appear on screen denoting intersection points between line segments
constexpr int multiPairSetSizeThreshold{ 100 }; // the maximum lines drawn when in multi pair mode (MPM)
constexpr double precision = 0.1; // (0.1) the precision of the logical grid (the smallest possible distance between two points)
constexpr double lineThickness = 0.001; // (0.001) the logical thickness of a line segment (how far away from the absolute center of a line are points considered part of the line)
constexpr double yValuePrecision = 0.0000001; // (1e-07 : 0.0000001) the precision of the y value below the sweep line
// according to which calculations and comparisons between line segments in the status structure occur

const sf::Time timePerUpdate = sf::seconds(1.f / 60.f); // sets the maximum updates per second when the update limiter is on
const std::string singlePairModeText{ "Single Pair Mode (SPM)" };
const std::string multiPairModeText{ "Multi Pair Mode  (MPM)" };
const std::string fancyCalculationModeText{ "Fancy" };
const std::string fastCalculationModeText{ "Fast" };
const std::string updateLimiterOnText{ "Update Limiter On" };
const std::string updateLimiterOffText{ "Update Limiter Off" };
const std::string readModeText{ "Read Mode" };
const std::string writeModeText{ "Write Mode" };
const point epbTopLeft{ leftMargin, topMargin }; // top left point that defines the euclidean space on screen
const point epbTopRight{ windowWidth - rightMargin, topMargin }; // top right point that defines the euclidean space on screen
const point epbBotRight{ windowWidth - rightMargin, windowHeight - bottomMargin }; // bottom right point that defines the euclidean space on screen
const point epbBotLeft{ leftMargin, windowHeight - bottomMargin }; // bottom left point that defines the euclidean space on screen

__declspec(selectany) unsigned int debuggingUnionCounter{ 0 };

template <typename T>
std::string toString(const T& value) {
  std::stringstream stream;
  stream << value;
  return stream.str();
}

template <typename T>
T truncOneDigit(const T& value) {
  return (T)((int)(value * 10)) / 10;
}
