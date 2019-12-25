#pragma once
// ### # #################################################################################### # ###

// STL : Standard Template Library
#include <string>
#include <sstream>
#include <random>
#include <vector>
// Boost Library
// SFML : Simple and Fast Multimedia Library
#include <SFML/Graphics.hpp>
// Custom Headers
#include "point.h"
#include "line_segment.h"
#include "sweep_line.h"

// ### # #################################################################################### # ###

class euclidean_plane : private sf::NonCopyable {
// ### # Class Member Variables # ###
private:
  // Window
  sf::RenderWindow m_window;

  // Axis' & Boundaries
  sf::VertexArray m_xAxis;
  sf::VertexArray m_yAxis;

  sf::VertexArray m_leftBoundary;
  sf::VertexArray m_topBoundary;
  sf::VertexArray m_rightBoundary;
  sf::VertexArray m_botBoundary;

  // Physical and Logical Lines and Intersection Points
  line_segment m_physicalLineA;
  line_segment m_physicalLineB;
  point m_physicalIntersectionPointNaive;
  point m_physicalIntersectionPointSweep;

  sf::VertexArray m_logicalLineA;
  sf::VertexArray m_logicalLineB;
  sf::CircleShape m_logicalIntersectionPointNaive;
  sf::CircleShape m_logicalIntersectionPointSweep;

  std::vector<line_segment> m_physicalMultiPairSet;
  std::vector<point> m_physicalMultiPairIntersectionPointsNaive;
  std::vector<point> m_physicalMultiPairIntersectionPointsSweep;

  std::vector<sf::VertexArray> m_logicalMultiPairSet;
  std::vector<sf::CircleShape> m_logicalMultiPairIntersectionPointsNaive;
  std::vector<sf::CircleShape> m_logicalMultiPairIntersectionPointsSweep;

  // Sweep Line
  sweep_line m_physicalSweep;
  sf::VertexArray m_logicalSweep;

  // Texts & Fonts
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
  sf::Text m_naiveIntersectionPointText;
  sf::Text m_simulationModeText;
  sf::Text m_calculationModeText;
  sf::Text m_updateModeText;
  sf::Text m_multiPairSetSizeText;

  // Statistics
  sf::Time m_statisticsUpdateTime;
  std::size_t m_statisticsNumFrames;

  // Actions
  bool m_exit;
  bool m_genNewSetOfLines;
  bool m_calcIntersectionsNaive;
  bool m_calcIntersectionsSweep;
  bool m_toggleSimulationMode;
  bool m_toggleHide;
  bool m_eraseCurrentSet;
  bool m_toggleCalculationMode;
  bool m_toggleUpdateMode;
  bool m_toggleAdvancedInfo;

  // States
  bool m_reset;
  bool m_hideAxis;
  bool m_hideBoundaries;
  bool m_hideAdvancedInfo;
  bool m_drawSinglePairIntersectionNaive;
  bool m_drawSinglePairIntersectionSweep;
  bool m_drawMultiPairIntersectionsNaive;
  bool m_drawMultiPairIntersectionsSweep;
  bool m_drawSinglePairSweepLine;
  bool m_drawMultiPairSweepLine;
  bool m_singlePairMode;
  bool m_singlePairTrashed;
  bool m_multiPairTrashed;
  bool m_fancyCalculationMode;
  bool m_useUpdateLimiter;
  bool m_currentlyCalculatingIntersectionsNaive;
  bool m_currentlyCalculatingIntersectionsSweep;

  // Iterators
  int m_multiPairSetSize;
  int m_naiveIterI;
  int m_naiveIterJ;

  // Random Numbers
  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_distribution;

// ### # Class Member Functions # ###
public:
  euclidean_plane(); // Class Constructor
  void run(); // Window Main Loop


private:
  void processEvents();
  void update();
  void render();

  void handleUserInput(sf::Keyboard::Key key, bool isPressed);
  void genNewLine(line_segment& physicalLine);
  void updateLine(sf::VertexArray& logicalLine, line_segment& physicalLine);
  void updateSweep(sf::VertexArray& logicalSweep, const sweep_line& physicalSweep);
  void updatePoint(sf::CircleShape& intersectionPoint, point& k);
  void updateLinesInfo();
  void updateIntersectionPointInfo();
  void updateStatistics(sf::Time elapsedTime);
  void updateSimulationStateInfo();
  void updateMultiPairSetSize(bool increase);

  float orientation(point a, point b, point c);
};
