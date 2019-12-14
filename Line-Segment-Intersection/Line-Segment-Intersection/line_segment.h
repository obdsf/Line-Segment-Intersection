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
	bool eq(line_segment& l);
	bool parallel(line_segment& l);
	bool collinear(line_segment& l);
	bool partially_contains(point& k);
	bool contains(point& k);
	bool partially_intersects(line_segment& l, point& k);
	bool intersects(line_segment& l, point& k);
	// Member Variables
	point p;
	point q;
	float slope;
	float yIntercept;
	int xMin;
	int xMax;
	int yMin;
	int yMax;

private:
	// Member Functions
	float calcSlope();
	float calcYIntercept();
};
