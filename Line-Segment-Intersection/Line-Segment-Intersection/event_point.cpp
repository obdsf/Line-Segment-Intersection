
// # ### #################################################################################### ### #

// STL : Standard Template Library
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "event_point.h"

// # ### #################################################################################### ### #

// Constructors & Destructor
event_point::event_point(point& p, line_segment& line)
  : p{ &p }, lines{}
{
  lines.push_back(&line);
}

event_point::event_point(point& p)
  : p{ &p }, lines{}
{}

event_point::~event_point() {}

// Member Functions
void event_point::addLine(line_segment& line) {
  lines.push_back(&line);
}
