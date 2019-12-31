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
// Constructors & Destructor
public:
	event_queue(std::vector<line_segment>& lineSet);
	~event_queue();
// Member Functions
public:
	void initialize();
	void add(const event_point& intersectionEventPoint);
	event_point getNextEventPoint();
	float nextEventPointPosition();
	bool isEmpty();
	void clear();
	void changeSet(std::vector<line_segment>& lineSet);
	void print();
// Member Variables
private:
	std::vector<event_point> m_queue;
	std::vector<line_segment>* m_lineSet;
};
