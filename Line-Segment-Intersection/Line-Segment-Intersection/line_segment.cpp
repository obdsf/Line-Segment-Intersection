
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
line_segment::line_segment(point& p, point& q)
	:p{ p }, q{ q }, slope{ calcSlope() }, yIntercept{ calcYIntercept() }
{}

line_segment::line_segment(int px, int py, int qx, int qy) {
	p.update(px, py);
	q.update(qx, qy);
	slope = calcSlope();
	yIntercept = calcYIntercept();
}

line_segment::~line_segment() {}

// Member Functions
void line_segment::update(int px, int py, int qx, int qy) {
	p.update(px, py);
	q.update(qx, qy);
	slope = calcSlope();
	yIntercept = calcYIntercept();
	return;
}

void line_segment::print() {
	std::cout
		<< "(" << p.x << ", " << p.y << ")"
		<< " <---> "
		<< "(" << q.x << ", " << q.y << ")"
		<< " || slope: " << slope
		<< " || y-intercept: " << yIntercept
		<< '\n';
	return;
}

bool line_segment::eq(line_segment& l) {
	if (p.eq(l.p) && q.eq(l.q) || q.eq(l.p) && p.eq(l.q)) return true;
	return false;
}

float line_segment::calcSlope() {
	if (p.x == q.x) return NULL;
	return ((float)q.y - (float)p.y) / ((float)q.x - (float)p.x);
}

float line_segment::calcYIntercept() {
	if (slope == NULL) return NULL;
	return (float)p.y - (float)p.x * slope;
}

bool line_segment::parallel(line_segment& l) {
	if (slope == l.slope) return true;
	return false;
}

bool line_segment::collinear(line_segment& l) {
	if (parallel(l) && yIntercept == l.yIntercept) return true;
	return false;
}

bool line_segment::contains(point k) {
	
}
