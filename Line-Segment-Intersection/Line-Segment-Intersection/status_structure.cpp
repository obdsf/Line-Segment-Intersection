
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <iterator>
#include <vector>
#include <iostream>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
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
void status_structure::add(line_segment& lineSegToAdd) {
  auto it = m_status.begin();
  for (line_segment* lineSeg : m_status) {
    if (lineSegToAdd.upperEndPoint.x < lineSeg->solveForX(lineSegToAdd.upperEndPoint.y)) break;
    it++;
  }
  m_status.insert(it, &lineSegToAdd);
}

void status_structure::find(event_point& ep, std::vector<line_segment*>& linesL, std::vector<line_segment*>& linesC) {
  bool consecutiveLinesChainBroke{ false }; // optional optimization measure: whenever a chain of neighbor lines ends
                                            // (after the first line found that contains the event point then
                                            // if any line is found that does not contain it, it breaks the chain)
                                            // then no line from that point on can contain the same event point
                                            // so the function exits
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
