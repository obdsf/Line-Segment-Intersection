#pragma once
#include "point.h"

class line_segment {
public:
	// Constructors & Destructor
	line_segment(point &p, point &q);
	line_segment(float px, float py, float qx, float qy);
	~line_segment();
	// Member Functions
	void update(float px, float py, float qx, float qy);
	void print();
	bool eq(line_segment &l);
	float slopeOrientation(line_segment &l);
	// Member Variables
	point p;
	point q;
	float slope;

private:
	// Member Functions
	float calcSlope();
};
