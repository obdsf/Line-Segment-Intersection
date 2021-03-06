#pragma once
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <string>
#include <sstream>
#include <random>
#include <vector>
#include <array>
// Boost Library
// SFML : Simple and Fast Multimedia Library
#include <SFML/Graphics.hpp>
// Custom Headers
#include "point.h"
#include "line_segment.h"
#include "sweep_line.h"
#include "r_tree.h"
#include "minimum_bounding_rectangle.h"

// # ### #################################################################################### ### #

class euclidean_plane : private sf::NonCopyable {
// # ### Class Member Variables # ###
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
  std::vector<line_segment> m_physicalSinglePairSet;
  point m_physicalIntersectionPointNaive;
  point m_physicalIntersectionPointSweep;

  sf::VertexArray m_logicalLineA;
  sf::VertexArray m_logicalLineB;
  sf::CircleShape m_logicalIntersectionPointNaive;
  sf::CircleShape m_logicalIntersectionPointSweep;

  std::vector<line_segment> m_physicalMultiPairSet;
  std::vector<point> m_physicalMultiPairIntersectionPointsNaive;
  std::vector<point> m_physicalMultiPairIntersectionPointsSweep;
  std::vector<point> m_physicalNaivePointsNotInSweep;
  std::vector<point> m_physicalSweepPointsNotInNaive;

  std::vector<sf::VertexArray> m_logicalMultiPairSet;
  std::vector<sf::CircleShape> m_logicalMultiPairIntersectionPointsNaive;
  std::vector<sf::CircleShape> m_logicalMultiPairIntersectionPointsSweep;
  std::vector<sf::CircleShape> m_logicalNaivePointsNotInSweep;
  std::vector<sf::CircleShape> m_logicalSweepPointsNotInNaive;

  // Sweep Line
  sweep_line m_physicalSweep;
  sf::VertexArray m_logicalSweep;

  // R-Tree
  r_tree m_physicalRTree;
  std::vector<sf::VertexArray> m_logicalRTree;

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
  sf::Text m_sweepIntersectionPointText;
  sf::Text m_simulationModeText;
  sf::Text m_calculationModeText;
  sf::Text m_updateModeText;
  sf::Text m_multiPairSetSizeText;
  sf::Text m_naiveIntersectionsHiddenText;
  sf::Text m_sweepIntersectionsHiddenText;
  sf::Text m_naiveIntersectionsFoundText;
  sf::Text m_sweepIntersectionsFoundText;
  sf::Text m_naiveTimeText;
  sf::Text m_sweepTimeText;
  sf::Text m_timeRecordingModeText;
  sf::Text m_readWriteModeText;
  sf::Text m_readWriteModeActionText;
  sf::Text m_intersectionMismatchText;

  // Statistics
  sf::Time m_statisticsUpdateTime;
  std::size_t m_statisticsNumFrames;

  // line segment intersection time recorders
  sf::Clock m_naiveClock;
  sf::Clock m_sweepClock;
  sf::Time m_naiveTime;
  sf::Time m_sweepTime;

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
  bool m_toggleNaiveIntersections;
  bool m_toggleSweepIntersections;
  bool m_toggleTimeRecording;
  bool m_toggleReadWriteMode;
  bool m_executeReadWriteOperation;
  bool m_compareIntersections;
  bool m_runTestCode;
  bool m_toggleRendererTestCode;
  
  // States
  bool m_reset;

  bool m_hideAxis;
  bool m_hideBoundaries;
  bool m_hideAdvancedInfo;

  bool m_hideNaiveIntersections;
  bool m_hideSweepIntersections;

  bool m_drawSinglePairIntersectionNaive;
  bool m_drawSinglePairIntersectionSweep;
  bool m_drawMultiPairIntersectionsNaive;
  bool m_drawMultiPairIntersectionsSweep;
  bool m_drawSinglePairSweepLine;
  bool m_drawMultiPairSweepLine;

  bool m_singlePairMode;
  bool m_singlePairTrashed;
  bool m_multiPairTrashed;

  bool m_lastSweptSinglePair;
  bool m_fancyCalculationMode;
  bool m_useUpdateLimiter;

  bool m_currentlyCalculatingIntersectionsNaive;
  bool m_currentlyCalculatingIntersectionsSweep;

  bool m_currentlyInReadMode;

  bool m_calcTime;
  bool m_drawNaiveTime;
  bool m_drawSweepTime;
  bool m_intersectionMismatch;
  bool m_drawIntersectionMismatch;
  bool m_drawIntersectionMismatchText;
  bool m_drawReadWriteModeActionText;

  bool m_runRendererTestCode;

  // Iterators
  int m_multiPairSetSize;
  int m_naiveIterI;
  int m_naiveIterJ;

  // Random Numbers
  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_distribution;

  // Other
  std::string m_saveFileName;
  int m_saveFileNumber;
  std::string m_saveFileExtension;
  sf::Clock genericClock;

  // Debugging
  //minimum_bounding_rectangle mbr;

// # ### Class Member Functions # ###
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
  void updateNaiveIntersectionPointInfo();
  void updateSweepIntersectionPointInfo();
  void updateStatistics(sf::Time elapsedTime);
  void updateSimulationStateInfo();
  void updateMultiPairSetSize(bool increase);
  void readWriteMultiPairSetToFile();
  void updateLogicalRTree();
  void drawLogicalMBR(minimum_bounding_rectangle& mbr);
  bool containsPoint(std::vector<point>& vec, point& p);
  double orientation(point a, point b, point c);
};
