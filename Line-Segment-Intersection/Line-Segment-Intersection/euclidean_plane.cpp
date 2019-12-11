
// ### # #################################################################################### # ###

// STL : Standard Template Library
#include <iostream>
#include <random>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "euclidean_plane.h"

// ### # #################################################################################### # ###

const sf::Time euclidean_plane::timePerFrame = sf::seconds(1.f / 60.f);
const int euclidean_plane::windowWidth{ 1200 };
const int euclidean_plane::windowHeight{ 800 };
const int euclidean_plane::topMargin{ 100 };
const int euclidean_plane::bottomMargin{ 100 };
const int euclidean_plane::rightMargin{ 300 };
const int euclidean_plane::leftMargin{ 300 };
const int euclidean_plane::xBias{ windowWidth / 2 };
const int euclidean_plane::yBias{ windowHeight / 2 };
const int euclidean_plane::distMin{ -300 };
const int euclidean_plane::distMax{ 300 };
const int euclidean_plane::fontSize{ 25 };

euclidean_plane::euclidean_plane()
  : m_window(sf::VideoMode(windowWidth, windowHeight), "SFML Application")
  , m_physicalLineA{ leftMargin, yBias, windowWidth - leftMargin, yBias }
  , m_physicalLineB{ xBias, windowHeight - bottomMargin, xBias, topMargin }
  , m_xAxis{ sf::Lines, 2 }, m_yAxis{ sf::Lines, 2 }
  , m_logicalLineA{ sf::Lines, 2 }, m_logicalLineB{ sf::Lines, 2 }
  , m_font{}, m_statisticsText{}
  , m_lineAText{}, m_lineApText{}, m_lineAqText{}, m_lineAslopeText{}
  , m_lineBText{}, m_lineBpText{}, m_lineBqText{}, m_lineBslopeText{}
  , m_statisticsUpdateTime{}, m_statisticsNumFrames{ 0 }
  , m_genNewSetOfLines{ false }, m_reset{ true }, m_exit{ false }
  , m_distribution{ distMin, distMax }
{
  m_xAxis[0].position = sf::Vector2f(leftMargin, yBias);
  m_xAxis[1].position = sf::Vector2f(windowWidth - leftMargin, yBias);
  m_yAxis[0].position = sf::Vector2f(xBias, windowHeight - bottomMargin);
  m_yAxis[1].position = sf::Vector2f(xBias, topMargin);

  m_logicalLineA[0].color = sf::Color::Red;
  m_logicalLineA[1].color = sf::Color::Yellow;
  m_logicalLineB[0].color = sf::Color::Blue;
  m_logicalLineB[1].color = sf::Color::Cyan;

  m_font.loadFromFile("fonts/Sansation.ttf");
  m_statisticsText.setFont(m_font);
  m_statisticsText.setPosition(5.f, 5.f);
  m_statisticsText.setCharacterSize(fontSize);

  m_lineAText.setFont(m_font);
  m_lineApText.setFont(m_font);
  m_lineAqText.setFont(m_font);
  m_lineAslopeText.setFont(m_font);
  m_lineBText.setFont(m_font);
  m_lineBpText.setFont(m_font);
  m_lineBqText.setFont(m_font);
  m_lineBslopeText.setFont(m_font);

  m_lineAText.setPosition(5.f, windowHeight - 70.f);
  m_lineApText.setPosition(110.f, windowHeight - 70.f);
  m_lineAqText.setPosition(400.f, windowHeight - 70.f);
  m_lineAslopeText.setPosition(700.f, windowHeight - 70.f);
  m_lineBText.setPosition(5.f, windowHeight - 40.f);
  m_lineBpText.setPosition(110.f, windowHeight - 40.f);
  m_lineBqText.setPosition(400.f, windowHeight - 40.f);
  m_lineBslopeText.setPosition(700.f, windowHeight - 40.f);
  
  m_lineAText.setCharacterSize(fontSize);
  m_lineApText.setCharacterSize(fontSize);
  m_lineAqText.setCharacterSize(fontSize);
  m_lineAslopeText.setCharacterSize(fontSize);
  m_lineBText.setCharacterSize(fontSize);
  m_lineBpText.setCharacterSize(fontSize);
  m_lineBqText.setCharacterSize(fontSize);
  m_lineBslopeText.setCharacterSize(fontSize);

  m_lineApText.setFillColor(sf::Color::Red);
  m_lineAqText.setFillColor(sf::Color::Yellow);
  m_lineBpText.setFillColor(sf::Color::Blue);
  m_lineBqText.setFillColor(sf::Color::Cyan);

  m_lineAText.setString("Line A = ");
  m_lineBText.setString("Line B = ");
}

void euclidean_plane::run() {
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  while (m_window.isOpen()) {
    sf::Time elapsedTime = clock.restart();
    timeSinceLastUpdate += elapsedTime;
    while (timeSinceLastUpdate > timePerFrame) {
      timeSinceLastUpdate -= timePerFrame;
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
  if (m_genNewSetOfLines) {
    genNewSetOfLines(m_physicalLineA);
    genNewSetOfLines(m_physicalLineB);
    updateLine(m_logicalLineA, m_physicalLineA);
    updateLine(m_logicalLineB, m_physicalLineB);
    updateCoords();
  }
  if (m_reset) {
    m_distribution.reset();
  }
  if (m_exit) m_window.close();
  return;
}

void euclidean_plane::render() {
  m_window.clear();
  m_window.draw(m_xAxis);
  m_window.draw(m_yAxis);
  m_window.draw(m_statisticsText);
  if (!m_reset) {
  m_window.draw(m_lineAText);
  m_window.draw(m_lineApText);
  m_window.draw(m_lineAqText);
  m_window.draw(m_lineAslopeText);
  m_window.draw(m_lineBText);
  m_window.draw(m_lineBpText);
  m_window.draw(m_lineBqText);
  m_window.draw(m_lineBslopeText);
  m_window.draw(m_logicalLineA);
  m_window.draw(m_logicalLineB);
  }
  m_window.display();
  return;
}

void euclidean_plane::handleUserInput(sf::Keyboard::Key key, bool isPressed) {
  if (m_reset) m_reset = false;
  if (key == sf::Keyboard::Space) m_genNewSetOfLines = isPressed;
  if (key == sf::Keyboard::R) m_reset = true;
  if (key == sf::Keyboard::Escape) m_exit = isPressed;
  return;
}

void euclidean_plane::genNewSetOfLines(line_segment &physicalLine) {
  physicalLine.update(
    m_distribution(m_generator),
    m_distribution(m_generator),
    m_distribution(m_generator),
    m_distribution(m_generator)
  );
  return;
}

void euclidean_plane::updateLine(sf::VertexArray &logicalLine, line_segment &physicalLine) {
  logicalLine[0].position = sf::Vector2f(physicalLine.p.x + xBias, yBias - physicalLine.p.y);
  logicalLine[1].position = sf::Vector2f(physicalLine.q.x + xBias, yBias - physicalLine.q.y);
  return;
}

void euclidean_plane::updateCoords() {
  m_lineApText.setString("p(" + toString(m_physicalLineA.p.x) + ", " + toString(m_physicalLineA.p.y) + "),");
  m_lineAqText.setString("q(" + toString(m_physicalLineA.q.x) + ", " + toString(m_physicalLineA.q.y) + "),");
  m_lineAslopeText.setString("Slope = " + toString(m_physicalLineA.slope));
  m_lineBpText.setString("p(" + toString(m_physicalLineB.p.x) + ", " + toString(m_physicalLineB.p.y) + "),");
  m_lineBqText.setString("q(" + toString(m_physicalLineB.q.x) + ", " + toString(m_physicalLineB.q.y) + "),");
  m_lineBslopeText.setString("Slope = " + toString(m_physicalLineB.slope));
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

template <typename T>
std::string euclidean_plane::toString(const T& value) {
  std::stringstream stream;
  stream << value;
  return stream.str();
}
