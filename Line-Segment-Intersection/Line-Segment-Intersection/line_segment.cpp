
// ### # #################################################################################### # ###

// STL : Standard Template Library
#include <iostream>
// Boost Library
// SFML : Simple and Fast Multimedia Library
// Custom Headers
#include "point.h"
#include "line_segment.h"

// ### # #################################################################################### # ###

// Constructors & Destructor
line_segment::line_segment(point &p, point &q)
	:p{ p }, q{ q }, slope{ calcSlope() }
{}

line_segment::line_segment(int px, int py, int qx, int qy) {
	point pp{ px, py };
	point qq{ qx, qy };
	line_segment(pp, qq);
}

line_segment::~line_segment() {}

// Member Functions
void line_segment::update(int px, int py, int qx, int qy) {
	p.x = px;
	p.y = py;
	q.x = qx;
	q.y = qy;
	slope = calcSlope();
	return;
}

void line_segment::print() {
	p.print();
	std::cout << "<--->";
	q.print();
	std::cout << " || slope: " << slope;
	return;
}

bool line_segment::eq(line_segment &l) {
	if (p.eq(l.p) && q.eq(l.q) || q.eq(l.p) && p.eq(l.q)) return true;
	return false;
}

float line_segment::calcSlope() {
	if (p.x == q.x) return NULL;
	return ((float)q.y - (float)p.y) / ((float)q.x - (float)p.x);
}

float line_segment::orientation(line_segment &l) {
	return slope-l.slope;
}
