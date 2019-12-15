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
	line_segment(float px, float py, float qx, float qy);
	~line_segment();
	// Member Functions
	void update(float px, float py, float qx, float qy);
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
	float xMin;
	float xMax;
	float yMin;
	float yMax;

private:
	// Member Functions
	float calcSlope();
	float calcYIntercept();
};
