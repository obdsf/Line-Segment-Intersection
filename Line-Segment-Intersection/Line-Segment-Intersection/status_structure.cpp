
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <iterator>
#include <vector>
#include <iostream>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "global.h"
#include "status_structure.h"
#include "event_point.h"
#include "line_segment.h"

// # ### #################################################################################### ### #

// Constructors & Destructor
status_structure::status_structure()
  : m_status{}
{}

status_structure::~status_structure() {}

// Member Functions
void status_structure::add(line_segment* lineSegToAdd, bool useMinorStep) {
  float yValue{ lineSegToAdd->upperEndPoint.y };
  if (useMinorStep) yValue -= precision / 2;
  auto it = m_status.begin();
  for (line_segment* lineSeg : m_status) {
    if (lineSegToAdd->solveForX(yValue) < lineSeg->solveForX(yValue)) break; // horizontal segments are inserted last (solveForX Implementation ensures this)
    it++;
  }
  m_status.insert(it, lineSegToAdd);
}

void status_structure::find(std::vector<line_segment*>& linesL, std::vector<line_segment*>& linesC, event_point& ep) {
  bool consecutiveLinesChainBroke{ false };
  /* optional optimization measure: whenever a chain of neighbor lines ends
   * (after the first line found that contains the event point then
   * if any line is found that does not contain it, it breaks the chain)
   * then no line from that point on can contain the same event point
   * so the function exits
   */
  for (line_segment* lineSeg : m_status) {
    if (lineSeg->lowerEndPoint.eq(*ep.p)) {
      linesL.push_back(lineSeg);
      consecutiveLinesChainBroke = true;
    }
    else if (lineSeg->contains(*ep.p)) {
      linesC.push_back(lineSeg);
      consecutiveLinesChainBroke = true;
    }
    else if (consecutiveLinesChainBroke) break;
  }
  return;
}

int status_structure::findAdjacentSegments(line_segment*& leftSeg, line_segment*& rightSeg, event_point& ep) {
  if (m_status.empty()) return false; // found no adjacent segments, both segment pointers are invalid (the numeric value of true is 0)
  bool foundRight{ false };
  float epyValue{ ep.p->y };
  float epxValue{ ep.p->x };
  int leftIter{ -1 };
  for (line_segment* lineSeg : m_status) {
    if (lineSeg->solveForX(epyValue) > epxValue) {
      rightSeg = lineSeg;
      foundRight = true;
      break;
    }
    leftIter++;
  }
  if (foundRight) {
    if (leftIter == -1) { // same as 'if(segRight == m_status.front())'
      return INT_MAX; // found only right segment, left segment pointer is invalid
    } else {
      leftSeg = m_status.at(leftIter);
      return true; // found both segments, both segment pointers are valid (the numeric value of true is 1)
    }
  } else {
    leftSeg = m_status.back();
    return INT_MIN; // found only left segment, right segment pointer is invalid
  }
}

int status_structure::findBoundariesOfUnion(line_segment*& leftSeg, line_segment*& leftmostSeg,
                                            line_segment*& rightmostSeg, line_segment*& rightSeg,
                                            std::vector<line_segment*>& linesU, std::vector<line_segment*>& linesC,
                                            event_point& ep) {
  std::cout << "union " << ++debuggingUnionCounter << '\n';
  if (m_status.empty()) return 0; // all pointers are invalid (0)
  if (linesU.empty()) { // linesC has both boundaries
    leftmostSeg = linesC.front();
    rightmostSeg = linesC.back();
  } else if (linesC.empty()) { // linesU has both boundaries
    leftmostSeg = linesU.front();
    rightmostSeg = linesU.back();
  } else { // not sure which set has the left and right boundary
    float yValue{ep.p->y}; // prevents re-calculation
    line_segment* potentialLeftmostSegC{ linesC.front() }; // candidate of C for leftmost boundary
    line_segment* potentialLeftmostSegU{ linesU.front() }; // candidate of U for leftmost boundary
    line_segment* potentialRightmostSegC{ linesC.back() }; // candidate of C for rightmost boundary
    line_segment* potentialRightmostSegU{ linesU.back() }; // candidate of U for rightmost boundary
    if (potentialLeftmostSegC->solveForX(yValue) < potentialLeftmostSegU->solveForX(yValue)) leftmostSeg = potentialLeftmostSegC;
    else leftmostSeg = potentialLeftmostSegU; // smallest x is left boundary
    if (potentialRightmostSegC->solveForX(yValue) > potentialRightmostSegU->solveForX(yValue)) rightmostSeg = potentialRightmostSegC;
    else rightmostSeg = potentialRightmostSegU; // biggest x is right boundary
  }
  unsigned int returnValue{ 1 }; // both left and right segment pointers are valid (1)
  std::vector<line_segment*>::iterator leftmostIter = std::find(m_status.begin(), m_status.end(), leftmostSeg); // iterator to leftmostSeg in m_status
  std::vector<line_segment*>::iterator rightmostIter = std::find(m_status.begin(), m_status.end(), rightmostSeg); // iterator to of rightmostSeg in m_status
  if (leftmostIter == m_status.begin()) returnValue += 1; // left segment pointer is invalid (2)
  else leftSeg = *(--leftmostIter);
  if (rightmostIter == m_status.end() - 1) returnValue += 2; // right segment pointer is invalid (3)
  else rightSeg = *(++rightmostIter);
  return returnValue;
  /* returnValue possible values and their meanings:
   * (0): m_status is empty, all pointers are invalid and should not be used
   * (1): all pointers are valid
   * (2): left segment pointer is invalid, all other pointers are valid (including right segment pointer)
   * (3): right segment pointer is invalid, all other pointers are valid (including left segment pointer)
   * (4): left and right segment pointers are invalid, all other pointers are valid
   */
}

void status_structure::erase(std::vector<line_segment*>& linesL, std::vector<line_segment*>& linesC) {
  if (linesL.empty() && linesC.empty()) return;
  if (!linesL.empty()) {
    for (line_segment* lineSeg : linesL) {
      m_status.erase(std::remove(m_status.begin(), m_status.end(), lineSeg), m_status.end());
    }
  }
  if (!linesC.empty()) {
    for (line_segment* lineSeg : linesC) {
      m_status.erase(std::remove(m_status.begin(), m_status.end(), lineSeg), m_status.end());
    }
  }
  return;
}

bool status_structure::isEmpty() {
  if (m_status.empty()) return true;
  return false;
}

void status_structure::clear() {
  m_status.clear();
  return;
}

void status_structure::print() {
  int i{ 0 };
  std::cout << "___________________________________________________________________________________________________\n";
  for (line_segment* lineSeg : m_status) {
    std::cout << "\t[" << lineSeg->name << ']';
    if ((++i) % 10 == 0) std::cout << "\n\n";
  }
  if ((i) % 10 != 0) std::cout << "\n\n";
  std::cout << m_status.size() << "\n\n";
}
