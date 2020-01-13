
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
  { // Add first 2 event points to be able to compare the rest to them
    event_point upperEndPoint{ m_lineSet->at(0).upperEndPoint, m_lineSet->at(0) };
    event_point lowerEndPoint{ m_lineSet->at(0).lowerEndPoint };
    m_queue.push_back(lowerEndPoint);
    m_queue.push_back(upperEndPoint);
  }
  for (int i = 1; i < m_lineSet->size(); i++) { // start from the second line and add it's 2 event points
    line_segment* currLine{ &m_lineSet->at(i) }; // prevents re-calculation of 'std::vector.at()' later
    event_point upperEndPoint{ m_lineSet->at(i).upperEndPoint, *currLine }; // create ep from upper end point
    event_point lowerEndPoint{ m_lineSet->at(i).lowerEndPoint }; // create ep from lower end point
    bool insertUpper{ true }; // upper end point is yet to be inserted
    bool insertLower{ true }; // lower end point is yet to be inserted
    std::vector<event_point>::iterator it;
    for (it = m_queue.begin(); it != m_queue.end(); it++) { // start scanning the queue
      if (insertUpper && upperEndPoint.p.y < it->p.y) { // queue is reversed (higher y value event points are to the back)
        it = m_queue.insert(it, upperEndPoint);
        insertUpper = false; // stop searching for the upper event point, it's already in place
      } else if (insertUpper && upperEndPoint.p.eq(it->p)) { // event point already exists
        it->addLine(*currLine); // simply add the line to this event points list of lines
        insertUpper = false; // stop searching for the upper event point
      }
      if (insertLower && lowerEndPoint.p.y < it->p.y) {
        it = m_queue.insert(it, lowerEndPoint);
        insertLower = false;
      } else if (insertLower && lowerEndPoint.p.eq(it->p)) {// event point already exists
        insertLower = false;
      }
      if (!insertUpper && !insertLower) break; // if both event points are in place stop the scanning
    }
    if (insertLower) m_queue.push_back(lowerEndPoint); // if lower event point wasn't handled, add it to the end of the list
    if (insertUpper) m_queue.push_back(upperEndPoint); // if upper event point wasn't handled, add it above the lower (to the back, queue is inverted)
  }
  return;
}

void event_queue::add(const event_point intersectionEventPoint) {
   /* adds a new found (intersection) point to the queue as an event point
    * event points added this way cannot be passed by reference since they
    * are temporary and will be deleted afterwards by the caller function
    */
  std::vector<event_point>::iterator it;
  bool insertIntersectionEventPoint{ true };
  for (it = m_queue.begin(); it != m_queue.end(); it++) {
    if (intersectionEventPoint.p.y < it->p.y) {
      m_queue.insert(it, intersectionEventPoint);
      insertIntersectionEventPoint = false;
      break;
    } else if (intersectionEventPoint.p.y == it->p.y) { // in same y events are treated from left to right (Q is inverted, next event is back)
      if (intersectionEventPoint.p.x > it->p.x) {
        m_queue.insert(it, intersectionEventPoint);
        insertIntersectionEventPoint = false;
        break;
      }
    }
  }
  if (insertIntersectionEventPoint) m_queue.push_back(intersectionEventPoint);
  return;
}

event_point event_queue::getNextEventPoint() {
  event_point nextEventPoint{ m_queue.back() };
  m_queue.pop_back();
  return nextEventPoint;
}

double event_queue::nextEventPointPosition() {
  if (m_queue.empty()) return NULL;
  else return m_queue.back().p.y;
}

bool event_queue::contains(event_point& epNew) {
  if (m_queue.empty()) return false;
  for (event_point ep : m_queue) {
    if (ep.p.eq(epNew.p)) {
      return true;
    }
  }
  return false;
}

bool event_queue::empty() {
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
    std::cout << "\t[" << p.p.y << ']';
    if ((++i) % 10 == 0) std::cout << "\n\n";
  }
  if ((i) % 10 != 0) std::cout << "\n\n";
  std::cout << m_queue.size() << "\n\n";
}
