
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <iostream>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "global.h"
#include "sweep_line.h"

// # ### #################################################################################### ### #

// Constructors & Destructor
sweep_line::sweep_line()
  : sweep{ epbTopLeft, epbTopRight }
  , Q{}, T{}, position{}
  , m_step{ 2 * precision }
  , scope{ m_step }
  , m_start{ (int)epbTopLeft.y }
  , m_end{ (int)epbBotLeft.y }
{}

sweep_line::~sweep_line() {}

// Member Functions
void sweep_line::reset() {
  sweep.update(epbTopLeft, epbTopRight);
}

void sweep_line::advance() {
  sweep.p.y = truncOneDigit(sweep.p.y + m_step);
  sweep.q.y = truncOneDigit(sweep.q.y + m_step);
  position = yBias - sweep.p.y;
  /* truncOneDigit function truncs every digit after the first decimal digit
   * just like trunc, there are cases of error due to how floating point
   * numbers operate (number 512.1 causes truncOneDigit to produce the same
   * result -in our case error- when we start counting from 300 with step
   * equal to 0.1, in our case with step equal to 0.2 and the same starting
   * point we are bound to hit the same wall somewhere above 1000, but that
   * never happens since we stop at 900).
   * floating point numbers are impossible to trunc because they're
   * impossible to even represent accurately
   * (https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html)
   */
  return;
}

void sweep_line::advance(const float& position) {
  if (position >= m_start && position <= m_end) {
    sweep.p.y = position;
    sweep.q.y = position;
    this->position = yBias - sweep.p.y;
  }
  return;
}

bool sweep_line::reachedEnd() {
  if (sweep.p.y > epbBotLeft.y)return true;
  return false;
}

bool sweep_line::handleEventPoint(event_point ep, point& intersectionPoint) {
  /* "Let U(p) be the set of segments whose upper endpoint is p; these segments
   *  are stored with the event point p. (For horizontal segments, the upper
   *  endpoint is by definition the left endpoint.)"
   */
  bool epIsIntersectionPoint{ false };
  std::vector<line_segment*> linesL; // "Let L(p) denote the subset of segments found whose lower endpoint is p"
  std::vector<line_segment*> linesC; // "Let C(p) denote the subset of segments found that contain p in their interior"
  T.find(linesL, linesC, ep); // "Find all segments stored in T that contain p;"
  if (ep.linesU.size() + linesL.size() + linesC.size() > 1) { // "If the union of L(p), U(p) and C(p) contains more than one segment"
    epIsIntersectionPoint = true; // "then report p as an intersection"
    intersectionPoint = ep.p;

    std::cout << "found intersection!"; // TEMP TEST
    ep.p.print(); // TEMP TEST
  }
  T.erase(linesL, linesC); // "Delete the segments in the union of L(p) and C(p) from T"
  for (line_segment* lineSeg : ep.linesU) { // "Insert the segments in U(p) into T"
    std::cout << "upper end point: " << lineSeg->name << '\n'; // TEMP TEST
    T.add(lineSeg);
  }
  for (line_segment* lineSeg : linesC) { // "Insert the segments in C(p) into T"
    T.add(lineSeg, true, ep.p.y);
  } // "Deleting and re-inserting the segments of C(p) reverses their order"
  if (ep.linesU.empty() && linesC.empty()) {
    line_segment* leftSeg{ nullptr };
    line_segment* rightSeg{ nullptr };
    //leftSeg != nullptr && rightSeg != nullptr
    if (T.findAdjacentSegments(leftSeg, rightSeg, ep)) { // "Let sl and sr be the left and right neighbors of p in T"
      findNewEvent(*leftSeg, *rightSeg, ep); // "FINDNEWEVENT(sl,sr, p)"
    }
  } else {
    /* "Let s' be the leftmost segment of union U(p), C(p) in T
     *  Let sl be the left neighbor of s' in T
     *  Let s'' be the rightmost segment of union U(p), C(p) in T
     *  Let sr be the right neighbor of s'' in T
     *  FINDNEWEVENT(sl, s', p)
     *  FINDNEWEVENT(s'', sr, p)"
     */
    line_segment* leftSeg{ nullptr };
    line_segment* leftmostSeg{ nullptr };
    line_segment* rightmostSeg{ nullptr };
    line_segment* rightSeg{ nullptr };
    int kappa{ T.findBoundariesOfUnion(leftSeg, leftmostSeg, rightmostSeg, rightSeg, ep.linesU, linesC, ep) };
    std::cout << "kappa: " << kappa << '\n'; // TEMP TEST
    /*kappa == 1 || kappa == 3*/
    if (leftSeg != nullptr && leftmostSeg != nullptr) {
      std::cout << "left side: leftSeg: " << leftSeg->name << ", leftmostSeg: " << leftmostSeg->name << '\n'; // TEMP TEST
      findNewEvent(*leftSeg, *leftmostSeg, ep); // conditions checked ensure pointers are not null
    }
    /*kappa == 1 || kappa == 2*/
    if (rightmostSeg != nullptr && rightSeg != nullptr) {
      std::cout << "right side: rightmostSeg: " << rightmostSeg->name << ", rightSeg: " << rightSeg->name << '\n'; // TEMP TEST
      findNewEvent(*rightmostSeg, *rightSeg, ep); // conditions checked ensure pointers are not null
    }
  }
  return epIsIntersectionPoint; // if ep was an intersection point return true, else return false
}

void sweep_line::findNewEvent(line_segment& leftSeg, line_segment& rightSeg, event_point& ep) {
  point intersectionPoint{};
  std::cout << "trying to find new event...\n"; // TEMP TEST
  if (leftSeg.intersects(rightSeg, intersectionPoint)) {
    std::cout << "trying harder...\n"; // TEMP TEST
    if (intersectionPoint.y < ep.p.y || intersectionPoint.y == ep.p.y && intersectionPoint.x > ep.p.x) {
      std::cout << "trying harderer...\n"; // TEMP TEST
      event_point epNew{ intersectionPoint };
      if (!Q.contains(epNew)) {
        Q.add(epNew);
        std::cout << "new event FOUND!\n"; // TEMP TEST
      }
    }
  }
}

/* Soon to be removed
void sweep_line::changeQueueSet(std::vector<line_segment>& lineSet) {
  Q.changeSet(lineSet);
  return;
}

void sweep_line::changeIntersectionPointSet(std::vector<point>& intersectionPointSet) {
  m_intersectionPointSet = &intersectionPointSet;
  return;
}
*/
