#pragma once
//#include "line_segment.h"

class point {
public:
	// Constructors & Destructor
	point(float x = 0, float y = 0);
	~point();
	// Member Functions
	void print();
	bool eq(point p);
	// Member Variables
	float x;
	float y;
	bool definesLineSegment;

private:

};
