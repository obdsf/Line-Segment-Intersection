#pragma once
// # ### #################################################################################### ### #

// STL : Standard Template Library
#include <vector>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "event_point.h"
#include "line_segment.h"

// # ### #################################################################################### ### #

class status_structure {
// Constructors & Destructor
public:
	status_structure();
	~status_structure();
// Member Functions
	void add(line_segment& lineSegToAdd);
	void find(event_point& ep, std::vector<line_segment*>& linesL, std::vector<line_segment*>& linesC);
	void print();
// Member Variables
private:
	std::vector<line_segment*> m_status;
};
