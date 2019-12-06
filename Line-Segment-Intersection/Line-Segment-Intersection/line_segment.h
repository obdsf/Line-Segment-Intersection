#pragma once
#include "point.h"

class line_segment {
public:
	// Constructors & Destructor
	line_segment(point &p, point &q);
	~line_segment();
	// Member Functions
	void print();
	bool eq(line_segment &l);
	int slopeOrientation(line_segment &l);
	// Member Variables
	point* p;
	point* q;
	float slope;

private:
	// Member Functions
	float calcSlope();
};
