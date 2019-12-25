
// ### # #################################################################################### # ###

// STL : Standard Template Library
#include <iostream>
#include <random>
#include <vector>
// Boost Library
// SFML : Simple and Fast Multimedia Library
#include <SFML/Graphics.hpp>
// Custom Headers
#include "global.h"
#include "point.h"
#include "line_segment.h"
#include "euclidean_plane.h"

// ### # #################################################################################### # ###

// Constructors & Destructor
euclidean_plane::euclidean_plane()
  : m_window(sf::VideoMode(windowWidth, windowHeight), "Line Segment Intersection Driver")
  , m_physicalLineA{ 0, 0, 1, 1 }, m_physicalLineB{ 0, 0, 1, 1 }, m_physicalIntersectionPoint{}
  , m_xAxis{ sf::Lines, 2 }, m_yAxis{ sf::Lines, 2 }
  , m_leftBoundary{ sf::Lines, 2 }, m_topBoundary{ sf::Lines, 2 }
  , m_rightBoundary{ sf::Lines, 2 }, m_botBoundary{ sf::Lines, 2 }
  , m_logicalLineA{ sf::Lines, 2 }, m_logicalLineB{ sf::Lines, 2 }, m_logicalIntersectionPoint{}
  , m_physicalMultiPairSet{}, m_logicalMultiPairSet{}
  , m_physicalMultiPairIntersectionPointsNaive{}, m_physicalMultiPairIntersectionPointsSweep{}
  , m_logicalMultiPairIntersectionPointsNaive{}, m_logicalMultiPairIntersectionPointsSweep{}
  , m_font{}, m_statisticsText{}
  , m_lineAText{}, m_lineApText{}, m_lineAqText{}, m_lineAslopeText{}
  , m_lineBText{}, m_lineBpText{}, m_lineBqText{}, m_lineBslopeText{}
  , m_intersectionPointText{}, m_simulationModeText{}
  , m_calculationModeText{}, m_updateModeText{}, m_multiPairSetSizeText{}
  , m_statisticsUpdateTime{}, m_statisticsNumFrames{ 0 }
  , m_genNewSetOfLines{ false }, m_reset{ false }, m_exit{ false }
  , m_calcIntersectionsNaive{ false }, m_calcIntersectionsSweep{ false }
  , m_toggleSimulationMode{ false }, m_toggleHide{ false }, m_eraseCurrentSet{ false }
  , m_toggleCalculationMode{ false }, m_toggleUpdateMode{ false }, m_toggleAdvancedInfo{ false }
  , m_drawSinglePairIntersectionNaive{ false }, m_drawSinglePairIntersectionSweep{ false }
  , m_drawMultiPairIntersectionsNaive{ false }, m_drawMultiPairIntersectionsSweep{ false }
  , m_hideAxis{ false }, m_hideBoundaries{ false }, m_hideAdvancedInfo{ false }
  , m_singlePairMode{ true }, m_singlePairTrashed{ false }, m_multiPairTrashed{ false }
  , m_fancyCalculationMode{ true }, m_useUpdateLimiter{ false }
  , m_currentlyCalculatingIntersectionsNaive{ false }, m_currentlyCalculatingIntersectionsSweep{ false }
  , m_multiPairSetSize{ 25 }, m_naiveIterI{ 0 }, m_naiveIterJ{ 0 }, m_distribution{ distMin, distMax }
{
  m_xAxis[0].position = sf::Vector2f(leftMargin, yBias);
  m_xAxis[1].position = sf::Vector2f(windowWidth - leftMargin, yBias);
  m_yAxis[0].position = sf::Vector2f(xBias, windowHeight - bottomMargin);
  m_yAxis[1].position = sf::Vector2f(xBias, topMargin);

  m_leftBoundary[0].position = sf::Vector2f(epbTopLeft.x, epbTopLeft.y);
  m_leftBoundary[1].position = sf::Vector2f(epbBotLeft.x, epbBotLeft.y);
  m_topBoundary[0].position = sf::Vector2f(epbTopLeft.x, epbTopLeft.y);
  m_topBoundary[1].position = sf::Vector2f(epbTopRight.x, epbTopRight.y);
  m_rightBoundary[0].position = sf::Vector2f(epbTopRight.x, epbTopRight.y);
  m_rightBoundary[1].position = sf::Vector2f(epbBotRight.x, epbBotRight.y);
  m_botBoundary[0].position = sf::Vector2f(epbBotLeft.x, epbBotLeft.y);
  m_botBoundary[1].position = sf::Vector2f(epbBotRight.x, epbBotRight.y);

  m_logicalLineA[0].color = sf::Color::Red;
  m_logicalLineA[1].color = sf::Color::Yellow;
  m_logicalLineB[0].color = sf::Color::Blue;
  m_logicalLineB[1].color = sf::Color::Cyan;

  m_logicalIntersectionPoint.setRadius(intersectionPointsRadius);
  m_logicalIntersectionPoint.setFillColor(sf::Color::Green);

  m_font.loadFromFile("fonts/Sansation.ttf");

  m_statisticsText.setFont(m_font);
  m_lineAText.setFont(m_font);
  m_lineApText.setFont(m_font);
  m_lineAqText.setFont(m_font);
  m_lineAslopeText.setFont(m_font);
  m_lineAyInterceptText.setFont(m_font);
  m_lineBText.setFont(m_font);
  m_lineBpText.setFont(m_font);
  m_lineBqText.setFont(m_font);
  m_lineBslopeText.setFont(m_font);
  m_lineByInterceptText.setFont(m_font);
  m_intersectionPointText.setFont(m_font);
  m_simulationModeText.setFont(m_font);
  m_calculationModeText.setFont(m_font);
  m_updateModeText.setFont(m_font);
  m_multiPairSetSizeText.setFont(m_font);

  m_statisticsText.setPosition(5.f, 5.f);
  m_lineAText.setPosition(5.f, windowHeight - 70.f);
  m_lineApText.setPosition(100.f, windowHeight - 70.f);
  m_lineAqText.setPosition(260.f, windowHeight - 70.f);
  m_lineAslopeText.setPosition(420.f, windowHeight - 70.f);
  m_lineAyInterceptText.setPosition(640.f, windowHeight - 70.f);
  m_lineBText.setPosition(5.f, windowHeight - 40.f);
  m_lineBpText.setPosition(100.f, windowHeight - 40.f);
  m_lineBqText.setPosition(260.f, windowHeight - 40.f);
  m_lineBslopeText.setPosition(420.f, windowHeight - 40.f);
  m_lineByInterceptText.setPosition(640.f, windowHeight - 40.f);
  m_intersectionPointText.setPosition(5.f, yBias - fontSize);
  m_simulationModeText.setPosition(windowWidth - 270.f, windowHeight - 70.f);
  m_calculationModeText.setPosition(windowWidth - 270.f, windowHeight - 40.f);
  m_updateModeText.setPosition(windowWidth - 270.f, 5.f);
  m_multiPairSetSizeText.setPosition(windowWidth / 2 - 80.f, 5.f);
  
  m_statisticsText.setCharacterSize(fontSize);
  m_lineAText.setCharacterSize(fontSize);
  m_lineApText.setCharacterSize(fontSize);
  m_lineAqText.setCharacterSize(fontSize);
  m_lineAslopeText.setCharacterSize(fontSize);
  m_lineAyInterceptText.setCharacterSize(fontSize);
  m_lineBText.setCharacterSize(fontSize);
  m_lineBpText.setCharacterSize(fontSize);
  m_lineBqText.setCharacterSize(fontSize);
  m_lineBslopeText.setCharacterSize(fontSize);
  m_lineByInterceptText.setCharacterSize(fontSize);
  m_intersectionPointText.setCharacterSize(fontSize);
  m_simulationModeText.setCharacterSize(fontSize);
  m_calculationModeText.setCharacterSize(fontSize);
  m_updateModeText.setCharacterSize(fontSize);
  m_multiPairSetSizeText.setCharacterSize(fontSize);

  m_lineApText.setFillColor(sf::Color::Red);
  m_lineAqText.setFillColor(sf::Color::Yellow);
  m_lineBpText.setFillColor(sf::Color::Blue);
  m_lineBqText.setFillColor(sf::Color::Cyan);
  m_intersectionPointText.setFillColor(sf::Color::Green);

  updateSimulationStateInfo();
}

void euclidean_plane::run() {
  // Test Zone Start
  line_segment l1{ 5, 0, 5, 1 };
  line_segment l2{ 1, 0, 2, 1 };
  std::cout << "l1 slope: " << l1.slope << '\n'
    << "l2 slope: " << l2.slope << '\n';
  std::cout << "l1 parallel to l2: " << l1.parallel(l2) << '\n';
  std::cout << "l1 slope eq inf: " << (l1.slope == INFINITY) << '\n';
  std::cout << "l1 slope: " << l1.slope << '\n';
  std::cout << "l1 yIntercept: " << l1.yIntercept << '\n';
  // Test Zone End
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  while (m_window.isOpen()) {
    sf::Time elapsedTime = clock.restart();
    if (m_useUpdateLimiter) {
      timeSinceLastUpdate += elapsedTime;
      while (timeSinceLastUpdate > timePerFrame) {
        timeSinceLastUpdate -= timePerFrame;
        processEvents();
        update();
      }
    } else {
      processEvents();
      update();
    }
    render();
    updateStatistics(elapsedTime);
  }
  return;
}

void euclidean_plane::processEvents() {
  sf::Event event;
  while (m_window.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::KeyPressed:
        handleUserInput(event.key.code, true);
        break;
      case sf::Event::KeyReleased:
        handleUserInput(event.key.code, false);
        break;
      case sf::Event::Closed:
        m_window.close();
        break;
    }
  }
  return;
}

void euclidean_plane::update() {
  // Exit the Application
  if (m_exit) m_window.close();
  // Reset the Random Number Generation Distribution
  if (m_reset) {
    m_distribution.reset();
    std::cout << "Distribution Reset.\n";
    m_reset = false;
  }
  // Change between Single and Multi Pair Simulation
  if (m_toggleSimulationMode) {
    m_singlePairMode = m_singlePairMode ? false : true; // Toggle Simulation State
    updateSimulationStateInfo();
    m_toggleSimulationMode = false;
  }
  // Fancy/Fast calculation way
  if (m_toggleCalculationMode) {
    m_fancyCalculationMode = m_fancyCalculationMode ? false : true; // Fast/Fancy calculation
    updateSimulationStateInfo();
    m_toggleCalculationMode = false;
  }
  // turn advanced information on/off
  if (m_toggleAdvancedInfo) {
    m_hideAdvancedInfo = m_hideAdvancedInfo ? false : true; // show/hide advanced info
    m_toggleAdvancedInfo = false;
  }
  // 60 updates per second limit On/Off
  if (m_toggleUpdateMode) {
    m_useUpdateLimiter = m_useUpdateLimiter ? false : true; // Uncapped/60 updates per second
    updateSimulationStateInfo();
    m_toggleUpdateMode = false;
  }
  // Show/Hide Axis' & Boundaries
  if (m_toggleHide) {
    if (!m_hideAxis && !m_hideBoundaries) { // both on
      m_hideBoundaries = true; // hide boundaries (axis on)
    } else if (!m_hideAxis && m_hideBoundaries) { // axis on
      m_hideAxis = true; // hide axis (both off)
    } else if (m_hideAxis && m_hideBoundaries) { // both off
      m_hideBoundaries = false; // show boundaries (boundaries on)
    } else if (m_hideAxis && !m_hideBoundaries) { // boundaries on
      m_hideAxis = false; // show axis (both on)
    }
    m_toggleHide = false;
  }
  // Erase Current Set
  if (m_eraseCurrentSet) {
    if (m_singlePairMode) m_singlePairTrashed = true;
    else m_multiPairTrashed = true;
    m_eraseCurrentSet = false;
  }
  // Generate New Set of Lines
  if (m_genNewSetOfLines) {
    if (m_singlePairMode) {
      genNewLine(m_physicalLineA);
      genNewLine(m_physicalLineB);
      updateLine(m_logicalLineA, m_physicalLineA);
      updateLine(m_logicalLineB, m_physicalLineB);
      if (m_singlePairTrashed) m_singlePairTrashed = false;
      if (m_drawSinglePairIntersectionNaive) m_drawSinglePairIntersectionNaive = false;
      if (m_drawSinglePairIntersectionSweep) m_drawSinglePairIntersectionSweep = false;
      updateLinesInfo();
    } else {
      m_physicalMultiPairSet.clear();
      m_logicalMultiPairSet.clear();
      for (int i = 0; i < m_multiPairSetSize; i++) {
        line_segment pLine{
          (float)m_distribution(m_generator),
          (float)m_distribution(m_generator),
          (float)m_distribution(m_generator),
          (float)m_distribution(m_generator)
        };
        sf::VertexArray lLine{ sf::Lines, 2 };
        updateLine(lLine, pLine);
        m_physicalMultiPairSet.push_back(pLine);
        m_logicalMultiPairSet.push_back(lLine);
      }
      if (m_multiPairTrashed) m_multiPairTrashed = false;
      if (m_drawMultiPairIntersectionsNaive) m_drawMultiPairIntersectionsNaive = false;
      if (m_currentlyCalculatingIntersectionsNaive) m_currentlyCalculatingIntersectionsNaive = false;
      if (m_drawMultiPairIntersectionsSweep) m_drawMultiPairIntersectionsSweep = false;
      if (m_currentlyCalculatingIntersectionsSweep) m_currentlyCalculatingIntersectionsSweep = false;
    }
    m_genNewSetOfLines = false;
  }
  // Calculate Intersections (Naive Algorithm)
  if (m_calcIntersectionsNaive) {
    if (m_singlePairMode) {
      if (!m_singlePairTrashed) {
        if (m_physicalLineA.intersects(m_physicalLineB, m_physicalIntersectionPoint)) {
          updatePoint(m_logicalIntersectionPoint, m_physicalIntersectionPoint);
          m_drawSinglePairIntersectionNaive = true;
          updateIntersectionPointInfo();
        }
      }
    } else {
      if (!m_multiPairTrashed) {
        // reset all buffers for recalculation
        m_physicalMultiPairIntersectionPointsNaive.clear();
        m_logicalMultiPairIntersectionPointsNaive.clear();
        if (m_fancyCalculationMode) { // Fancy calculation mode
          m_naiveIterI = 0;
          m_naiveIterJ = 0;
          m_drawMultiPairIntersectionsNaive = true;
          m_currentlyCalculatingIntersectionsNaive = true;
        } else { // fast calculation mode
          for (int i = 0; i < m_multiPairSetSize; i++) {
            for (int j = 0; j < m_multiPairSetSize; j++) {
              if (i == j) break;

              point pIntPoint;
              if (m_physicalMultiPairSet[i].intersects(m_physicalMultiPairSet[j], pIntPoint)) {
                sf::CircleShape lIntPoint;
                lIntPoint.setRadius(intersectionPointsRadius);
                lIntPoint.setFillColor(sf::Color::Green);
                updatePoint(lIntPoint, pIntPoint);
                m_logicalMultiPairIntersectionPointsNaive.push_back(lIntPoint);
                m_drawMultiPairIntersectionsNaive = true;
              }
            }
          }
        }
      }
    }
    m_calcIntersectionsNaive = false;
  }
  // calculate Intersections frame by frame in multi pair simulation
  if (m_currentlyCalculatingIntersectionsNaive) {
    if (m_naiveIterI != m_naiveIterJ) {
      point pIntPoint;
      if (m_physicalMultiPairSet.at(m_naiveIterI).intersects(m_physicalMultiPairSet.at(m_naiveIterJ), pIntPoint)) {
        sf::CircleShape lIntPoint;
        lIntPoint.setRadius(intersectionPointsRadius);
        lIntPoint.setFillColor(sf::Color::Green);
        updatePoint(lIntPoint, pIntPoint);
        m_logicalMultiPairIntersectionPointsNaive.push_back(lIntPoint);
      }
    }
    m_naiveIterJ++;
    if (m_naiveIterJ == m_multiPairSetSize) {
      m_naiveIterJ = 0;
      m_naiveIterI++;
    }
    if (m_naiveIterI == m_multiPairSetSize) m_currentlyCalculatingIntersectionsNaive = false;
  }
  // calculate intersections (sweep line algorithm)
  if (m_calcIntersectionsSweep) {
    if (m_singlePairMode && !m_singlePairTrashed || !m_singlePairMode && !m_multiPairTrashed) {
      m_physicalMultiPairIntersectionPointsSweep.clear();
      m_logicalMultiPairIntersectionPointsSweep.clear();
      // RESET SWEEP LINE #########################################################################
      if (m_singlePairMode) m_drawSinglePairIntersectionSweep = true;
      else m_drawMultiPairIntersectionsSweep = true;
      m_currentlyCalculatingIntersectionsSweep = true;
    }
    m_calcIntersectionsSweep = false;
  }
  // sweep line frame by frame
  if (m_currentlyCalculatingIntersectionsSweep) {
    // SWEEP LINE ++ (FOR STEP EQUAL TO g_precision) ##############################################
  }
  return;
}

void euclidean_plane::render() {
  m_window.clear();
  if (!m_hideAdvancedInfo) {
    m_window.draw(m_statisticsText);
    m_window.draw(m_simulationModeText);
    m_window.draw(m_calculationModeText);
    m_window.draw(m_updateModeText);
    if(!m_singlePairMode) m_window.draw(m_multiPairSetSizeText);
  }
  if (!m_hideAxis) {
    m_window.draw(m_xAxis);
    m_window.draw(m_yAxis);
  }
  if (!m_hideBoundaries) {
    m_window.draw(m_leftBoundary);
    m_window.draw(m_topBoundary);
    m_window.draw(m_rightBoundary);
    m_window.draw(m_botBoundary);
  }
  if (m_singlePairMode) {
    if (!m_singlePairTrashed) {
      m_window.draw(m_logicalLineA);
      m_window.draw(m_logicalLineB);
      m_window.draw(m_lineAText);
      m_window.draw(m_lineApText);
      m_window.draw(m_lineAqText);
      m_window.draw(m_lineAslopeText);
      m_window.draw(m_lineAyInterceptText);
      m_window.draw(m_lineBText);
      m_window.draw(m_lineBpText);
      m_window.draw(m_lineBqText);
      m_window.draw(m_lineBslopeText);
      m_window.draw(m_lineByInterceptText);
      if (m_drawSinglePairIntersectionNaive) {
        m_window.draw(m_logicalIntersectionPoint);
        m_window.draw(m_intersectionPointText);
      }
    }
  } else {
    if (!m_multiPairTrashed) {
      for (sf::VertexArray lineSegment : m_logicalMultiPairSet) {
        m_window.draw(lineSegment);
      }
      if (m_drawMultiPairIntersectionsNaive) {
        for (sf::CircleShape intersectionPoint : m_logicalMultiPairIntersectionPointsNaive) {
          m_window.draw(intersectionPoint);
        }
      }
    }
  }
  m_window.display();
  return;
}

void euclidean_plane::handleUserInput(sf::Keyboard::Key key, bool isPressed) {
  // Check while in Any mode
  if (key == sf::Keyboard::Escape && isPressed) m_exit = true;
  if (key == sf::Keyboard::Space && isPressed) m_genNewSetOfLines = true;
  if (key == sf::Keyboard::Tilde && isPressed) m_toggleUpdateMode = true;
  if (key == sf::Keyboard::R && isPressed) m_reset = true;
  if (key == sf::Keyboard::C && isPressed) m_calcIntersectionsNaive = true;
  if (key == sf::Keyboard::S && isPressed) m_calcIntersectionsSweep = true;
  if (key == sf::Keyboard::H && isPressed) m_toggleHide = true;
  if (key == sf::Keyboard::T && isPressed) m_toggleSimulationMode = true;
  if (key == sf::Keyboard::E && isPressed) m_eraseCurrentSet = true;
  if (key == sf::Keyboard::F && isPressed) m_toggleCalculationMode = true;
  if (key == sf::Keyboard::A && isPressed) m_toggleAdvancedInfo = true;
  // Check only when in Multi Pair mode
  if (!m_singlePairMode) {
    if (key == sf::Keyboard::Up && isPressed) updateMultiPairSetSize(true);
    if (key == sf::Keyboard::Down && isPressed) updateMultiPairSetSize(false);
  }
  return;
}

void euclidean_plane::genNewLine(line_segment& physicalLine) {
  physicalLine.update(
    (float)m_distribution(m_generator),
    (float)m_distribution(m_generator),
    (float)m_distribution(m_generator),
    (float)m_distribution(m_generator)
  );
  return;
}

void euclidean_plane::updateLine(sf::VertexArray& logicalLine, line_segment& physicalLine) {
  logicalLine[0].position = sf::Vector2f(physicalLine.p.x + xBias, yBias - physicalLine.p.y);
  logicalLine[1].position = sf::Vector2f(physicalLine.q.x + xBias, yBias - physicalLine.q.y);
  return;
}

void euclidean_plane::updatePoint(sf::CircleShape& intersectionPoint, point& k) {
  intersectionPoint.setPosition(k.x + xBias - intersectionPointsRadius, yBias - k.y - intersectionPointsRadius);
  return;
}

void euclidean_plane::updateLinesInfo() {
  m_lineAText.setString("Line A = ");
  m_lineApText.setString("p(" + toString(m_physicalLineA.p.x) + ", " + toString(m_physicalLineA.p.y) + "),");
  m_lineAqText.setString("q(" + toString(m_physicalLineA.q.x) + ", " + toString(m_physicalLineA.q.y) + "),");
  m_lineAslopeText.setString("Slope = " + toString(m_physicalLineA.slope));
  m_lineAyInterceptText.setString("y-intercept = " + toString(m_physicalLineA.yIntercept));
  m_lineBText.setString("Line B = ");
  m_lineBpText.setString("p(" + toString(m_physicalLineB.p.x) + ", " + toString(m_physicalLineB.p.y) + "),");
  m_lineBqText.setString("q(" + toString(m_physicalLineB.q.x) + ", " + toString(m_physicalLineB.q.y) + "),");
  m_lineBslopeText.setString("Slope = " + toString(m_physicalLineB.slope));
  m_lineByInterceptText.setString("y-intercept = " + toString(m_physicalLineB.yIntercept));
  return;
}

void euclidean_plane::updateIntersectionPointInfo() {
  m_intersectionPointText.setString("i(" + toString(m_physicalIntersectionPoint.x) + ", " + toString(m_physicalIntersectionPoint.y) + ")");
  return;
}

void euclidean_plane::updateStatistics(sf::Time elapsedTime) {
  m_statisticsUpdateTime += elapsedTime;
  m_statisticsNumFrames += 1;
  if (m_statisticsUpdateTime >= sf::seconds(1.0f)) {
    m_statisticsText.setString(
      "Frames / Second = " + toString(m_statisticsNumFrames) + "\n" +
      "Time / Update = " +
      toString(m_statisticsUpdateTime.asMicroseconds() / m_statisticsNumFrames) + "us"
    );
    m_statisticsUpdateTime -= sf::seconds(1.0f);
    m_statisticsNumFrames = 0;
  }
  return;
}

void euclidean_plane::updateSimulationStateInfo() {
  // Single/Multi pair mode
  if (m_singlePairMode) {
    m_simulationModeText.setString(singlePairModeText);
  } else {
    m_simulationModeText.setString(multiPairModeText);
  }
  // Fancy/Fast calculation mode
  if (m_fancyCalculationMode) {
    m_calculationModeText.setString(fancyCalculationModeText);
  } else {
    m_calculationModeText.setString(fastCalculationModeText);
  }
  // Use/Disable update limit
  if (m_useUpdateLimiter) {
    m_updateModeText.setString(updateLimiterOnText);
  } else {
    m_updateModeText.setString(updateLimiterOffText);
  }
  // Multi Pair Set Size
  m_multiPairSetSizeText.setString("Number of Line\nSegments: " + toString(m_multiPairSetSize));
}

void euclidean_plane::updateMultiPairSetSize(bool increase) {
  if (increase) {
    if (m_multiPairSetSize < multiPairSetSizeThreshold) {
      m_multiPairSetSize++;
      updateSimulationStateInfo();
      m_genNewSetOfLines = true;
    }
  } else {
    if (m_multiPairSetSize > 3) {
      m_multiPairSetSize--;
      updateSimulationStateInfo();
      m_genNewSetOfLines = true;
    }
  }
}

float euclidean_plane::orientation(point a, point b, point c) {
  return (b.y - a.y) * (c.x - b.x) - (c.y - b.y) * (b.x - a.x);
}

template <typename T>
std::string euclidean_plane::toString(const T& value) {
  std::stringstream stream;
  stream << value;
  return stream.str();
}
