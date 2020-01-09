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
	void add(line_segment* lineSegToAdd, bool useMinorStep = false);
	void find(std::vector<line_segment*>& linesL, std::vector<line_segment*>& linesC, event_point& ep);
	int findAdjacentSegments(line_segment* leftSeg, line_segment* rightSeg, event_point& ep);
	int findBoundariesOfUnion(line_segment* leftSeg, line_segment* leftmostSeg,
														line_segment* rightmostSeg, line_segment* rightSeg,
														std::vector<line_segment*>& linesU, std::vector<line_segment*>& linesC);
	void erase(std::vector<line_segment*>& linesL, std::vector<line_segment*>& linesC);
	bool isEmpty();
	void clear();
	void print();
// Member Variables
private:
	std::vector<line_segment*> m_status;
};
