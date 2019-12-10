#pragma once

// ### # #################################################################################### # ###

// STL : Standard Template Library
#include <string>
#include <sstream>
#include <random>
// Boost Library
// SFML : Simple and Fast Multimedia Library
#include <SFML/Graphics.hpp>
// Custom Headers
#include "line_segment.h"

// ### # #################################################################################### # ###

class euclidean_plane : private sf::NonCopyable {
// ### # Class Member Variables # ###
private:
  static const sf::Time timePerFrame;

  sf::RenderWindow m_window;
  sf::VertexArray m_logicalLineA;
  sf::VertexArray m_logicalLineB;
  line_segment m_physicalLineA;
  line_segment m_physicalLineB;
  sf::Font m_font;
  sf::Text m_statisticsText;
  sf::Time m_statisticsUpdateTime;
  std::size_t m_statisticsNumFrames;
  bool m_genNewSetOfLines;

  std::default_random_engine m_generator;
  std::uniform_real_distribution<float> m_distribution;

// ### # Class Member Functions # ###
public:
  euclidean_plane(); // Class Constructor
  void run(); // Window Main Loop

private:
  void processEvents();
  void update();
  void render();

  void handleUserInput(sf::Keyboard::Key key, bool isPressed);
  void genNewSetOfLines(line_segment &physicalLine);
  void updateLine(sf::VertexArray &logicalLine, line_segment &physicalLine);
  void updateStatistics(sf::Time elapsedTime);

  template <typename T>
  std::string toString(const T& value);
};
