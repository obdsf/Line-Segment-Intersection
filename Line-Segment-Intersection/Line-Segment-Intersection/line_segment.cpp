#include <iostream>
#include "point.h"
#include "line_segment.h"

// Constructors & Destructor
line_segment::line_segment(point p, point q)
	:p{ p }, q{ q }, slope{ calcSlope() }
{}

line_segment::~line_segment() {}

// Member Functions
void line_segment::print() {
	p.print();
	std::cout << "<--->";
	q.print();
	std::cout << " || slope: " << slope;
	return;
}

bool line_segment::eq(line_segment l) {
	return false;
}

float line_segment::calcSlope() {
	return (q.y - p.y) / (q.x - p.x);
}
