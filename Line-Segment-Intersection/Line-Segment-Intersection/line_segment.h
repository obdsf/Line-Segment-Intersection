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
	line_segment(const point &p, const point &q);
	line_segment(const float& px, const float& py, const float& qx, const float& qy);
	~line_segment();
	// Member Functions
	void update(const point& pNew, const point& qNew);
	void update(const float& px, const float& py, const float& qx, const float& qy);
	void print();
	bool eq(const line_segment& l);
	bool parallel(const line_segment& l);
	bool collinear(const line_segment& l);
	bool partially_contains(const point& k);
	bool contains(const point& k);
	bool partially_intersects(const line_segment& l, point& k);
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
	void calcLineBoundaries(const float& px, const float& py, const float& qx, const float& qy);
};
