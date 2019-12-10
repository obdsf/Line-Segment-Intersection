#include "euclidean_plane.h"
#include <iostream>
#include <random>

const sf::Time euclidean_plane::timePerFrame = sf::seconds(1.f / 60.f);

euclidean_plane::euclidean_plane()
  : m_window(sf::VideoMode(500, 500), "SFML Application")
  , m_logicalLineA{ sf::Lines, 2 }, m_logicalLineB{ sf::Lines, 2 }
  , m_physicalLineA{ 0, 0, 0, 0 }, m_physicalLineB{ 0, 0, 0, 0 }
  , m_font{}, m_statisticsText{}
  , m_statisticsUpdateTime{}, m_statisticsNumFrames{ 0 }
  , m_genNewSetOfLines{ false }, m_distribution{ 0, 500 }
{
  m_font.loadFromFile("fonts/Sansation.ttf");
  m_statisticsText.setFont(m_font);
  m_statisticsText.setPosition(5.f, 5.f);
  m_statisticsText.setCharacterSize(10);
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
}

void euclidean_plane::update() {
  if (m_genNewSetOfLines) {
    genNewSetOfLines(m_physicalLineA);
    genNewSetOfLines(m_physicalLineB);
  }

  updateLine(m_logicalLineA, m_physicalLineA);
  updateLine(m_logicalLineB, m_physicalLineB);
}

void euclidean_plane::render() {
  m_window.clear();
  m_window.draw(m_logicalLineA);
  m_window.draw(m_logicalLineB);
  m_window.draw(m_statisticsText);
  m_window.display();
}

void euclidean_plane::handleUserInput(sf::Keyboard::Key key, bool isPressed) {
  if (key == sf::Keyboard::Space) m_genNewSetOfLines = isPressed;
}

void euclidean_plane::genNewSetOfLines(line_segment &physicalLine) {
  physicalLine.p.x = m_distribution(m_generator);
  physicalLine.p.y = m_distribution(m_generator);
  physicalLine.q.x = m_distribution(m_generator);
  physicalLine.q.y = m_distribution(m_generator);
}

void euclidean_plane::updateLine(sf::VertexArray &logicalLine, line_segment &physicalLine) {
  logicalLine[0].position = sf::Vector2f(physicalLine.p.x, physicalLine.p.y);
  logicalLine[1].position = sf::Vector2f(physicalLine.q.x, physicalLine.q.y);
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
}

template <typename T>
std::string euclidean_plane::toString(const T& value) {
  std::stringstream stream;
  stream << value;
  return stream.str();
}
