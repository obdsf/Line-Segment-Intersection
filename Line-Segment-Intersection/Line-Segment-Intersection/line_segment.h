#pragma once

// ### # #################################################################################### # ###

// STL : Standard Template Library
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "point.h"

// ### # #################################################################################### # ###

class line_segment {
public:
	// Constructors & Destructor
	line_segment(point &p, point &q);
	line_segment(int px, int py, int qx, int qy);
	~line_segment();
	// Member Functions
	void update(int px, int py, int qx, int qy);
	void print();
	bool eq(line_segment &l);
	float orientation(line_segment &l);
	// Member Variables
	point p;
	point q;
	float slope;

private:
	// Member Functions
	float calcSlope();
};
