
// # ### #################################################################################### ### #

// STL : Standard Template Library
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "event_point.h"

// # ### #################################################################################### ### #

// Constructors & Destructor
event_point::event_point(point& p, line_segment& line)
  : p{ &p }, linesU{}
{
  linesU.push_back(&line);
}

event_point::event_point(point& p)
  : p{ &p }, linesU{}
{}

event_point::~event_point() {}

// Member Functions
void event_point::addLine(line_segment& line) {
  linesU.push_back(&line);
}
