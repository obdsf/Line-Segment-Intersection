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

constexpr int windowWidth{ 1200 };
constexpr int windowHeight{ 800 };
constexpr int topMargin{ 100 };
constexpr int bottomMargin{ 100 };
constexpr int rightMargin{ 300 };
constexpr int leftMargin{ 300 };
constexpr int xBias{ windowWidth / 2 };
constexpr int yBias{ windowHeight / 2 };
constexpr int distMin{ -300 };
constexpr int distMax{ 300 };
constexpr int fontSize{ 25 };
constexpr int intersectionPointsRadius{ 3 };
constexpr int multiPairSetSizeThreshold{ 100 };
constexpr float g_precision = 0.1;

const sf::Time timePerFrame = sf::seconds(1.f / 60.f);
const std::string singlePairModeText{ "Single Pair Mode (SPM)" };
const std::string multiPairModeText{ "Multi Pair Mode  (MPM)" };
const std::string fancyCalculationModeText{ "Fancy" };
const std::string fastCalculationModeText{ "Fast" };
const std::string updateLimiterOnText{ "Update Limiter On" };
const std::string updateLimiterOffText{ "Update Limiter Off" };
const point epbTopLeft{ leftMargin, topMargin };
const point epbTopRight{ windowWidth - rightMargin, topMargin };
const point epbBotRight{ windowWidth - rightMargin, windowHeight - bottomMargin };
const point epbBotLeft{ leftMargin, windowHeight - bottomMargin };

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
