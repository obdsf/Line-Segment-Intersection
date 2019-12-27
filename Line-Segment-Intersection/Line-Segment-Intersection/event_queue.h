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

class event_queue {
public:
	// Constructors & Destructor
	event_queue(std::vector<line_segment>& lineSet);
	~event_queue();
	// Member Functions
	void initialize();
	// Member Variables
	std::vector<event_point> queue;
	std::vector<line_segment>* lineSet;
};
