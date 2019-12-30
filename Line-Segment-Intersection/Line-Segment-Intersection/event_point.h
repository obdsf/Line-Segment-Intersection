#pragma once
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <vector>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "point.h"
#include "line_segment.h"

// # ### #################################################################################### ### #

class event_point {
public:
	// Constructors & Destructor
	event_point(point& p, line_segment& line);
	event_point(point& p);
	~event_point();
	// Member Functions
	void addLine(line_segment& line);
	// Member Variables
	point* p;
	std::vector<line_segment*> linesU;
};
