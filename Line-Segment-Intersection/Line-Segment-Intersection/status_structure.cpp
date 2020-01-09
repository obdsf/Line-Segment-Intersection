
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
  if (useMinorStep) yValue -= g_precision / 2;
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

int status_structure::findAdjacentSegments(line_segment* leftSeg, line_segment* rightSeg, event_point& ep) {
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

int status_structure::findBoundariesOfUnion(line_segment* leftSeg, line_segment* leftmostSeg,
                                            line_segment* rightmostSeg, line_segment* rightSeg,
                                            std::vector<line_segment*>& linesU, std::vector<line_segment*>& linesC) {
  if (m_status.empty()) {
    return false;
  } else if (m_status.size() == 1) { // this check is needed because the for loop only works for m_statis.size() > 2
    leftmostSeg = m_status.front();
    rightmostSeg = leftmostSeg;
    return -1;
  } else if (m_status.size() == 2) { // this check is needed because the for loop only works for m_statis.size() > 2
   leftmostSeg = m_status.front();
   rightmostSeg = m_status.back();
   return -2;
  }
  line_segment* potentialLeftmostSegU{ linesU.front() };
  line_segment* potentialLeftmostSegC{ linesC.front() };
  line_segment* potentialRightmostSegU{ linesU.back() };
  line_segment* potentialRightmostSegC{ linesC.back() };
  int leftIter{ -1 };
  int rightIter{ 1 };
  leftmostSeg = potentialLeftmostSegU;
  rightmostSeg = potentialRightmostSegU;
  bool leftSideActive{ true }; // still searching for left and leftmost segments
  bool foundFirstPotentialRightmostSeg{ false };
  for (line_segment* lineSeg : m_status) { // only works for m_status.size() > 2
    if (leftSideActive) { // look for the leftmost boundary
      if (lineSeg == potentialLeftmostSegC) {
        leftmostSeg = potentialLeftmostSegC;
        leftIter = rightIter - 2;
        leftSideActive = false;
      } else if (lineSeg == potentialLeftmostSegU) {
        leftIter = rightIter - 2;
        leftSideActive = false;
      }
    } else { // look for the rightmost boundary
      if (lineSeg == potentialRightmostSegC) {
        if (foundFirstPotentialRightmostSeg) {
          rightmostSeg = potentialRightmostSegC;
          break;
        }
        foundFirstPotentialRightmostSeg = true;
      } else if (lineSeg == potentialRightmostSegU) {
        if (foundFirstPotentialRightmostSeg) {
          rightmostSeg = potentialRightmostSegU;
          break;
        }
        foundFirstPotentialRightmostSeg = true;
      }
    }
    rightIter++;
  }
  int returnValue{ 1 }; // found both segments, both left and right segment pointers are valid (true)
  if (leftIter == -1) {
    returnValue++; // left segment pointer is invalid (2)
  } else {
    leftSeg = m_status.at(leftIter);
  }
  if (rightIter == m_status.size()) {
    returnValue += 2; // right segment pointer is invalid (3)
  } else {
    rightSeg = m_status.at(rightIter);
  }
  return returnValue; // if returnValue is 4 then both left and right segment pointers are invalid
}

void status_structure::erase(std::vector<line_segment*>& linesL, std::vector<line_segment*>& linesC) {
  if (linesL.empty() && linesC.empty()) return;
  bool checkForL{ false };
  bool checkForC{ false };
  line_segment* firstLineL = nullptr;
  line_segment* firstLineC = nullptr;
  int posL{ -1 };
  int posC{ -1 };
  if (!linesL.empty()) {
    checkForL = true;
    firstLineL = linesL.front();
  }
  if (!linesC.empty()) {
    checkForC = true;
    firstLineC = linesC.front();
  }
  for (line_segment* lineSeg : m_status) {
    if (checkForL) {
      posL++;
      if (lineSeg->name == firstLineL->name) checkForL = false;
    }
    if (checkForC) {
      posC++;
      if (lineSeg->name == firstLineC->name) checkForC = false;
    }
    if (!checkForL && !checkForC) break;
  }
  if (posL > -1) {
    auto first{ m_status.begin() + posL };
    auto last{ first + m_status.size() };
    m_status.erase(first, last);
  }
  if (posC > -1) {
    auto first{ m_status.begin() + posC };
    auto last{ first + m_status.size() };
    m_status.erase(first, last);
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
