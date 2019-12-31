#pragma once
// # ### #################################################################################### ### #

// STL : Standard Template Library
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "global.h"
#include "line_segment.h"
#include "event_queue.h"
#include "status_structure.h"

// # ### #################################################################################### ### #

class sweep_line {
public:
	// Constructors & Destructor
	sweep_line(std::vector<line_segment>& lineSet);
	~sweep_line();
	// Member Functions
	void reset();
	void advance();
	void advance(const float& position);
	bool reachedEnd();
	void handleEventPoint(event_point ep);
	void changeQueueSet(std::vector<line_segment>& lineSet);
	// Member Variables
	line_segment sweep;
	event_queue Q;
	status_structure T;
private:
	float step;
	int start;
	int end;
};
