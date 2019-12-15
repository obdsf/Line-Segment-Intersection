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
  static const int windowWidth;
  static const int windowHeight;
  static const int topMargin;
  static const int bottomMargin;
  static const int rightMargin;
  static const int leftMargin;
  static const int xBias;
  static const int yBias;
  static const int distMin;
  static const int distMax;
  static const int fontSize;
  static const int intersectionPointsRadius;

  sf::RenderWindow m_window;

  line_segment m_physicalLineA;
  line_segment m_physicalLineB;
  point m_physicalIntersectionPoint;

  sf::VertexArray m_xAxis;
  sf::VertexArray m_yAxis;
  sf::VertexArray m_logicalLineA;
  sf::VertexArray m_logicalLineB;
  sf::CircleShape m_logicalIntersectionPoint;

  sf::Font m_font;
  sf::Text m_statisticsText;
  sf::Text m_lineAText;
  sf::Text m_lineApText;
  sf::Text m_lineAqText;
  sf::Text m_lineAslopeText;
  sf::Text m_lineAyInterceptText;
  sf::Text m_lineBText;
  sf::Text m_lineBpText;
  sf::Text m_lineBqText;
  sf::Text m_lineBslopeText;
  sf::Text m_lineByInterceptText;
  sf::Text m_intersectionPointText;
  sf::Time m_statisticsUpdateTime;
  std::size_t m_statisticsNumFrames;

  bool m_exit;
  bool m_reset;
  bool m_calcIntersections;
  bool m_hideAxis;

  bool m_genNewSetOfLines;
  bool m_drawIntersections;

  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_distribution;

// ### # Class Member Functions # ###
public:
  euclidean_plane(); // Class Constructor
  void launch(); // Window Main Loop


private:
  void processEvents();
  void update();
  void render();

  void handleUserInput(sf::Keyboard::Key key, bool isPressed);
  void genNewSetOfLines(line_segment& physicalLine);
  void updateLine(sf::VertexArray& logicalLine, line_segment& physicalLine);
  void updatePoint(sf::CircleShape& intersectionPoint, point& k);
  void updateLinesInfo();
  void updateIntersectionPointInfo();
  void updateStatistics(sf::Time elapsedTime);

  float orientation(point a, point b, point c);

  template <typename T>
  std::string toString(const T& value);
};
