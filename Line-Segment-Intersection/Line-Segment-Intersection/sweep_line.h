#pragma once
// # ### #################################################################################### ### #

// STL : Standard Template Library
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "global.h"
#include "point.h"
#include "line_segment.h"
#include "event_queue.h"
#include "status_structure.h"

// # ### #################################################################################### ### #

class sweep_line {
// Constructors & Destructor
public:
	sweep_line();
	~sweep_line();
// Member Functions
public:
	void reset();
	void advance();
	void advance(const float& position);
	bool reachedEnd();
	bool handleEventPoint(event_point ep, point* intersectionPoint);
	void changeQueueSet(std::vector<line_segment>& lineSet);
	void changeIntersectionPointSet(std::vector<point>& intersectionPointSet);
// Member Variables
public:
	line_segment sweep;
	event_queue Q;
	status_structure T;
private:
	std::vector<point>* m_intersectionPointSet;
	float m_step;
	int m_start;
	int m_end;
};
