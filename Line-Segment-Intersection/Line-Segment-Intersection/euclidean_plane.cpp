
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
// Boost Library
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/find.hpp>
// SFML : Simple and Fast Multimedia Library
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
// Custom Headers
#include "global.h"
#include "point.h"
#include "line_segment.h"
#include "euclidean_plane.h"

#include "event_point.h"
#include "event_queue.h"
#include "status_structure.h"

// # ### #################################################################################### ### #

// Constructors & Destructor
euclidean_plane::euclidean_plane()
  : m_window(sf::VideoMode(windowWidth, windowHeight), "Line Segment Intersection Driver")
  , m_physicalLineA{}, m_physicalLineB{}
  , m_physicalSinglePairSet{ &m_physicalLineA, &m_physicalLineB }
  , m_physicalIntersectionPointNaive{}, m_physicalIntersectionPointSweep{}
  , m_xAxis{ sf::Lines, 2 }, m_yAxis{ sf::Lines, 2 }
  , m_leftBoundary{ sf::Lines, 2 }, m_topBoundary{ sf::Lines, 2 }
  , m_rightBoundary{ sf::Lines, 2 }, m_botBoundary{ sf::Lines, 2 }
  , m_logicalLineA{ sf::Lines, 2 }, m_logicalLineB{ sf::Lines, 2 }
  , m_logicalIntersectionPointNaive{}, m_logicalIntersectionPointSweep{}
  , m_physicalMultiPairSet{}, m_logicalMultiPairSet{}
  , m_physicalMultiPairIntersectionPointsNaive{}, m_physicalMultiPairIntersectionPointsSweep{}
  , m_logicalMultiPairIntersectionPointsNaive{}, m_logicalMultiPairIntersectionPointsSweep{}
  , m_physicalSweep{}, m_logicalSweep{ sf::Lines, 2 }
  , m_font{}, m_statisticsText{}
  , m_lineAText{}, m_lineApText{}, m_lineAqText{}, m_lineAslopeText{}
  , m_lineBText{}, m_lineBpText{}, m_lineBqText{}, m_lineBslopeText{}
  , m_naiveIntersectionPointText{}, m_sweepIntersectionPointText{}, m_simulationModeText{}
  , m_calculationModeText{}, m_updateModeText{}, m_multiPairSetSizeText{}
  , m_naiveIntersectionsHiddenText{}, m_sweepIntersectionsHiddenText{}
  , m_naiveIntersectionsFoundText{}, m_sweepIntersectionsFoundText{}
  , m_naiveTimeText{}, m_sweepTimeText{}, m_timeRecordingModeText{}
  , m_statisticsUpdateTime{}, m_statisticsNumFrames{ 0 }
  , m_naiveClock{}, m_sweepClock{}, m_naiveTime{}, m_sweepTime{}
  , m_genNewSetOfLines{ false }, m_reset{ false }, m_exit{ false }
  , m_calcIntersectionsNaive{ false }, m_calcIntersectionsSweep{ false }
  , m_toggleSimulationMode{ false }, m_toggleHide{ false }, m_eraseCurrentSet{ false }
  , m_toggleCalculationMode{ false }, m_toggleUpdateMode{ false }, m_toggleAdvancedInfo{ false }
  , m_toggleNaiveIntersections{ false }, m_toggleSweepIntersections{ false }
  , m_toggleTimeRecording{ false }
  , m_toggleReadWriteMode{ false }, m_executeReadWriteOperation{ false }
  , m_runTestCode{ false }
  , m_drawSinglePairIntersectionNaive{ false }, m_drawSinglePairIntersectionSweep{ false }
  , m_drawMultiPairIntersectionsNaive{ false }, m_drawMultiPairIntersectionsSweep{ false }
  , m_hideAxis{ false }, m_hideBoundaries{ false }, m_hideAdvancedInfo{ false }
  , m_hideNaiveIntersections{ false }, m_hideSweepIntersections{ false }
  , m_drawSinglePairSweepLine{ false }, m_drawMultiPairSweepLine{ false }
  , m_singlePairMode{ true }, m_singlePairTrashed{ true }, m_multiPairTrashed{ true }
  , m_fancyCalculationMode{ true }, m_lastSweptSinglePair{ true }, m_useUpdateLimiter{ false }
  , m_currentlyCalculatingIntersectionsNaive{ false }, m_currentlyCalculatingIntersectionsSweep{ false }
  , m_currentlyInReadMode{ true }, m_calcTime{ true }
  , m_drawNaiveTime{ false }, m_drawSweepTime{ false }
  , m_multiPairSetSize{ 25 }, m_naiveIterI{ 0 }, m_naiveIterJ{ 0 }, m_distribution{ distMin, distMax }
  , m_saveFileName{ "saves/multiple-pair-line-segment-set-" }, m_saveFileNumber{}, m_saveFileExtension{ ".dat" }
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

  m_logicalSweep[0].color = sf::Color::Magenta;
  m_logicalSweep[1].color = sf::Color::Magenta;

  m_logicalIntersectionPointNaive.setRadius(intersectionPointsRadius);
  m_logicalIntersectionPointNaive.setFillColor(sf::Color::Green);
  m_logicalIntersectionPointNaive.setPointCount(10);

  m_logicalIntersectionPointSweep.setRadius(intersectionPointsRadius);
  m_logicalIntersectionPointSweep.setFillColor(sf::Color::Green);
  m_logicalIntersectionPointSweep.setPointCount(10);

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
  m_naiveIntersectionPointText.setFont(m_font);
  m_sweepIntersectionPointText.setFont(m_font);
  m_simulationModeText.setFont(m_font);
  m_calculationModeText.setFont(m_font);
  m_updateModeText.setFont(m_font);
  m_multiPairSetSizeText.setFont(m_font);
  m_naiveIntersectionsHiddenText.setFont(m_font);
  m_sweepIntersectionsHiddenText.setFont(m_font);
  m_naiveIntersectionsFoundText.setFont(m_font);
  m_sweepIntersectionsFoundText.setFont(m_font);
  m_naiveTimeText.setFont(m_font);
  m_sweepTimeText.setFont(m_font);
  m_timeRecordingModeText.setFont(m_font);

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
  m_naiveIntersectionPointText.setPosition(5.f, yBias - 40.f);
  m_sweepIntersectionPointText.setPosition(windowWidth - 270.f, yBias - 40.f);
  m_simulationModeText.setPosition(windowWidth - 270.f, windowHeight - 70.f);
  m_calculationModeText.setPosition(windowWidth - 270.f, windowHeight - 40.f);
  m_updateModeText.setPosition(windowWidth - 270.f, 5.f);
  m_multiPairSetSizeText.setPosition(xBias - 80.f, 5.f);
  m_naiveIntersectionsHiddenText.setPosition(5.f, yBias + 20.f);
  m_sweepIntersectionsHiddenText.setPosition(windowWidth - 270.f, yBias + 20.f);
  m_naiveIntersectionsFoundText.setPosition(5.f, yBias - 190.f);
  m_sweepIntersectionsFoundText.setPosition(windowWidth - 270.f, yBias - 190.f);
  m_naiveTimeText.setPosition(5.f, yBias - 100.f);
  m_sweepTimeText.setPosition(windowWidth - 270.f, yBias - 100.f);
  m_timeRecordingModeText.setPosition(windowWidth - 150.f, windowHeight - 40.f);
  
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
  m_naiveIntersectionPointText.setCharacterSize(fontSize);
  m_sweepIntersectionPointText.setCharacterSize(fontSize);
  m_simulationModeText.setCharacterSize(fontSize);
  m_calculationModeText.setCharacterSize(fontSize);
  m_updateModeText.setCharacterSize(fontSize);
  m_multiPairSetSizeText.setCharacterSize(fontSize);
  m_naiveIntersectionsHiddenText.setCharacterSize(fontSize);
  m_sweepIntersectionsHiddenText.setCharacterSize(fontSize);
  m_naiveIntersectionsFoundText.setCharacterSize(fontSize);
  m_sweepIntersectionsFoundText.setCharacterSize(fontSize);
  m_naiveTimeText.setCharacterSize(fontSize);
  m_sweepTimeText.setCharacterSize(fontSize);
  m_timeRecordingModeText.setCharacterSize(fontSize);

  m_lineApText.setFillColor(sf::Color::Red);
  m_lineAqText.setFillColor(sf::Color::Yellow);
  m_lineBpText.setFillColor(sf::Color::Blue);
  m_lineBqText.setFillColor(sf::Color::Cyan);
  m_naiveIntersectionPointText.setFillColor(sf::Color::Green);
  m_sweepIntersectionPointText.setFillColor(sf::Color::Magenta);
  m_naiveIntersectionsHiddenText.setFillColor(sf::Color::Green);
  m_sweepIntersectionsHiddenText.setFillColor(sf::Color::Magenta);
  m_naiveIntersectionsFoundText.setFillColor(sf::Color::Green);
  m_sweepIntersectionsFoundText.setFillColor(sf::Color::Magenta);
  m_naiveTimeText.setFillColor(sf::Color::Green);
  m_sweepTimeText.setFillColor(sf::Color::Magenta);

  m_naiveIntersectionsHiddenText.setString("Naive Algorithm\nIntersections: Hidden");
  m_sweepIntersectionsHiddenText.setString("Sweep Line Algorithm\nIntersections: Hidden");

  updateSimulationStateInfo();
}

void euclidean_plane::run() {
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  while (m_window.isOpen()) {
    sf::Time elapsedTime = clock.restart();
    if (m_useUpdateLimiter) {
      timeSinceLastUpdate += elapsedTime;
      while (timeSinceLastUpdate > timePerUpdate) {
        timeSinceLastUpdate -= timePerUpdate;
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
  // toggle naive algorithm intersections on or off
  if (m_toggleNaiveIntersections) {
    m_hideNaiveIntersections = m_hideNaiveIntersections ? false : true;
    m_toggleNaiveIntersections = false;
  }
  // toggle sweep line algorithm intersections on or off
  if (m_toggleSweepIntersections) {
    m_hideSweepIntersections = m_hideSweepIntersections ? false : true;
    m_toggleSweepIntersections = false;
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
  // toggle read/write mode
  if (m_toggleReadWriteMode) {
    if (m_currentlyInReadMode) m_currentlyInReadMode = false;
    else m_currentlyInReadMode = true;
    m_toggleReadWriteMode = false;
    if (m_currentlyInReadMode) std::cout << "read mode\n";
    else std::cout << "write mode\n";
  }
  // toggle time recording on/off
  if (m_toggleTimeRecording) {
    if (m_calcTime) m_calcTime = false;
    else m_calcTime = true;
    updateSimulationStateInfo();
    m_toggleTimeRecording = false;
  }
  // read write save files
  if (m_executeReadWriteOperation) {
    readWriteMultiPairSetToFile();
    m_executeReadWriteOperation = false;
  }
  // Erase Current Set
  if (m_eraseCurrentSet) {
    // trash the right set
    if (m_singlePairMode) m_singlePairTrashed = true;
    else m_multiPairTrashed = true;
    // if any calculation process is currently active, stop it
    m_currentlyCalculatingIntersectionsNaive = false;
    m_currentlyCalculatingIntersectionsSweep = false;
    // reset erase bool
    m_eraseCurrentSet = false;
  }
  // Generate New Set of Lines
  if (m_genNewSetOfLines) {
    m_physicalLineA.resetID(); // reset unique ids
    if (m_singlePairMode) {
      genNewLine(m_physicalLineA);
      genNewLine(m_physicalLineB);
      updateLine(m_logicalLineA, m_physicalLineA);
      updateLine(m_logicalLineB, m_physicalLineB);
      if (m_singlePairTrashed) m_singlePairTrashed = false;
      if (m_drawSinglePairIntersectionNaive) m_drawSinglePairIntersectionNaive = false;
      if (m_drawSinglePairIntersectionSweep) m_drawSinglePairIntersectionSweep = false;
      if (m_drawSinglePairSweepLine) m_drawSinglePairSweepLine = false;
      if (m_currentlyCalculatingIntersectionsSweep && m_lastSweptSinglePair) m_currentlyCalculatingIntersectionsSweep = false;
      updateLinesInfo();
    } else {
      m_physicalMultiPairSet.clear();
      m_logicalMultiPairSet.clear();
      for (int i = 0; i < m_multiPairSetSize; i++) {
        line_segment pLine{}; // create temp line
        genNewLine(pLine); // update temp lines end points
        sf::VertexArray lLine{ sf::Lines, 2 }; // create temp logical line
        updateLine(lLine, pLine); // update logical line based on the physical line
        m_physicalMultiPairSet.push_back(pLine); // add physical line to set
        m_logicalMultiPairSet.push_back(lLine); // add logical line to set
      }
      if (m_multiPairTrashed) m_multiPairTrashed = false;
      if (m_drawMultiPairIntersectionsNaive) m_drawMultiPairIntersectionsNaive = false;
      if (m_drawMultiPairIntersectionsSweep) m_drawMultiPairIntersectionsSweep = false;
      if (m_drawMultiPairSweepLine) m_drawMultiPairSweepLine = false;
      if (m_currentlyCalculatingIntersectionsNaive) m_currentlyCalculatingIntersectionsNaive = false;
      if (m_currentlyCalculatingIntersectionsSweep && !m_lastSweptSinglePair) m_currentlyCalculatingIntersectionsSweep = false;
      if (m_drawNaiveTime) m_drawNaiveTime = false;
      if (m_drawSweepTime) m_drawSweepTime = false;
    }
    m_genNewSetOfLines = false;
  }
  // Calculate Intersections (Naive Algorithm)
  if (m_calcIntersectionsNaive) {
    if (m_singlePairMode) {
      if (!m_singlePairTrashed) {
        if (m_physicalLineA.intersects(m_physicalLineB, m_physicalIntersectionPointNaive)) {
          updatePoint(m_logicalIntersectionPointNaive, m_physicalIntersectionPointNaive);
          m_drawSinglePairIntersectionNaive = true;
          updateNaiveIntersectionPointInfo();
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
          if (m_calcTime) m_naiveTime = sf::Time::Zero;
          else m_drawNaiveTime = false;
        } else { // fast calculation mode
          if (m_calcTime) { // calculate time
            m_naiveTime = sf::Time::Zero;
            m_naiveClock.restart();
          } else { // value in m_naiveTime no longer corresponds to current naive intersection set
            m_drawNaiveTime = false;
          }
          std::vector<bool> pLineDone(m_multiPairSetSize, false);
          for (int i = 0; i < m_multiPairSetSize; i++) {
            for (int j = 0; j < m_multiPairSetSize; j++) {
              if (i == j) continue;
              else if (pLineDone.at(j)) continue;
              point pIntPoint;
              if (m_physicalMultiPairSet[i].intersects(m_physicalMultiPairSet[j], pIntPoint)) {
                if (!containsPoint(m_physicalMultiPairIntersectionPointsNaive, pIntPoint)) {
                  m_physicalMultiPairIntersectionPointsNaive.push_back(pIntPoint);
                  sf::CircleShape lIntPoint;
                  lIntPoint.setPointCount(10);
                  lIntPoint.setRadius(intersectionPointsRadius);
                  lIntPoint.setFillColor(sf::Color::Green);
                  updatePoint(lIntPoint, pIntPoint);
                  m_logicalMultiPairIntersectionPointsNaive.push_back(lIntPoint);
                  m_drawMultiPairIntersectionsNaive = true;
                }
              }
            }
            pLineDone.at(i) = true;
          }
          if (m_calcTime) { // calculate time
            m_naiveTime = m_naiveClock.getElapsedTime(); // get time since the start of the naive algorithm
            sf::Int64 timeMicro{ m_naiveTime.asMicroseconds() }; // get time as micro seconds
            float timeSec{ m_naiveTime.asSeconds() }; // get time as seconds
            m_naiveTimeText.setString("Naive Algorithm\nTime:\n" + toString(timeMicro) + " ms or\n" + toString(timeSec) + " s");
            m_drawNaiveTime = true; // draw said time on screen
          }
          m_naiveIntersectionsFoundText.setString("Naive Algorithm\nIntersections Found:\n" + toString(m_physicalMultiPairIntersectionPointsNaive.size()));
        }
      }
    }
    m_calcIntersectionsNaive = false;
  }
  // calculate intersections frame by frame in multi pair simulation (naive - fancy)
  if (m_currentlyCalculatingIntersectionsNaive) {
    if (m_calcTime) m_naiveClock.restart(); // restart the clock to get current "loop's" time
    if (m_naiveIterI != m_naiveIterJ) {
      point pIntPoint;
      if (m_physicalMultiPairSet.at(m_naiveIterI).intersects(m_physicalMultiPairSet.at(m_naiveIterJ), pIntPoint)) {
        if (!containsPoint(m_physicalMultiPairIntersectionPointsNaive, pIntPoint)) {
          m_physicalMultiPairIntersectionPointsNaive.push_back(pIntPoint);
          sf::CircleShape lIntPoint;
          lIntPoint.setPointCount(10);
          lIntPoint.setRadius(intersectionPointsRadius);
          lIntPoint.setFillColor(sf::Color::Green);
          updatePoint(lIntPoint, pIntPoint);
          m_logicalMultiPairIntersectionPointsNaive.push_back(lIntPoint);
        }
      }
    }
    m_naiveIterJ++;
    if (m_naiveIterJ == m_multiPairSetSize) {
      m_naiveIterJ = 0;
      m_naiveIterI++;
    }
    if (m_calcTime) m_naiveTime += m_naiveClock.getElapsedTime();
    m_naiveIntersectionsFoundText.setString("Naive Algorithm\nIntersections Found:\n" + toString(m_physicalMultiPairIntersectionPointsNaive.size())); // number of intersections found
    if (m_naiveIterI == m_multiPairSetSize) { // calculation is over
      if (m_calcTime) {
        sf::Int64 timeMicro{ m_naiveTime.asMicroseconds() }; // get time as micro seconds
        float timeSec{ m_naiveTime.asSeconds() }; // get time as seconds
        m_naiveTimeText.setString("Naive Algorithm\nTime:\n" + toString(timeMicro) + " ms or\n" + toString(timeSec) + " s");
        m_drawNaiveTime = true; // draw said time on screen
      }
      m_currentlyCalculatingIntersectionsNaive = false;
    }
  }
  // calculate intersections (sweep line algorithm)
  if (m_calcIntersectionsSweep) {
    if (m_singlePairMode) {
      if (!m_singlePairTrashed) {
        m_lastSweptSinglePair = true;
        m_physicalSweep.reset();
        m_drawSinglePairIntersectionSweep = false;
        m_drawMultiPairSweepLine = false;
        if (m_fancyCalculationMode) {
           m_drawSinglePairIntersectionSweep = true;
          // this goes to the point where the sweep line actually finds the intersection point
          // (if there is one) BUT NOT HERE
          // THIS IS TEMPORARY THIS IS TEMPORARY THIS IS TEMPORARY THIS IS TEMPORARY THIS IS TEMPORARY THIS IS TEMPORARY THIS IS TEMPORARY
          m_physicalIntersectionPointSweep.update(0, 0); // TEMP TEST
          m_drawSinglePairIntersectionSweep = true; // TEMP TEST
          updateSweepIntersectionPointInfo(); // THIS GOES INSIDE THE SWEEP LINE CLASS WHEN IT IS NEEDED
          // THIS IS TEMPORARY THIS IS TEMPORARY THIS IS TEMPORARY THIS IS TEMPORARY THIS IS TEMPORARY THIS IS TEMPORARY THIS IS TEMPORARY
          m_drawSinglePairSweepLine = true;
          m_currentlyCalculatingIntersectionsSweep = true;
        } else {
          while (!m_physicalSweep.reachedEnd()) {
            // PROCESS CURRENT STEP
            // UPDATE m_physicalIntersectionPointSweep IF NEEDED
            // UPDATE m_logicalIntersectionPointSweep IF NEEDED
            m_physicalSweep.advance();
          }
        }
      }
    } else {
      if (!m_multiPairTrashed) {
        m_lastSweptSinglePair = false;
        m_physicalMultiPairIntersectionPointsSweep.clear();
        m_logicalMultiPairIntersectionPointsSweep.clear();
        m_physicalSweep.reset();
        updateSweep(m_logicalSweep, m_physicalSweep);
        m_physicalSweep.Q.changeSet(m_physicalMultiPairSet);
        m_physicalSweep.Q.clear();
        m_physicalSweep.Q.initialize();
        m_physicalSweep.T.clear();
        //debuggingUnionCounter = 0; // TEMP TEST TEMP TEST TEMP TEST
        //system("CLS"); // TEMP TEST TEMP TEST TEMP TEST
        m_drawMultiPairIntersectionsSweep = false;
        m_drawSinglePairSweepLine = false;
        if (m_fancyCalculationMode) { // fancy calculation mode
          m_drawMultiPairIntersectionsSweep = true;
          m_drawMultiPairSweepLine = true;
          m_currentlyCalculatingIntersectionsSweep = true;
          if (m_calcTime) m_sweepTime = sf::Time::Zero;
          else m_drawSweepTime = false;
        } else { // fast calculation mode
          if (m_calcTime) { // calculate time
            m_sweepTime = sf::Time::Zero;
            m_sweepClock.restart();
          } else { // value in m_naiveTime no longer corresponds to current naive intersection set
            m_drawSweepTime = false;
          }
          while (!m_physicalSweep.Q.empty()) {
            m_physicalSweep.advance(m_physicalSweep.Q.nextEventPointPosition());
            point pIntPoint{};
            if (m_physicalSweep.handleEventPoint(m_physicalSweep.Q.getNextEventPoint(), pIntPoint)) {
              if (!containsPoint(m_physicalMultiPairIntersectionPointsSweep, pIntPoint)) {
                m_physicalMultiPairIntersectionPointsSweep.push_back(pIntPoint); // get next ep, if it is an intersection point add it, if not do nothing
                sf::CircleShape lIntPoint;
                lIntPoint.setPointCount(10);
                lIntPoint.setRadius(intersectionPointsRadius);
                lIntPoint.setFillColor(sf::Color::Magenta);
                updatePoint(lIntPoint, pIntPoint);
                m_logicalMultiPairIntersectionPointsSweep.push_back(lIntPoint);
                m_drawMultiPairIntersectionsSweep = true;
              }
            }
          }
          if (m_calcTime) { // calculate time
            m_sweepTime = m_sweepClock.getElapsedTime(); // get time since the start of the sweep
            sf::Int64 timeMicro{ m_sweepTime.asMicroseconds() }; // get time as micro seconds
            float timeSec{ m_sweepTime.asSeconds() }; // get time as seconds
            m_sweepTimeText.setString("Sweep Line Algorithm\nTime:\n" + toString(timeMicro) + " ms or\n" + toString(timeSec) + " s");
            m_drawSweepTime = true; // draw said time on screen
          }
          m_sweepIntersectionsFoundText.setString("Sweep Line Algorithm\nIntersections Found:\n" + toString(m_physicalMultiPairIntersectionPointsSweep.size()));
        }
      }
    }
    m_calcIntersectionsSweep = false;
  }
  // sweep line frame by frame (fancy)
  if (m_currentlyCalculatingIntersectionsSweep) {
    if (m_calcTime) m_sweepClock.restart(); // restart the clock to get current "loop's" time
    if (!m_physicalSweep.reachedEnd()) {
      if (!m_physicalSweep.Q.empty()) {
        if (abs(m_physicalSweep.position - m_physicalSweep.Q.nextEventPointPosition()) <= m_physicalSweep.scope) {
          //m_physicalSweep.Q.print(); // TEMP TEST
          //m_physicalSweep.T.print(); // TEMP TEST
          point pIntPoint{};
          if (m_physicalSweep.handleEventPoint(m_physicalSweep.Q.getNextEventPoint(), pIntPoint)) {
            if (!containsPoint(m_physicalMultiPairIntersectionPointsSweep, pIntPoint)) {
              m_physicalMultiPairIntersectionPointsSweep.push_back(pIntPoint); // get next ep, if it is an intersection point add it, if not do nothing
              sf::CircleShape lIntPoint;
              lIntPoint.setPointCount(10);
              lIntPoint.setRadius(intersectionPointsRadius);
              lIntPoint.setFillColor(sf::Color::Magenta);
              updatePoint(lIntPoint, pIntPoint);
              m_logicalMultiPairIntersectionPointsSweep.push_back(lIntPoint);
            }
          }
        } else {
          m_physicalSweep.advance();
          updateSweep(m_logicalSweep, m_physicalSweep);
        }
        //m_physicalSweep.Q.print(); // TEMP TEST print queue after new event point has been popped
      } else {
        m_physicalSweep.advance();
        updateSweep(m_logicalSweep, m_physicalSweep);
      }
      if(m_calcTime) m_sweepTime += m_sweepClock.getElapsedTime(); // get time since the start of the sweep
      m_sweepIntersectionsFoundText.setString("Sweep Line Algorithm\nIntersections Found:\n" + toString(m_physicalMultiPairIntersectionPointsSweep.size())); // number of intersections found
    } else { // calculation is over
      if (m_calcTime) { // calculate time
        sf::Int64 timeMicro{ m_sweepTime.asMicroseconds() }; // get time as micro seconds
        float timeSec{ m_sweepTime.asSeconds() }; // get time as seconds
        m_sweepTimeText.setString("Sweep Line Algorithm\nTime:\n" + toString(timeMicro) + " ms or\n" + toString(timeSec) + " s");
        m_drawSweepTime = true; // draw said time on screen
      }
      m_currentlyCalculatingIntersectionsSweep = false;
      if (m_drawSinglePairSweepLine) m_drawSinglePairSweepLine = false;
      if (m_drawMultiPairSweepLine) m_drawMultiPairSweepLine = false;
    }
  }
  /* # ### ####################################################################################################################### ### # *|
  |* # ### #### Test Zone Start ################################################################################################## ### # *|
  |* # ### ####################################################################################################################### ### # */
  if (m_runTestCode) {
    system("CLS"); // clears the terminal
    /* # ### preprocessor directive - TEST_OPTION ### #
     *  (0): elease Option, nothing happens
     *  (1): event queue feature test
     *  (2): status structure feature test
     *  (3): sweep line feature test (setup)
     */
#define TEST_OPTION 5
    /* _____________________________|___________________________________________________________________________________________________ *|
    |* # ### Write Code Below ### # | |~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|  *|
    |* _____________________________|___________________________________________________________________________________________________ */
#if TEST_OPTION == 1 // EVENT QUEUE FEATURE TEST (V)
    event_queue Q{};
    line_segment lineSet{ 69, -69, 96, -96 };
    std::vector<line_segment> qSet{ lineSet };
    Q.changeSet(qSet);
    Q.clear();
    lineSet.upperEndPoint.print();
    lineSet.lowerEndPoint.print();
    Q.initialize();
    Q.print();

    for (int i = 0; i < 10; i++) {
      point p{ (float)i, (float)i + 10 };
      event_point ep{ p };
      p.print();
      Q.add(ep);
      Q.print();
    }

    point p{ 300, -300 };
    event_point ep{ p };
    p.print();
    Q.add(ep);
    Q.print();

    p.update(300, 300);
    event_point ep2{ p };
    p.print();
    Q.add(ep2);
    Q.print();

    line_segment lineSet2{ 69, -69, 96, -96 };
    lineSet2.upperEndPoint.print();
    Q.add(lineSet2.upperEndPoint);
    Q.print();
#elif TEST_OPTION == 2 // STATUS STRACTURE FEATURE TEST (V)
    point p0{ 0, 300 };
    point p1{ 300, 300 };
    point p2{ 300, -300 };
    point p3{ -300, -300 };
    point p4{ 300, 0 };
    point p5{ -300, 0 };
    point p6{ 0, -300 };
    line_segment l0{ p0, p1 };
    l0.print();
    line_segment l1{ p0, p2 };
    l1.print();
    line_segment l2{ p0, p3 };
    l2.print();
    line_segment l3{ p0, p4 };
    l3.print();
    line_segment l4{ p0, p5 };
    l4.print();
    line_segment l5{ p0, p6 };
    l5.print();

    status_structure T{};
    T.add(&l0);
    T.print();
    T.add(&l1);
    T.print();
    T.add(&l2);
    T.print();
    T.add(&l3);
    T.print();
    T.add(&l4);
    T.print();
    T.add(&l5);
    T.print();

    std::vector<line_segment*> linesToErase1{ &l2, &l4 };
    std::vector<line_segment*> linesToErase2{ &l5 };
    T.erase(linesToErase1, linesToErase2);
    T.print();
#elif TEST_OPTION == 3 // SWEEP LINE FEATURE TEST (X)
    //std::cout << "naive intersection points:\n";
    //for (point p : m_physicalMultiPairIntersectionPointsNaive) {
    //  p.print();
    //}
    //std::cout << "sweep intersection points:\n";
    //for (point p : m_physicalMultiPairIntersectionPointsSweep) {
    //  p.print();
    //}

    m_physicalMultiPairSet.clear();
    m_logicalMultiPairSet.clear();

    line_segment s{};
    sf::VertexArray lLine{ sf::Lines, 2 };
    s.resetID();

    line_segment s0 { -217, 266, 270, -61 }; // s0
    updateLine(lLine, s0);
    m_physicalMultiPairSet.push_back(s0);
    m_logicalMultiPairSet.push_back(lLine);
    std::cout << s0.name << '\n';

    line_segment s1{ -153, 188, -187, -52 }; // s1
    updateLine(lLine, s1);
    m_physicalMultiPairSet.push_back(s1);
    m_logicalMultiPairSet.push_back(lLine);
    std::cout << s1.name << '\n';

    line_segment s2{ -103, 16, 129, -222 }; // s2
    updateLine(lLine, s2);
    m_physicalMultiPairSet.push_back(s2);
    m_logicalMultiPairSet.push_back(lLine);
    std::cout << s2.name << '\n';

    line_segment s3{ -213, 82, -272, 37 }; // s3
    updateLine(lLine, s3);
    m_physicalMultiPairSet.push_back(s3);
    m_logicalMultiPairSet.push_back(lLine);
    std::cout << s3.name << '\n';

    line_segment s4{ 270, 43, -300, 44 }; // s4
    updateLine(lLine, s4);
    m_physicalMultiPairSet.push_back(s4);
    m_logicalMultiPairSet.push_back(lLine);
    std::cout << s4.name << '\n';

    line_segment s5{ 180, 79, 89, 68 }; // s5
    updateLine(lLine, s5);
    m_physicalMultiPairSet.push_back(s5);
    m_logicalMultiPairSet.push_back(lLine);
    std::cout << s5.name << '\n';

    line_segment s6{ -159, -60, -103, 82 }; // s6
    updateLine(lLine, s6);
    m_physicalMultiPairSet.push_back(s6);
    m_logicalMultiPairSet.push_back(lLine);
    std::cout << s6.name << '\n';

    line_segment s7{ 8, -35, 35, -271 }; // s7
    updateLine(lLine, s7);
    m_physicalMultiPairSet.push_back(s7);
    m_logicalMultiPairSet.push_back(lLine);
    std::cout << s7.name << '\n';

    m_multiPairSetSize = m_physicalMultiPairSet.size();
    updateSimulationStateInfo();

    if (m_multiPairTrashed) m_multiPairTrashed = false;
    if (m_drawMultiPairIntersectionsNaive) m_drawMultiPairIntersectionsNaive = false;
    if (m_drawMultiPairIntersectionsSweep) m_drawMultiPairIntersectionsSweep = false;
    if (m_drawMultiPairSweepLine) m_drawMultiPairSweepLine = false;
    if (m_currentlyCalculatingIntersectionsNaive) m_currentlyCalculatingIntersectionsNaive = false;
    if (m_currentlyCalculatingIntersectionsSweep && !m_lastSweptSinglePair) m_currentlyCalculatingIntersectionsSweep = false;

    std::cout << "custom lines created\n";
#elif TEST_OPTION == 4 // PRINT LINE SEGMENT INFO
    for (line_segment lineSeg : m_physicalMultiPairSet) {
      lineSeg.print();
    }
#elif TEST_OPTION == 5 // OTHER
    std::cout << "Naive Points Repeated: " << '\n';
    for (point p : m_physicalMultiPairIntersectionPointsNaive) {
      int counter{ 0 };
      for (point q : m_physicalMultiPairIntersectionPointsNaive) {
        if (p.eq(q)) counter++;
      }
      if (counter > 1) {
        std::cout << "_____________\n";
        p.print();
        std::cout << "_____________\n";
      }
    }
    std::cout << "Sweep Points Repeated: " << '\n';
    for (point p : m_physicalMultiPairIntersectionPointsSweep) {
      int counter{ 0 };
      for (point q : m_physicalMultiPairIntersectionPointsSweep) {
        if (p.eq(q)) counter++;
      }
      if (counter > 1) {
        std::cout << "_____________\n";
        p.print();
        std::cout << "_____________\n";
      }
    }
    std::cout << "________________________________ " << m_physicalMultiPairIntersectionPointsNaive.size() << '\n';
    for (point p : m_physicalMultiPairIntersectionPointsNaive) p.print();
    std::cout << "________________________________ " << m_physicalMultiPairIntersectionPointsSweep.size() << '\n';
    for (point p : m_physicalMultiPairIntersectionPointsSweep) p.print();
    std::cout << m_logicalIntersectionPointNaive.getPointCount() << '\n';
#endif
    /* _____________________________|___________________________________________________________________________________________________ *|
    |* # ### Write Code Above ### # | |~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|  *|
    |* _____________________________|___________________________________________________________________________________________________ */
    m_runTestCode = false;
  }
  /* # ### ####################################################################################################################### ### # *|
  |* # ### #### Test Zone End #################################################################################################### ### # *|
  |* # ### ####################################################################################################################### ### # */
  return;
}

void euclidean_plane::render() {
  m_window.clear();
  if (!m_hideAdvancedInfo) {
    m_window.draw(m_statisticsText);
    m_window.draw(m_updateModeText);
    m_window.draw(m_simulationModeText);
    m_window.draw(m_calculationModeText);
    m_window.draw(m_timeRecordingModeText);
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
  if (m_hideNaiveIntersections) m_window.draw(m_naiveIntersectionsHiddenText);
  if (m_hideSweepIntersections) m_window.draw(m_sweepIntersectionsHiddenText);
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
        if (!m_hideNaiveIntersections) m_window.draw(m_logicalIntersectionPointNaive);
        m_window.draw(m_naiveIntersectionPointText);
      }
      if (m_drawSinglePairIntersectionSweep) {
        if (!m_hideSweepIntersections) m_window.draw(m_logicalIntersectionPointSweep);
        m_window.draw(m_sweepIntersectionPointText);
      }
      if (m_drawSinglePairSweepLine) {
        m_window.draw(m_logicalSweep);
      }
    }
  } else {
    if (!m_multiPairTrashed) {
      // draw all line segments
      for (sf::VertexArray lineSegment : m_logicalMultiPairSet) {
        m_window.draw(lineSegment);
      }
      // draw all naive algorithm intersection points
      if (m_drawMultiPairIntersectionsNaive) {
        if (!m_hideNaiveIntersections) {
          for (sf::CircleShape intersectionPoint : m_logicalMultiPairIntersectionPointsNaive) {
            m_window.draw(intersectionPoint);
          }
        }
        m_window.draw(m_naiveIntersectionsFoundText); // draw the number of intersections found (naive)
      }
      // draw naive algorithm time
      if (m_drawNaiveTime) m_window.draw(m_naiveTimeText);
      // draw all sweep line algorithm intersection points
      if (m_drawMultiPairIntersectionsSweep) {
        if (!m_hideSweepIntersections) {
          for (sf::CircleShape intersectionPoint : m_logicalMultiPairIntersectionPointsSweep) {
            m_window.draw(intersectionPoint);
          }
        }
        m_window.draw(m_sweepIntersectionsFoundText); // draw the number of intersections found (sweep)
      }
      // draw sweep line algorithm time
      if (m_drawSweepTime) m_window.draw(m_sweepTimeText);
      // draw the sweep line
      if (m_drawMultiPairSweepLine) {
        m_window.draw(m_logicalSweep);
      }
    }
  }
  m_window.display();
  return;
}

void euclidean_plane::handleUserInput(sf::Keyboard::Key key, bool isPressed) {
  // check while in Any mode
  if (key == sf::Keyboard::Escape && isPressed) m_exit = true;
  if (key == sf::Keyboard::Space && isPressed) m_genNewSetOfLines = true;
  if (key == sf::Keyboard::Q && isPressed) m_toggleUpdateMode = true;
  if (key == sf::Keyboard::W && isPressed) m_toggleSimulationMode = true;
  if (key == sf::Keyboard::E && isPressed) m_eraseCurrentSet = true;
  if (key == sf::Keyboard::R && isPressed) m_reset = true;
  if (key == sf::Keyboard::A && isPressed) m_toggleAdvancedInfo = true;
  if (key == sf::Keyboard::S && isPressed) m_calcIntersectionsSweep = true;
  if (key == sf::Keyboard::D && isPressed) m_toggleHide = true;
  if (key == sf::Keyboard::F && isPressed) m_toggleCalculationMode = true;
  if (key == sf::Keyboard::Z && isPressed) m_toggleNaiveIntersections = true;
  if (key == sf::Keyboard::X && isPressed) m_toggleSweepIntersections = true;
  if (key == sf::Keyboard::C && isPressed) m_calcIntersectionsNaive = true;
  if (key == sf::Keyboard::T && isPressed) m_toggleTimeRecording = true;
  // check only when in Multi Pair mode
  if (!m_singlePairMode) {
    if (key == sf::Keyboard::Up && isPressed) updateMultiPairSetSize(true);
    if (key == sf::Keyboard::Down && isPressed) updateMultiPairSetSize(false);
  }
  // multi pair set saves (0-9)
  if (key == sf::Keyboard::Num0 && isPressed) {
    m_executeReadWriteOperation = true;
    m_saveFileNumber = 0;
  }
  if (key == sf::Keyboard::Num1 && isPressed) {
    m_executeReadWriteOperation = true;
    m_saveFileNumber = 1;
  }
  if (key == sf::Keyboard::Num2 && isPressed) {
    m_executeReadWriteOperation = true;
    m_saveFileNumber = 2;
  }
  if (key == sf::Keyboard::Num3 && isPressed) {
    m_executeReadWriteOperation = true;
    m_saveFileNumber = 3;
  }
  if (key == sf::Keyboard::Num4 && isPressed) {
    m_executeReadWriteOperation = true;
    m_saveFileNumber = 4;
  }
  if (key == sf::Keyboard::Num5 && isPressed) {
    m_executeReadWriteOperation = true;
    m_saveFileNumber = 5;
  }
  if (key == sf::Keyboard::Num6 && isPressed) {
    m_executeReadWriteOperation = true;
    m_saveFileNumber = 6;
  }
  if (key == sf::Keyboard::Num7 && isPressed) {
    m_executeReadWriteOperation = true;
    m_saveFileNumber = 7;
  }
  if (key == sf::Keyboard::Num8 && isPressed) {
    m_executeReadWriteOperation = true;
    m_saveFileNumber = 8;
  }
  if (key == sf::Keyboard::Num9 && isPressed) {
    m_executeReadWriteOperation = true;
    m_saveFileNumber = 9;
  }
  if (key == sf::Keyboard::G && isPressed) m_toggleReadWriteMode = true;
  // test code
  if (key == sf::Keyboard::Numpad0 && isPressed) m_runTestCode = true;
  return;
}

void euclidean_plane::genNewLine(line_segment& physicalLine) {
  physicalLine.update(
    m_distribution(m_generator),
    m_distribution(m_generator),
    m_distribution(m_generator),
    m_distribution(m_generator)
  );
  return;
}

void euclidean_plane::updateLine(sf::VertexArray& logicalLine, line_segment& physicalLine) {
  logicalLine[0].position = sf::Vector2f(physicalLine.p.x + xBias, yBias - physicalLine.p.y);
  logicalLine[1].position = sf::Vector2f(physicalLine.q.x + xBias, yBias - physicalLine.q.y);
  return;
}

void euclidean_plane::updateSweep(sf::VertexArray& logicalSweep, const sweep_line& physicalSweep) {
  logicalSweep[0].position = sf::Vector2f(physicalSweep.sweep.p.x, physicalSweep.sweep.p.y);
  logicalSweep[1].position = sf::Vector2f(physicalSweep.sweep.q.x, physicalSweep.sweep.q.y);
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

void euclidean_plane::updateNaiveIntersectionPointInfo() {
  m_naiveIntersectionPointText.setString("i(" + toString(m_physicalIntersectionPointNaive.x) + ", " + toString(m_physicalIntersectionPointNaive.y) + ")");
  return;
}

void euclidean_plane::updateSweepIntersectionPointInfo() {
  m_sweepIntersectionPointText.setString("i(" + toString(m_physicalIntersectionPointSweep.x) + ", " + toString(m_physicalIntersectionPointSweep.y) + ")");
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
  // Time Recording Mode On/Off
  if (m_calcTime) m_timeRecordingModeText.setString("Time On");
  else m_timeRecordingModeText.setString("Time Off");
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

void euclidean_plane::readWriteMultiPairSetToFile() {
  std::string fileName{ m_saveFileName + toString(m_saveFileNumber) + m_saveFileExtension }; // the name of the file to read/write from/to
  if (m_currentlyInReadMode) { // read from file
    std::ifstream fileToRead{ fileName };
    if (!fileToRead) { // if file doesn't exist or permissions are not granted
      std::cout << "file could not be read\n";
      return;
    }
    m_physicalMultiPairSet.clear(); // clear physical set
    m_logicalMultiPairSet.clear(); // clear logical set
    { line_segment ls{}; ls.resetID(); } // reset line segment unique IDs
    while (fileToRead) { // haven't reached the end of file (EOF)
      std::string strInput{}; // input string
      getline(fileToRead, strInput); // whole line (without new line character)
      if (strInput.find('!') != strInput.npos || strInput.find("\/\/") != strInput.npos) continue;
      std::vector<std::string> strResult; // result string vector
      boost::split(strResult, strInput, boost::is_any_of(" ")); // split string and push back each parced string in above vector
      if (strResult.size() != 4) continue; // if line segment is not represented correctly
      try { // try creating the physical line
        int px{ boost::lexical_cast<int>(strResult.at(0)) }; // create px
        int py{ boost::lexical_cast<int>(strResult.at(1)) }; // create py
        int qx{ boost::lexical_cast<int>(strResult.at(2)) }; // create qx
        int qy{ boost::lexical_cast<int>(strResult.at(3)) }; // create qy
        line_segment pLine{ (double)px, (double)py, (double)qx, (double)qy }; // form physical line from above coordinates
        sf::VertexArray lLine{ sf::Lines, 2 }; // logical line
        updateLine(lLine, pLine); // make logical line match physical
        m_physicalMultiPairSet.push_back(pLine); // add physical line to current set
        m_logicalMultiPairSet.push_back(lLine); // add logical line to current set
      } catch (boost::bad_lexical_cast&) { // if any of the coordinates is not a number
        continue; // ignore this line
      }
    }
    // set pair size and update it on screen
    m_multiPairSetSize = m_physicalMultiPairSet.size();
    updateSimulationStateInfo();
    // perform important checks
    if (m_multiPairTrashed) m_multiPairTrashed = false;
    if (m_drawMultiPairIntersectionsNaive) m_drawMultiPairIntersectionsNaive = false;
    if (m_drawMultiPairIntersectionsSweep) m_drawMultiPairIntersectionsSweep = false;
    if (m_drawMultiPairSweepLine) m_drawMultiPairSweepLine = false;
    if (m_currentlyCalculatingIntersectionsNaive) m_currentlyCalculatingIntersectionsNaive = false;
    if (m_currentlyCalculatingIntersectionsSweep && !m_lastSweptSinglePair) m_currentlyCalculatingIntersectionsSweep = false;
    std::cout << "line segments loaded successfully\n";
  } else { // write to file
    if (m_physicalSinglePairSet.empty()) { // line segment set is empty
      std::cout << "no line segments in set" << '\n';
      return;
    }
    std::ofstream fileToWrite{ fileName };
    if (!fileToWrite) { // unable to write to file
      std::cout << "could not write to file\n";
      return;
    }
    for (line_segment lineSeg : m_physicalMultiPairSet) { // write every line in the file in a 'px py qx qy' format
      fileToWrite
        << lineSeg.p.x << ' '
        << lineSeg.p.y << ' '
        << lineSeg.q.x << ' '
        << lineSeg.q.y << std::endl;
    }
    std::cout << "line segments saved successfully\n";
  }
  return;
}

bool euclidean_plane::containsPoint(std::vector<point>& vec, point& p) {
  for (point pVec : vec) if (p.eq(pVec)) return true;
  return false;
}

double euclidean_plane::orientation(point a, point b, point c) {
  return (b.y - a.y) * (c.x - b.x) - (c.y - b.y) * (b.x - a.x);
}
