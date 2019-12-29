
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
event_queue::event_queue(std::vector<line_segment>& lineSet)
  : queue{}, lineSet{ &lineSet }
{}

event_queue::~event_queue() {}

// Member Functions
void event_queue::initialize() {
  {
    event_point upperEndPoint{ lineSet->at(0).upperEndPoint, lineSet->at(0) };
    event_point lowerEndPoint{ lineSet->at(0).lowerEndPoint };
    queue.push_back(lowerEndPoint);
    queue.push_back(upperEndPoint);
  }
  for (int i = 1; i < lineSet->size(); i++) {
    event_point upperEndPoint{ lineSet->at(i).upperEndPoint, lineSet->at(i) };
    event_point lowerEndPoint{ lineSet->at(i).lowerEndPoint };
    std::vector<event_point>::iterator it;
    bool insertUpper{ true };
    bool insertLower{ true };
    for (it = queue.begin(); it != queue.end(); it++) {
      if (insertUpper && upperEndPoint.p->y < it->p->y) {
        it = queue.insert(it, upperEndPoint);
        insertUpper = false;
      }
      if (insertLower && lowerEndPoint.p->y < it->p->y) {
        it = queue.insert(it, lowerEndPoint);
        insertLower = false;
      }
      if (!insertUpper && !insertLower) break;
    }
    if (insertLower) queue.push_back(lowerEndPoint);
    if (insertUpper) queue.push_back(upperEndPoint);
  }
  return;
}

void event_queue::add(const event_point& intersectionEventPoint) {
  std::vector<event_point>::iterator it;
  bool inserIntersectionEventPoint{ true };
  for (it = queue.begin(); it != queue.end(); it++) {
    if (intersectionEventPoint.p->y < it->p->y) {
      queue.insert(it, intersectionEventPoint);
      inserIntersectionEventPoint = false;
      break;
    }
  }
  if (inserIntersectionEventPoint) queue.push_back(intersectionEventPoint);
  return;
}

event_point event_queue::getNextEventPoint() {
  event_point nextEventPoint{ queue.back() };
  queue.pop_back();
  return nextEventPoint;
}

void event_queue::printEventPointPositions() {
  int i{ 0 };
  for (event_point p : queue) {
    std::cout << "\t[" << p.p->y << "]";
    if ((++i) % 10 == 0) std::cout << "\n\n";
  }
  if ((i) % 10 != 0) std::cout << "\n\n";
  std::cout << queue.size() << '\n';
}
