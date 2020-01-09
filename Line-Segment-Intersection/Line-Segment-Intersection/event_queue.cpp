
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <iostream>
#include <iterator>
#include <vector>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "event_point.h"
#include "event_queue.h"
#include "line_segment.h"

// # ### #################################################################################### ### #

// Constructors & Destructor
event_queue::event_queue()
  : m_queue{}, m_lineSet{}
{}

event_queue::~event_queue() {}

// Member Functions
void event_queue::initialize() {
  {
    event_point upperEndPoint{ m_lineSet->at(0).upperEndPoint, m_lineSet->at(0) };
    event_point lowerEndPoint{ m_lineSet->at(0).lowerEndPoint };
    m_queue.push_back(lowerEndPoint);
    m_queue.push_back(upperEndPoint);
  }
  for (int i = 1; i < m_lineSet->size(); i++) {
    event_point upperEndPoint{ m_lineSet->at(i).upperEndPoint, m_lineSet->at(i) };
    event_point lowerEndPoint{ m_lineSet->at(i).lowerEndPoint };
    std::vector<event_point>::iterator it;
    bool insertUpper{ true };
    bool insertLower{ true };
    for (it = m_queue.begin(); it != m_queue.end(); it++) {
      if (insertUpper && upperEndPoint.p->y < it->p->y) {
        it = m_queue.insert(it, upperEndPoint);
        insertUpper = false;
      }
      if (insertLower && lowerEndPoint.p->y < it->p->y) {
        it = m_queue.insert(it, lowerEndPoint);
        insertLower = false;
      }
      if (!insertUpper && !insertLower) break;
    }
    if (insertLower) m_queue.push_back(lowerEndPoint);
    if (insertUpper) m_queue.push_back(upperEndPoint);
  }
  return;
}

void event_queue::add(const event_point& intersectionEventPoint) {
  std::vector<event_point>::iterator it;
  bool inserIntersectionEventPoint{ true };
  for (it = m_queue.begin(); it != m_queue.end(); it++) {
    if (intersectionEventPoint.p->y < it->p->y) {
      m_queue.insert(it, intersectionEventPoint);
      inserIntersectionEventPoint = false;
      break;
    }
  }
  if (inserIntersectionEventPoint) m_queue.push_back(intersectionEventPoint);
  return;
}

event_point event_queue::getNextEventPoint() {
  event_point nextEventPoint{ m_queue.back() };
  m_queue.pop_back();
  return nextEventPoint;
}

float event_queue::nextEventPointPosition() {
  if (m_queue.empty()) return NULL;
  else return m_queue.back().p->y;
}

bool event_queue::isEmpty() {
  if (m_queue.empty()) return true;
  return false;
}

void event_queue::clear() {
  m_queue.clear();
  return;
}

void event_queue::changeSet(std::vector<line_segment>& lineSet) {
  m_lineSet = &lineSet;
  return;
}

void event_queue::print() {
  int i{ 0 };
  std::cout << "___________________________________________________________________________________________________\n";
  for (event_point p : m_queue) {
    std::cout << "\t[" << p.p->y << ']';
    if ((++i) % 10 == 0) std::cout << "\n\n";
  }
  if ((i) % 10 != 0) std::cout << "\n\n";
  std::cout << m_queue.size() << "\n\n";
}
