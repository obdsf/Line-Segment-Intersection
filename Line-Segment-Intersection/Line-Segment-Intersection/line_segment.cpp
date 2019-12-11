
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

line_segment::line_segment(float px, float py, float qx, float qy) {
	point pp{ px, py };
	point qq{ qx, qy };
	line_segment(pp, qq);
}

line_segment::~line_segment() {}

// Member Functions
void line_segment::update(float px, float py, float qx, float qy) {
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
	if (p.x == q.x) {
		std::cout << p.x-q.x << '\n';
		return 0;
	}
	return (q.y - p.y) / (q.x - p.x);
}

float line_segment::slopeOrientation(line_segment &l) {
	return slope-l.slope;
}
